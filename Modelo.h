//
// Created by Mili on 01/11/2025.
// La usamos para gestionar los modelos.
//Cada modelo tendra su propia mat de modelado.
//

#ifndef PRACTICA1PAG_MODELO_H
#define PRACTICA1PAG_MODELO_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <memory>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace PAG{

    class Modelo{
    private:

        struct Vertex {
            float px, py, pz; //pos
            float nx, ny, nz; //normal
        };

        Modelo(const Modelo&) = delete;
        Modelo& operator=(const Modelo&) = delete;
        Modelo(Modelo&&) noexcept;
        Modelo& operator=(Modelo&&) noexcept;

        GLuint _vao = 0;
        GLuint _vbo = 0;
        GLuint _ibo = 0;
        GLsizei _indexCount = 0;

        glm::mat4 _model{1.0f};

        void releaseGL();
        static void gatherMesh(const aiMesh* m,
                               std::vector<Vertex>& outVerts,
                               std::vector<unsigned>& outIdx,
                               unsigned baseVertex);

    public:
            Modelo();
            ~Modelo();


            // Construcción desde datos ya cargados (interno)
            void buildGPU(const std::vector<Vertex>& verts,
                          const std::vector<unsigned>& indices);

            // Carga un OBJ desde path con Assimp y genera VBO/IBO/VAO
            void loadOBJ(const std::string& path,
                         bool smoothNormals = true); // aiProcess_GenSmoothNormals vs GenNormals

            // Dibujo, activa VAO y  glDrawElements
            void dibuja() const;

            // Transforma/ de modelado
            glm::mat4& modelaMatrix() { return _model; }
            const glm::mat4& modelaMatrix() const { return _model; }

            void setTranslation(const glm::vec3& t);
            void translate(const glm::vec3& dt);

            // para los archivos
            std::string nombre;   //archivo base
            std::string ruta;

            bool wireframe = false;

            size_t cuentaTriang() const { return _indexCount / 3; } //para contar cuantos triangulos


    };
}


#endif //PRACTICA1PAG_MODELO_H
