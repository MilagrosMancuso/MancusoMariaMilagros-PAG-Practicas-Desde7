//
// Created by Mili on 01/11/2025.
//
#include "Modelo.h"
#include <stdexcept>
#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

namespace PAG {

    Modelo::Modelo() = default;

    Modelo::~Modelo() {
        releaseGL();
    }

    Modelo::Modelo(PAG::Modelo &&m) noexcept {
        _vao = m._vao;
        m._vao = 0;

        _vbo = m._vbo;
        m._vbo = 0;

        _ibo = m._ibo;
        m._ibo = 0;

        _indexCount = m._indexCount;
        m._indexCount = 0;

        _model = m._model;
        nombre = std::move(m.nombre);
        ruta = std::move(m.ruta);
        wireframe = m.wireframe;
    }

    Modelo &Modelo::operator=(Modelo &&m) noexcept { //vemos si tenemos otro modelo
        if (this != &m) {
            releaseGL();
            _vao = m._vao;
            m._vao = 0;
            _vbo = m._vbo;
            m._vbo = 0;
            _ibo = m._ibo;
            m._ibo = 0;
            _indexCount = m._indexCount;
            m._indexCount = 0;
            _model = m._model;
            nombre = std::move(m.nombre);
            ruta = std::move(m.ruta);
            wireframe = m.wireframe;
        }
        return *this;
    }

    void Modelo::releaseGL() {
        if (_ibo) {
            glDeleteBuffers(1, &_ibo);
            _ibo = 0;

        }

        if (_vbo) {
            glDeleteBuffers(1, &_vbo);
            _vbo = 0;
        }

        if (_vao) {
            glDeleteVertexArrays(1, &_vao);
            _vao = 0;
        }

        _indexCount = 0;
    }

    void Modelo::buildGPU(const std::vector<Vertex> &verts,
                          const std::vector<unsigned> &indices) {
        releaseGL();

        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(Vertex) * verts.size(),
                     verts.data(),
                     GL_STATIC_DRAW);

        glGenBuffers(1, &_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(unsigned) * indices.size(),
                     indices.data(),
                     GL_STATIC_DRAW);


        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(float) * 3));
        glEnableVertexAttribArray(1);

        _indexCount = static_cast<GLsizei>(indices.size());

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }


    void Modelo::gatherMesh(const aiMesh *m,
                            std::vector<Vertex> &outVerts,
                            std::vector<unsigned> &outIdx,
                            unsigned baseVertex) {
        // Extraemos vertices
        for (unsigned i = 0; i < m->mNumVertices; ++i) {
            Vertex v{};
            v.px = m->mVertices[i].x;
            v.py = m->mVertices[i].y;
            v.pz = m->mVertices[i].z;
            if (m->HasNormals()) {
                v.nx = m->mNormals[i].x;
                v.ny = m->mNormals[i].y;
                v.nz = m->mNormals[i].z;
            } else {
                v.nx = v.ny = 0.0f;
                v.nz = 1.0f;
            }
            outVerts.push_back(v);
        }

        for (unsigned f = 0; f < m->mNumFaces; ++f) {
            const aiFace &face = m->mFaces[f];
            for (unsigned k = 0; k < face.mNumIndices; ++k) {
                outIdx.push_back(baseVertex + face.mIndices[k]);
            }
        }
    }


    void Modelo::loadOBJ(const std::string &path, bool smoothNormals) {
        Assimp::Importer importer;

        unsigned flags = aiProcess_JoinIdenticalVertices
                         | aiProcess_Triangulate
                         | (smoothNormals ? aiProcess_GenSmoothNormals : aiProcess_GenNormals);

        const aiScene *scene = importer.ReadFile(path, flags);
        if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
            throw std::runtime_error(std::string("Assimp error: ") + importer.GetErrorString());
        }


        std::vector<Vertex> verts;
        std::vector<unsigned> indices;
        verts.reserve(8192);
        indices.reserve(16384);

        // Recorremos y aplanamos en UN solo VAO/VBO/IBO
        std::vector<const aiNode*> stack{ scene->mRootNode };
        while (!stack.empty()) {
            auto* n = stack.back(); stack.pop_back();
            for (unsigned i = 0; i < n->mNumMeshes; ++i) {
                const aiMesh* m = scene->mMeshes[n->mMeshes[i]];
                unsigned base = static_cast<unsigned>(verts.size());
                gatherMesh(m, verts, indices, base);
            }
            for (unsigned c = 0; c < n->mNumChildren; ++c)
                stack.push_back(n->mChildren[c]);
        }

        if (verts.empty() || indices.empty())
            throw std::runtime_error("El modelo no contiene geometría utilizable.");

        buildGPU(verts, indices);

        ruta = path;
        try {
            nombre = fs::path(path).filename().string();
        } catch (...) {
            nombre = path;
        }

        // Inicial transform: unidad
        _model = glm::mat4(1.0f);
    }


    void Modelo::dibuja() const {
        if (_vao == 0 || _ibo == 0 || _indexCount == 0) return;

        if (wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        glBindVertexArray(_vao);
        glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        if (wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }


    void Modelo::setTranslation(const glm::vec3& t) {
        // M = T * R * S
        _model[3] = glm::vec4(t, 1.0f);
    }


    void Modelo::translate(const glm::vec3& dt) {
        _model = glm::translate(_model, dt);
    }


}