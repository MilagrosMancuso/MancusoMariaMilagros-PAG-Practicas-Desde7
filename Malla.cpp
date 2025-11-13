//
// Created by Mili on 10/11/2025.
//
#include "Malla.h"

/**
 * Limpia buffers de GPU
 */
void Malla::liberarGL() {
    if (ibo) glDeleteBuffers(1, &ibo);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (vao) glDeleteVertexArrays(1, &vao);
    vao = vbo = ibo = 0;
    cuentaIndices = 0;
}

Malla::~Malla() {
    liberarGL();
}


Malla::Malla(Malla&& m) noexcept {
    moverDesde(m);
}

Malla& Malla::operator=(Malla&& m) noexcept {
    if (this != &m) {
        liberarGL();
        moverDesde(m);
    }
    return *this;
}

void Malla::moverDesde(Malla& m) {
    vao = m.vao;  m.vao = 0;
    vbo = m.vbo;  m.vbo = 0;
    ibo = m.ibo;  m.ibo = 0;
    cuentaIndices = m.cuentaIndices; m.cuentaIndices = 0;
}


/**
 * Construye buffers GPU para esta malla
 */
void Malla::construir(const std::vector<Vertice>& vertices,
                      const std::vector<unsigned>& indices)
{
    cuentaIndices = (GLsizei)indices.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBO: posiciones + normales entrelazadas
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(Vertice),
                 vertices.data(),
                 GL_STATIC_DRAW);

    // IBO: índices
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned),
                 indices.data(),
                 GL_STATIC_DRAW);

    // Atributo 0 → posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertice),
                          (void*)offsetof(Vertice, pos));
    glEnableVertexAttribArray(0);

    // Atributo 1 → normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertice),
                          (void*)offsetof(Vertice, normal));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}


/**
 * Dibuja la malla
 */
void Malla::dibuja() const {
    if (!vao) return;

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, cuentaIndices, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
