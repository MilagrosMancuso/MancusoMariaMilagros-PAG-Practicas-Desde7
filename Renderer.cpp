//
// Created by Mili on 17/09/2025.
//

#include "Renderer.h"
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace PAG {
    Renderer *PAG::Renderer::instancia = nullptr;


    Renderer::Renderer()
            : shaderProg(nullptr),
              idSP(0),
              idVAO(0),
              idVBO(0),
              idIBO(0) {
        // colorFondo por defecto (gris)
        colorFondo[0] = 0.6f;
        colorFondo[1] = 0.6f;
        colorFondo[2] = 0.6f;
        _colorBorrado[0] = 0.6f;
        _colorBorrado[1] = 0.6f;
        _colorBorrado[2] = 0.6f;
        _colorBorrado[3] = 1.0f;
        _mensaje.reserve(64);
    }

    //@todo ojo que en el guion era &. por si da problema
    /**
     * Consulta del objeto unico de la clase
     * @return
     */
    Renderer &Renderer::getInstancia() {
        if (!instancia) {
            instancia = new Renderer();
        }
        return *instancia;

    }

    /**
     * Destructor
     */

    Renderer::~Renderer() {
        // liberar GL
        if (idIBO) {
            glDeleteBuffers(1, &idIBO);
            idIBO = 0;
        }
        if (idVBO) {
            glDeleteBuffers(1, &idVBO);
            idVBO = 0;
        }
        if (idVAO) {
            glDeleteVertexArrays(1, &idVAO);
            idVAO = 0;
        }

        // liberar ShaderProgram
        delete shaderProg;
        shaderProg = nullptr;

    }

    void Renderer::inicializar() {
        glClearColor(colorFondo[0], colorFondo[1], colorFondo[2], 1.0);
        glEnable(GL_DEPTH_TEST);
    }


    void Renderer::redimencionar(int ancho, int alto) {
        glViewport(0, 0, ancho, alto);
        // para sincronizar siempre la cámara con el viewport
        cam.onResize(ancho, alto);
        cam.actualizaPosCamara();
    }

    void Renderer::setColorFondo(const float color[3]) {
        colorFondo[0] = color[0];
        colorFondo[1] = color[1];
        colorFondo[2] = color[2];
        glClearColor(colorFondo[0], colorFondo[1], colorFondo[2],
                     1.0); //esto es necesario si quiero que cambie el color de fondo
    }

    const float *Renderer::getColorFondo() const {
        return colorFondo;
    }

    void Renderer::addMensaje(const std::string &msm) {
        _mensaje.push_back(msm);
    }

    const std::vector<std::string> &Renderer::getMensaje() const {
        return _mensaje;
    }


    //MODELO DEL TRIANGULO, YA NO LO USAMOS
    void PAG::Renderer::creaModelo() {
        /**
         * OPCIONAL CON VBO NO ENTRELAZADOS
         */
        /* GLfloat vertices[] = {-0.5, -0.5, 0,
                               0.5, -0.5, 0,
                               0, 0.5, 0};


         GLfloat colores[] = {
                 1.0, 0, 0.5,
                 1.0, 0.4, 0.7,
                 1.0, 0.7, 0.9
         };

         GLuint indices[] = {0, 1, 2};

         glGenVertexArrays(1, &idVAO);
         glBindVertexArray(idVAO);

         // VBO para posicion
         glGenBuffers(1, &idVBO);
         glBindBuffer(GL_ARRAY_BUFFER, idVBO);
         glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
         glEnableVertexAttribArray(0);

         // VBO para los colores
         GLuint idColorVBO;
         glGenBuffers(1, &idColorVBO);
         glBindBuffer(GL_ARRAY_BUFFER, idColorVBO);
         glBufferData(GL_ARRAY_BUFFER, sizeof(colores), colores, GL_STATIC_DRAW);
         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
         glEnableVertexAttribArray(1);

         // el IBO
         glGenBuffers(1, &idIBO);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
         */

        /**
         * OPCIONAL CON VBO ENTRELAZADOS
         */
        GLfloat vertices[] = {
                // posiciones                    // colores
                -0.5, -0.5, 0, 0, 1.0, 0.5,
                0.5, -0.5, 0, 0.8, 0, 0.7,
                0, 0.5, 0, 0, 1.0, 0.9
        };

        GLuint indices[] = {0, 1, 2};

        glGenVertexArrays(1, &idVAO);
        glBindVertexArray(idVAO);

        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) 0);
        glEnableVertexAttribArray(0);


        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    }


    /**
     * Metodo para inicializar los parametros globales de OpenGL
     */

    void PAG::Renderer::inicializaOpenGL() {
        glClearColor(_colorBorrado[0], _colorBorrado[1], _colorBorrado[2], _colorBorrado[3]);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);

        glDepthFunc(GL_LEQUAL);      // para multipasada con blending

    }


