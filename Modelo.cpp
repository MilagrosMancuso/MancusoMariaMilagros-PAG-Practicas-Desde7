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

            vertices.push_back(v);
        }

        // Triángulos
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

        unsigned flags = aiProcess_JoinIdenticalVertices
                         | aiProcess_Triangulate
                         | (smoothNormals ? aiProcess_GenSmoothNormals : aiProcess_GenNormals);

        const aiScene *scene = importer.ReadFile(path, flags);

        if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
            throw std::runtime_error(std::string("Assimp error: ") + importer.GetErrorString());
        }

        _ruta = path;
        _nombre = fs::path(path).filename().string();

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
}