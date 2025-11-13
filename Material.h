//
// Created by Mili on 10/11/2025.
//

#ifndef PRACTICA1PAG_MATERIAL_H
#define PRACTICA1PAG_MATERIAL_H

#include <glm/glm.hpp>

struct Material {
    glm::vec3 Ka {0.2, 0.2, 0.2};   // ambiente
    glm::vec3 Kd {0.8, 0.8, 0.8};   // difuso
    glm::vec3 Ks {1.0, 1.0, 1.0};   // especular
    float brillo = 32.0;
};

#endif //PRACTICA1PAG_MATERIAL_H