// Método auxiliar para leer ficheros de shader
    std::string Renderer::loadShader(const std::string &filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            addMensaje("No se puede abrir el archivo: " + filename);
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    void Renderer::creaShaderProgram() {
        // por compatibilidad cargamos el shader por defecto "pag08"
        loadShaderProgramFromBase("pag08");
    }


    void Renderer::loadShaderProgramFromBase(const std::string &baseName) {
        auto *nuevo = new ShaderProgram(); //hago uno temporal por si falla algo, no toco el actual

        std::vector<std::string> msgs;
        GLuint nuevoID = nuevo->loadFromBaseName(baseName, msgs);

        for (const auto &m: msgs)
            addMensaje(m);

        // Si falla, no tocamos el programa actual
        if (nuevoID == 0) {
            addMensaje(" Error al cargar shaders '" + baseName + "'. Se mantiene el programa anterior.");
            delete nuevo;
            return;
        }

        //si anda bien, libero el anterior y me quedo con el nuevo
        if (shaderProg) {
            delete shaderProg;
            shaderProg = nullptr;
            idSP = 0;
        }

        shaderProg = nuevo;
        idSP = nuevoID;

        // localizar uniforms y subrutinas
        fetchUniforms();
        fetchSubroutines();


        addMensaje("Programa shader: " + baseName + " (program=" + std::to_string(idSP) + ")");
    }


/**
 * comprobar errores de compilación/linkado
 * @param shader
 * @param type
 */
    void Renderer::checkCompilaError(GLuint shader, std::string type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAMA") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                addMensaje("ERROR de compilacion " + type + ": " + std::string(infoLog));
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                addMensaje("ERROR enlazando PROGRAMA: " + std::string(infoLog));
            }
        }
    }


    void Renderer::fetchUniforms() {
        if (idSP == 0) return;

        uModelLoc = glGetUniformLocation(idSP, "uModel");
        uViewLoc = glGetUniformLocation(idSP, "uView");
        uProjLoc = glGetUniformLocation(idSP, "uProj");

        if (uModelLoc < 0) addMensaje("Aviso: uModel no encontrado");
        if (uViewLoc < 0) addMensaje("Aviso: uView no encontrado");
        if (uProjLoc < 0) addMensaje("Aviso: uProj no encontrado");
    }

    void Renderer::fetchSubroutines() {
        /*
        // Localizar el uniform de subrutinas
        locSubroutine = glGetSubroutineUniformLocation(
                idSP,
                GL_FRAGMENT_SHADER,
                "uMetodoColor"
        );

        if (locSubroutine < 0)
            addMensaje("ADVERTENCIA: No se encontró uMetodoColor.");

        // Subrutina modo alambre
        idxModoAlambre = glGetSubroutineIndex(
                idSP,
                GL_FRAGMENT_SHADER,
                "modoAlambre"
        );

        if (idxModoAlambre == GL_INVALID_INDEX)
            addMensaje("ERROR: No se encontró subrutina modoAlambre.");

        // Subrutina modo sólido
        idxModoSolido = glGetSubroutineIndex(
                idSP,
                GL_FRAGMENT_SHADER,
                "modoSolido"
        );

        if (idxModoSolido == GL_INVALID_INDEX)
            addMensaje("ERROR: No se encontró subrutina modoSolido.");
            */
        if (idSP == 0) return;

        // Buscamos la variable uniforme de subrutina "uMetodoLuz"
        locMetodoLuz = glGetSubroutineUniformLocation(idSP, GL_FRAGMENT_SHADER, "uMetodoLuz");

        if (locMetodoLuz < 0) {
            addMensaje("AVISO: No se encontró la subrutina uniforme 'uMetodoLuz' en el Fragment Shader.");
        }


    }



    void Renderer::setMatrices(const glm::mat4 &view, const glm::mat4 &proj) {
        if (idSP == 0) return;
        glUseProgram(idSP);
        if (uModelLoc >= 0) glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(model));
        if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));
    }


