//
// Created by Mili on 19/11/2025.
// Une las Caracteristicas de la luz con el tipo de luz que vamos a usar
//
#include "Light.h"

namespace PAG {

    Light::Light(std::unique_ptr<PAG::LightApplicator> aplica)
            : applicator(std::move(aplica))
        {}


    void Light::setEstrategia(std::unique_ptr<LightApplicator> e) {
        applicator = std::move(e);
    }

    void Light::aplica(GLuint program, const glm::mat4& V) const {
        if (props.activa) {
            applicator->aplicaLuz(program, props, V);
        }
    }

}
