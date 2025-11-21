//
// Created by Mili on 17/09/2025.
//
#include <glad/glad.h>
#include <vector>
#include <string>
#include "ShaderProgram.h"
#include "Camara.h"
#include <glm/glm.hpp>

#include <memory>
#include "Modelo.h"
#include "Light.h"

#ifndef PRACTICA1PAG_RENDERER_H
#define PRACTICA1PAG_RENDERER_H
namespace PAG {
    class Renderer {
    private:
        static Renderer* instancia;

        float colorFondo[3] = {0.1, 0.1, 0.1};
        float _colorBorrado[4] = {1.0, 1.0, 1.0, 1.0};

        std::vector<std::string> _mensaje; //buffer para guardar los mensajes que quiero mostrar

        ShaderProgram* shaderProg = nullptr;


        GLuint idSP = 0; // Identificador del shader program
        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBO = 0; // Identificador del vertex buffer object
        GLuint idIBO = 0; // Identificador del index buffer object

        // los uniforms para las matrices
        GLint uModelLoc = -1;
        GLint uViewLoc = -1;
        GLint uProjLoc = -1;
        glm::mat4 model = glm::mat4(1);

        Camara cam; //para tenerla en el renderer

        void fetchUniforms();
        Renderer();

        // función auxiliar para leer shaders desde archivo
        std::string loadShader(const std::string& filename);
        void checkCompilaError(GLuint shader, std::string type);

        // Modelos
        std::vector<std::unique_ptr<Modelo>> _modelos;
        int _modeloSelec = -1; // seleccionado en GUI

        // Luces
        std::vector<Light> _luces;

        GLuint idxModoAlambre = 0;
        GLuint idxModoSolido  = 0;
        GLint locSubroutine   = -1;

    public:
        static Renderer& getInstancia();
        virtual ~Renderer();

        void refrescar();
        void inicializar();
        void redimencionar(int ancho, int alto);

        // colores
        void setColorFondo(const float color[3]);
        const float* getColorFondo() const;


        // mensajes
        void addMensaje(const std::string& msm);
        const std::vector<std::string>& getMensaje() const;

        //Shader
        void creaShaderProgram();
         void loadShaderProgramFromBase(const std::string &baseName);

        //Crear la geometria basica
        void creaModelo();

        //Inicializacion de OpenGL
        void inicializaOpenGL();

        //set de las matrices de vista/proyeccion en el shader
        void setMatrices(const glm::mat4& view, const glm::mat4& proj);

        Camara& getCamara() { return cam; }

        // Gestión de modelos
        int loadOBJModel(const std::string& path, bool smoothNormals = true); // devuelve índice
        void removeModel(int index);

        std::vector<std::string> listaNombreModelo() const;

        int  getModeloSeleccionado() const {
                return _modeloSelec;
        }

        void setModeloSeleccionado(int idx) {
                _modeloSelec = idx;
        }

        Modelo* getModelo(int idx);

        int modeloCount() const { //cuantos modelos tengo
                return (int)_modelos.size();
        }

        // Gestion de las luces
        std::vector<Light>& getLuces()             { return _luces; }
        const std::vector<Light>& getLuces() const { return _luces; }

        // Crea una luz con una estrategia concreta. Devuelve índice.
        int addLuz(std::unique_ptr<LightApplicator> strat);

        // Elimina una luz por índice
        void removeLuz(int index);

        // enviar uModel (por modelo) y dibujar todos
        void dibujaModelos(const glm::mat4& view, const glm::mat4& proj);

        void fetchSubroutines();
    };
}

#endif //PRACTICA1PAG_RENDERER_H