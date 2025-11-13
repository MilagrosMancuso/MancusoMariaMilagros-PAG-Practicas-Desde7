//
// Created by Mili on 01/11/2025.
// La usamos para gestionar los modelos.
//Cada modelo tendra su propia mat de modelado.
//

#ifndef PRACTICA1PAG_MODELO_H
#define PRACTICA1PAG_MODELO_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Malla.h"
#include "Material.h"


namespace PAG{

    class Modelo{
    private:

        std::vector<Malla> mallas;
        glm::mat4 modelo {1.0};
        bool wireframe = false;

        std::string _ruta;
        std::string _nombre;

        // Procesa una malla de Assimp y devuelve una Malla lista para la GPU
        Malla procesaMalla(const aiMesh* m);

        Material material;

    public:
        Modelo() = default;
        ~Modelo() = default;

        Modelo(const Modelo&) = delete;
        Modelo& operator=(const Modelo&) = delete;

        Modelo(Modelo&&) noexcept = default;
        Modelo& operator=(Modelo&&) noexcept = default;

        /**
        * Carga un archivo OBJ
        */
        void loadOBJ(const std::string& path, bool smoothNormals);

        /**
         * Dibujar este modelo (todas sus mallas)
         */
        void dibuja() const;

        // Matriz de modelado
        glm::mat4& modelaMatrix() { return modelo; }
        const glm::mat4& modelaMatrix() const { return modelo; }

        size_t cuentaTriang() const;

        // para el archivo
        const std::string& nombre() const { return _nombre; }
        const std::string& ruta()   const { return _ruta;   }
        void setNombre(const std::string& n) { _nombre = n; }
        void setRuta(const std::string& r)   { _ruta = r;   }

        bool getWireframe() const { return wireframe; }
        void setWireframe(bool w) { wireframe = w; }

        Material& getMaterial() { return material; }
        const Material& getMaterial() const { return material; }


    };
}


#endif //PRACTICA1PAG_MODELO_H
