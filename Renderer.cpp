//
// Created by Mili on 17/09/2025.
//

#include "Renderer.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

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
         GLfloat vertices[] = {-0.5, -0.5, 0,
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


        /**
         * OPCIONAL CON VBO ENTRELAZADOS
         */
        /*
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
*/
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
        loadShaderProgramFromBase("pag09");
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

       /* if (idSP == 0) return;

        // Buscamos la variable uniforme de subrutina "uMetodoLuz"
        locMetodoLuz = glGetSubroutineUniformLocation(idSP, GL_FRAGMENT_SHADER, "uMetodoLuz");

        if (locMetodoLuz < 0) {
            addMensaje("AVISO: No se encontró la subrutina uniforme 'uMetodoLuz' en el Fragment Shader.");
        }
*/
        if (idSP == 0) return;

        //  EL TAMAÑO DEL VECTOR: cuántas subrutinas uniformes tiene el shader ( Luz y Color)
        GLint tam = 0;
        glGetProgramStageiv(idSP, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &tam);
        _numSubrutinasActivas = tam;

        // LOCALIZAR LOS UNIFORMS

        // Selector de Color
        locMetodoColor = glGetSubroutineUniformLocation(idSP, GL_FRAGMENT_SHADER, "uMetodoColor");

        if (locMetodoColor < 0) {
            addMensaje("Error: No se encontró el uniform de subrutina 'uMetodoColor'");
        }

        // Selector de Luz
        locMetodoLuz = glGetSubroutineUniformLocation(idSP, GL_FRAGMENT_SHADER, "uMetodoLuz");

        if (locMetodoLuz < 0) {
            addMensaje("Error: No se encontró el uniform de subrutina 'uMetodoLuz'");
        }

        // LOCALIZAR LAS IMPLEMENTACIONES
        //  Colores
        idxColorMaterial = glGetSubroutineIndex(idSP, GL_FRAGMENT_SHADER, "ColorMaterial");
        idxColorTextura  = glGetSubroutineIndex(idSP, GL_FRAGMENT_SHADER, "ColorTextura");

        //  Luces
        idxLuzAmbiente    = glGetSubroutineIndex(idSP, GL_FRAGMENT_SHADER, "luzAmbiente");
        idxLuzPuntual     = glGetSubroutineIndex(idSP, GL_FRAGMENT_SHADER, "luzPuntual");
        idxLuzDireccional    = glGetSubroutineIndex(idSP, GL_FRAGMENT_SHADER, "luzDireccional");
        idxLuzSpot     = glGetSubroutineIndex(idSP, GL_FRAGMENT_SHADER, "luzFoco");

        if (idxColorTextura == GL_INVALID_INDEX) {
            addMensaje("Error: No se encontró la función 'ColorTextura' en el shader");
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
            try {
                if (path.find("vaca") != std::string::npos) {
                    m->asignarTextura("./texturas/vaca.png");
                }
                else if (path.find("t-rex") != std::string::npos) {
                    // Textura de Color
                    m->asignarTextura("./texturas/t-rex.png");

                    // Mapa de Normales
                    m->asignarMapaNormal("./Normal/t-rex-gris_normal.png");
                }
                else if (path.find("dado") != std::string::npos) {
                    m->asignarTextura("./texturas/dado.png");
                    m->asignarMapaNormal("./Normal/dieHeightMap_normal.png");
                }
            }catch (const std::exception& eTex) {
                addMensaje(std::string("Error cargando textura: ") + eTex.what());
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

    void Renderer::dibujaModelos(GLuint idxSubrutinaLuz) {
        // Vector para la configuración. Debe tener el tamaño de las subrutinas activas
        std::vector<GLuint> configuracion(_numSubrutinasActivas);

        for (auto& m : _modelos) {
            if (!m) continue;

            //  MATRICES Y MATERIALES

            glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(m->modelaMatrix()));

            const Material& mat = m->getMaterial();
            glUniform3fv(glGetUniformLocation(idSP, "uKa"), 1, &mat.Ka[0]);
            glUniform3fv(glGetUniformLocation(idSP, "uKd"), 1, &mat.Kd[0]);
            glUniform3fv(glGetUniformLocation(idSP, "uKs"), 1, &mat.Ks[0]);
            glUniform1f (glGetUniformLocation(idSP, "uShininess"), mat.brillo);

            //  CONFIGURACIÓN DE SUBRUTINAS
            // LUZ: Usamos el que nos pasan por parámetro
            configuracion[locMetodoLuz] = idxSubrutinaLuz;


            // COLOR: Depende de si el modelo tiene textura
            if (m->usaTextura()) {
                    // Si tiene textura, activamos la unidad 0 y elegimos la función de textura
                    m->getTextura().activar(0);
                    configuracion[locMetodoColor] = idxColorTextura;
            } else {
                    // Si no, usamos el color sólido
                    configuracion[locMetodoColor] = idxColorMaterial;
            }


            // ENVIAMOS AL SHADER
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, _numSubrutinasActivas, configuracion.data());

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
      /*  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (idSP == 0) return;

        glUseProgram(idSP);

        // Matrices Globales
        glm::mat4 view = cam.matrizVision();
        glm::mat4 proj = cam.matrizProyeccion();
        if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));

        // Textura: Aseguramos que el sampler use la unidad 0
        glUniform1i(glGetUniformLocation(idSP, "muestreador"), 0);
        // nuevo
        glUniform1i(glGetUniformLocation(idSP, "muestreadorNormal"), 1); // Normales
        glUniform1i(glGetUniformLocation(idSP, "muestreadorSombra"), 2); // Sombras

        // NUEVO --- CÁLCULO DE MATRIZ DE SOMBRAS ---
        // Por ahora, usamos una matriz identidad o una de prueba para que el shader no falle
        glm::mat4 matSombras = glm::mat4(1.0);
        glUniformMatrix4fv(glGetUniformLocation(idSP, "uMatrizSombras"), 1, GL_FALSE, glm::value_ptr(matSombras));


        // PASADA 1 LUZ AMBIENTE (Base)
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthFunc(GL_LEQUAL);

        // Calcular ambiente total
        glm::vec3 ambienteTotal(0.0f);
        bool hayLuces = false;
        for(const auto& L : _luces) {
            if(L.props.activa) {
                ambienteTotal += L.props.Ia;
                hayLuces = true;
            }
        }
        if(!hayLuces) ambienteTotal = glm::vec3(0.2f);

        glUniform3fv(glGetUniformLocation(idSP, "uAmbiente_Ia"), 1, glm::value_ptr(ambienteTotal));

        // DIBUJAR CON SUBRUTINA AMBIENTE
        dibujaModelos(idxLuzAmbiente);

        // PASADA 2 LUCES ACTIVAS
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        for (const Light& L : _luces) {
            if (!L.props.activa) continue;
            // L.aplica envía los uniforms de posición/color y nos devuelve QUÉ tipo de luz es
            GLuint idxLuzActual = L.aplica(idSP, view);

            // DIBUJAR CON LA SUBRUTINA DE ESTA LUZ ESPECÍFICA
            dibujaModelos(idxLuzActual);
        }

        glDisable(GL_BLEND);
        */
        if (idSP == 0) return;

        //  GENERAR MAPA DE SOMBRAS
        glViewport(0, 0, 1024, 1024); // Tamaño del mapa de sombras
        glBindFramebuffer(GL_FRAMEBUFFER, _fboSombras);
        glClear(GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT); // Para evitar el shadow acne

        glm::mat4 lightSpaceMatrix(1.0f);
        if (!_luces.empty()) {
            // Usamos Foco o Direccional
            const auto& L = _luces[0];

            // Calculamos matrices de la cámara virtual de la luz
            glm::mat4 lightView = glm::lookAt(L.props.posicion, L.props.posicion + L.props.direccion, glm::vec3(0,1,0));
            glm::mat4 lightProj = glm::perspective(glm::radians(L.props.aperturaGrados * 2.0f), 1.0f, 0.1f, 50.0f);
            lightSpaceMatrix = lightProj * lightView;

            glUseProgram(_idSPSombras);
            GLint uMVPShadow = glGetUniformLocation(_idSPSombras, "matrizModVisProy");

            for (auto& m : _modelos) {
                if (!m) continue;
                glm::mat4 mvp = lightSpaceMatrix * m->modelaMatrix();
                glUniformMatrix4fv(uMVPShadow, 1, GL_FALSE, glm::value_ptr(mvp));
                m->dibuja();
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glCullFace(GL_BACK);

        // RENDERIZADO FINAL
        int anchoVP, altoVP;
        glfwGetFramebufferSize(glfwGetCurrentContext(), &anchoVP, &altoVP);
        glViewport(0, 0, anchoVP, altoVP); // Restauramos viewport
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LEQUAL);

        glUseProgram(idSP);

        // Matrices Globales
        glm::mat4 view = cam.matrizVision();
        glm::mat4 proj = cam.matrizProyeccion();
        if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));

        // Configuración de Samplers
        glUniform1i(glGetUniformLocation(idSP, "muestreador"), 0);       // Unidad 0: Color
        glUniform1i(glGetUniformLocation(idSP, "muestreadorNormal"), 1); // Unidad 1: Normales
        glUniform1i(glGetUniformLocation(idSP, "muestreadorSombra"), 2); // Unidad 2: Sombras

        // Matriz de sombras escalada
        glm::mat4 B = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f));
        B = glm::scale(B, glm::vec3(0.5f));
        glm::mat4 matSombrasUniform = B * lightSpaceMatrix;
        glUniformMatrix4fv(glGetUniformLocation(idSP, "uMatrizSombras"), 1, GL_FALSE, glm::value_ptr(matSombrasUniform));

        // Activamos la textura de sombras generada
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, _texSombra);

        // PASADA 1 LUZ AMBIENTE (Base)
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glm::vec3 ambienteTotal(0.0f);
        bool hayLucesActivas = false;
        for(const auto& L : _luces) {
            if(L.props.activa) {
                ambienteTotal += L.props.Ia;
                hayLucesActivas = true;
            }
        }
        if(!hayLucesActivas) ambienteTotal = glm::vec3(0.2f);
        glUniform3fv(glGetUniformLocation(idSP, "uAmbiente_Ia"), 1, glm::value_ptr(ambienteTotal));

        dibujaModelos(idxLuzAmbiente);

        // PASADA 2 LUCES ACTIVAS (Iluminación + Sombras)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        for (const Light& L : _luces) {
            if (!L.props.activa) continue;
            GLuint idxLuzActual = L.aplica(idSP, view);
            dibujaModelos(idxLuzActual);
        }

        glDisable(GL_BLEND);
    }



    void Renderer::inicializaShadowMapping() {
        // 1. Crear el FBO
        glGenFramebuffers(1, &_fboSombras);

        // 2. Crear textura de profundidad
        glGenTextures(1, &_texSombra);
        glBindTexture(GL_TEXTURE_2D, _texSombra);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        // Configuración obligatoria para shadow mapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

        // 3. Vincular al FBO
        glBindFramebuffer(GL_FRAMEBUFFER, _fboSombras);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _texSombra, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

}