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

   /* void Renderer::refrescar() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          //el triangulo
           if (idSP == 0 || idVAO == 0 || idIBO == 0) return;
           glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
           glUseProgram(idSP);
           glBindVertexArray(idVAO); //vinculamos el vao en la funcionalidad de refrescar
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
           glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        dibujaModelos();

    }*/

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
        glDepthFunc(GL_LEQUAL);      // para multipasada con blending

        glEnable(GL_MULTISAMPLE);

        glEnable(GL_BLEND);          // activamos blending globalmente
        glBlendEquation(GL_FUNC_ADD);
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

    void Renderer::dibujaModelos(const glm::mat4& view, const glm::mat4& proj) {
        if (idSP == 0) return;

        glUseProgram(idSP);


        if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));


        // uViewPos desde la inversa de la View. lo necesito para saber donde estoy
        glm::mat4 invView = glm::inverse(view);
        glm::vec3 camPos = glm::vec3(invView[3]);

        glUniform3fv(glGetUniformLocation(idSP, "uViewPos"), 1, glm::value_ptr(camPos));

        //esta es la luz, esta fija
        glm::vec3 lightPos(2.0f, 3.0f, 2.0f);
        glUniform3fv(glGetUniformLocation(idSP, "uLightPos"), 1, glm::value_ptr(lightPos));

        // DIBUJAR TODOS LOS MODELOS
        for (auto& m : _modelos) {
            if (!m) continue;

            glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(m->modelaMatrix()));

            // MATERIAL
            const Material& mat = m->getMaterial();

            glUniform3fv(glGetUniformLocation(idSP, "uKa"), 1, &mat.Ka[0]);
            glUniform3fv(glGetUniformLocation(idSP, "uKd"), 1, &mat.Kd[0]);
            glUniform3fv(glGetUniformLocation(idSP, "uKs"), 1, &mat.Ks[0]);
            glUniform1f (glGetUniformLocation(idSP, "uShininess"), mat.brillo);

            // SUBRUTINAS: modo alambre / modo solido
            GLuint modo = m->getWireframe() ? idxModoAlambre : idxModoSolido;
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &modo);

            // DIBUJAR
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

        // Matrices de camara
        const glm::mat4 view = cam.matrizVision();
        const glm::mat4 proj = cam.matrizProyeccion();

        glUseProgram(idSP);

        // Si NO hay luces definidas, dibujamos como antes (de una sola pasada)
        if (_luces.empty()) {
            glm::vec3 defaultLightPos(2.0f, 3.0f, 2.0f);
            GLint loc = glGetUniformLocation(idSP, "uLightPos");
            if (loc >= 0)
                glUniform3fv(loc, 1, glm::value_ptr(defaultLightPos));
//@todo mirar bien
            //glDisable(GL_BLEND);  // sin blending si solo hay una pasada
            dibujaModelos(view, proj);

            glUseProgram(0);
            return;
        }

        // Hay luces hacemos multipasada
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glDepthFunc(GL_LEQUAL);

        bool primera = true;

        for (const auto& luz : _luces) {
            if (!luz.props.activa) continue;

            // Función de mezcla según si es la primera luz o una adicional
            if (primera) {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                primera = false;
            } else {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            }

            // La estrategia de la luz selecciona la subrutina y pasa uniforms específicos
            luz.aplica(idSP, view);

            // Dibujamos los modelos con esa luz activa
            dibujaModelos(view, proj);
        }

        glUseProgram(0);

    }
}