//
// Created by Mili on 10/11/2025.
//

#ifndef PRACTICA1PAG_MALLA_H
#define PRACTICA1PAG_MALLA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <memory>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


struct Vertice {
    glm::vec3 pos;
    glm::vec3 normal;
};

class Malla{
    private:
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ibo = 0;
        GLsizei cuentaIndices = 0;

        void liberarGL();
        void moverDesde(Malla& m);

    public:
        Malla() = default;
        ~Malla();

        //dobles deletes y problemitas que me daba
        Malla(const Malla&) = delete;
        Malla operator= (const Malla&) = delete;

        Malla(Malla&& m) noexcept;
        Malla& operator=(Malla&& m) noexcept;

        /**
        * Construye la malla con datos ya procesados.
        */
        void construir(const std::vector<Vertice>& vertices,
                       const std::vector<unsigned>& indices);

        /**
         * Dibuja malla.
         */
        void dibuja() const;

        /**
         * devuelve cantidad de índices (para contar triángulos)
         */
        GLsizei getCuentaIndices() const { return cuentaIndices; }


};

#endif //PRACTICA1PAG_MALLA_H
