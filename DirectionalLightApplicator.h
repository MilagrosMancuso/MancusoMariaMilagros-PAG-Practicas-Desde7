//
// Created by Mili on 19/11/2025.
//

#ifndef PRACTICA1PAG_DIRECTIONALLIGHTAPPLICATOR_H
#define PRACTICA1PAG_DIRECTIONALLIGHTAPPLICATOR_H
#include "LightApplicator.h"

namespace PAG {

    class DirectionalLightApplicator : public LightApplicator {
    public:
        const char* nombre() const override {
            return "Direccional";
        }

        void aplicaLuz(GLuint program,
                        const LightProperties& p,
                        const glm::mat4& V) override;
    };

}
#endif //PRACTICA1PAG_DIRECTIONALLIGHTAPPLICATOR_H
