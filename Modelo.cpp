//
// Created by Mili on 01/11/2025.
//
#include "Modelo.h"
#include <stdexcept>
#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

namespace PAG {

/**
 * Recibe un aiMesh de Assimp y lo convierte en una Malla
 */
    Malla Modelo::procesaMalla(const aiMesh *mesh) {
        std::vector<Vertice> vertices;
        std::vector<unsigned> indices;
        vertices.reserve(mesh->mNumVertices);

        for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
            Vertice v{};
            v.pos = {
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z
            };

            if (mesh->HasNormals()) {
                v.normal = {
                        mesh->mNormals[i].x,
                        mesh->mNormals[i].y,
                        mesh->mNormals[i].z
                };
            } else {
                v.normal = {0, 0, 1};
            }

            // Texturas
            if(mesh->HasTextureCoords(0)) {
                v.texCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
            } else {
                v.texCoord = { 0.0f, 0.0f };
            }

            // TANGENTES Y BITANGENTES
            if (mesh->HasTangentsAndBitangents()) {
                v.tangente = {
                        mesh->mTangents[i].x,
                        mesh->mTangents[i].y,
                        mesh->mTangents[i].z
                };
                v.bitangente = {
                        mesh->mBitangents[i].x,
                        mesh->mBitangents[i].y,
                        mesh->mBitangents[i].z
                };
            } else {
                // por defecto por si falla el calculo
                v.tangente = {1, 0, 0};
                v.bitangente = {0, 1, 0};
            }

            vertices.push_back(v);
        }

        for (unsigned f = 0; f < mesh->mNumFaces; ++f) {
            const aiFace &face = mesh->mFaces[f];
            for (unsigned k = 0; k < face.mNumIndices; ++k)
                indices.push_back(face.mIndices[k]);
        }

        Malla nueva;
        nueva.construir(vertices, indices);
        return nueva;
    }


/**
 * Carga un OBJ usando Assimp
 */
    void Modelo::loadOBJ(const std::string &path, bool smoothNormals) {
        Assimp::Importer importer;

        // Añadimos aiProcess_CalcTangentSpace para generar tangentes y bitangentes
        unsigned flags = aiProcess_JoinIdenticalVertices
                         | aiProcess_Triangulate
                         | aiProcess_CalcTangentSpace
                         | (smoothNormals ? aiProcess_GenSmoothNormals : aiProcess_GenNormals);

        const aiScene *scene = importer.ReadFile(path, flags);

        if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
            throw std::runtime_error(std::string("Assimp error: ") + importer.GetErrorString());
        }

        _ruta = path;
        _nombre = fs::path(path).filename().string();
        mallas.clear();

        // Recorremos todas las mallas del modelo
        std::vector<const aiNode *> stack{scene->mRootNode};
        while (!stack.empty()) {
            auto *nodo = stack.back();
            stack.pop_back();

            for (unsigned i = 0; i < nodo->mNumMeshes; ++i) {
                const aiMesh *mesh = scene->mMeshes[nodo->mMeshes[i]];
                mallas.push_back(procesaMalla(mesh));
            }

            for (unsigned c = 0; c < nodo->mNumChildren; ++c)
                stack.push_back(nodo->mChildren[c]);
        }
    }


    /**
    Dibuja cada malla del modelo
    */
    void Modelo::dibuja() const
    {
        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        if (usaTextura()) {
            getTextura().activar(0);
        }

       /* if (_mapaNormal) {
            _mapaNormal->activar(1);
        }
*/

        int usarNM_Int = 0; // Valor para el shader
        if (_mapaNormal) {
            _mapaNormal->activar(1);
            // Solo enviamos 1 si tenemos textura Y el flag está activo
            if (_usarNormalMapping) usarNM_Int = 1;
        }

        // ENVIAR EL UNIFORM AL SHADER ACTIVO
        GLint loc = -1;
        // Buscamos el ID del programa actual
        GLint prog = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
        if (prog > 0) {
            loc = glGetUniformLocation(prog, "uUsarNormalMap");
            if (loc >= 0) glUniform1i(loc, usarNM_Int);
        }

        for (const auto& m : mallas)
            m.dibuja();

        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


    /**
 * Cuenta todos los triángulos del modelo
 */
    size_t Modelo::cuentaTriang() const {
        size_t total = 0;
        for (const auto &m: mallas)
            total += m.getCuentaIndices() / 3;
        return total;
    }


    /**
     * SET Para una textura
     */
    void Modelo::asignarTextura(const std::string& ruta)
    {
        _textura = std::make_unique<Textura>();
        _textura->carga(ruta);
        _usarTextura = true;
    }

/**
 * Asignar el mapa de normales a nuestro modelo
 * @param ruta (ruta del mapa de normales)
 */
    void Modelo::asignarMapaNormal(const std::string& ruta) {
        _mapaNormal = std::make_unique<Textura>();
        _mapaNormal->carga(ruta);
    }
}