//CARGA DE MODELOS:
    int Renderer::loadOBJModel(const std::string &path, bool smoothNormals) {
        try {
            auto m = std::make_unique<Modelo>();
            m->loadOBJ(path, smoothNormals);

            //Asignacion de texturas. el requisito es que la textura se llame igual que el modelo, entonces no hay error al asignarlas
            if (path.find("vaca") != std::string::npos) {
                m->asignarTextura("texturas/vaca.png");
            }
            else if (path.find("trex") != std::string::npos) {
                m->asignarTextura("texturas/t-rex.png");
            }
            else if (path.find("dado") != std::string::npos) {
                m->asignarTextura("texturas/dado.png");
            }

            addMensaje("Cargado OBJ: " + path + " (" + std::to_string(m->cuentaTriang()) + " triangulos)");
            _modelos.push_back(std::move(m));
            return (int) _modelos.size() - 1;

        } catch (const std::exception &e) {
            addMensaje(std::string("Error cargando OBJ: ") + e.what());
            return -1;
        }
    }

    void Renderer::removeModel(int index) {
        if (index < 0 || index >= (int) _modelos.size()) return; //si el modelo no esta return
        _modelos.erase(_modelos.begin() + index);
        if (_modeloSelec >= (int) _modelos.size()) _modeloSelec = (int) _modelos.size() - 1;
    }

    std::vector<std::string> Renderer::listaNombreModelo() const {
        std::vector<std::string> names;
        names.reserve(_modelos.size());

        for (const auto& m : _modelos) {
            names.push_back(m ? m->nombre() : std::string("Modelo"));
        }
        return names;
    }


    Modelo *Renderer::getModelo(int idx) {
        if (idx < 0 || idx >= (int) _modelos.size()) return nullptr;
        return _modelos[idx].get();
    }

    void Renderer::dibujaModelos() {

        for (auto& m : _modelos) {
            if (!m) continue;

            // Enviar matriz modelo
            if (uModelLoc >= 0)
            glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(m->modelaMatrix()));

            // Enviar Material
            const Material& mat = m->getMaterial();
            glUniform3fv(glGetUniformLocation(idSP, "uKa"), 1, &mat.Ka[0]);
            glUniform3fv(glGetUniformLocation(idSP, "uKd"), 1, &mat.Kd[0]);
            glUniform3fv(glGetUniformLocation(idSP, "uKs"), 1, &mat.Ks[0]);
            glUniform1f (glGetUniformLocation(idSP, "uShininess"), mat.brillo);

            // TEXTURA
            GLint locSampler = glGetUniformLocation(idSP, "muestreador");
            if (locSampler >= 0)
                glUniform1i(locSampler, 0);

            if (m->usaTextura()) {
                m->getTextura().activar(0);
            }

            // Renderizar
            m->dibuja();
        }
    }


    //LUCES
    int Renderer::addLuz(std::unique_ptr<PAG::LightApplicator> strat) {
        _luces.emplace_back(std::move(strat));
        return static_cast<int>(_luces.size()) - 1;
    }

    void Renderer::removeLuz(int index) {
        if (index < 0 || index >= static_cast<int>(_luces.size())) return;
        _luces.erase(_luces.begin() + index);
    }

    void Renderer::refrescar() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (idSP == 0) return;

        glUseProgram(idSP);
        fetchUniforms();
        fetchSubroutines();

        glm::mat4 view = cam.matrizVision();
        glm::mat4 proj = cam.matrizProyeccion();

        if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));


        //primer pasada
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Sumar color luz al color base
        glDepthFunc(GL_LEQUAL);      // Dibujar sobre la misma superficie

        // Seleccionar subrutina LuzAmbiente
        GLuint subAmbiente = glGetSubroutineIndex(idSP, GL_FRAGMENT_SHADER, "LuzAmbiente");
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &subAmbiente);

        // Calcular ambiente total (suma de todas las luces ambiente activas o un valor base)
        glm::vec3 ambienteTotal(0.0);
        bool hayLuces = false;

        for(const auto& L : _luces) {
            if(L.props.activa) {
                ambienteTotal += L.props.Ia;
                hayLuces = true;
            }
        }
        // Si no hay luces, poner un mínimo para que se vea algo (la vaca gris)
        if(!hayLuces) ambienteTotal = glm::vec3(0.2f);

        glUniform3fv(glGetUniformLocation(idSP, "uAmbiente_Ia"), 1, glm::value_ptr(ambienteTotal));

        dibujaModelos(); // Dibujar pasada base

        // Renderizar LUCES ACTIVAS
        glBlendFunc ( GL_SRC_ALPHA, GL_ONE );

        for (const Light& L : _luces) {
            if (!L.props.activa) continue;

            std::string tipo = L.nombreEstrategia();

            if (tipo == "Ambiente") continue;

            L.aplica(idSP, view);

            dibujaModelos(); // Dibujar
        }

    }
}