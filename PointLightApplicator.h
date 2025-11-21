//
// Created by Mili on 19/11/2025.
//

#ifndef PRACTICA1PAG_POINTLIGHTAPPLICATOR_H
#define PRACTICA1PAG_POINTLIGHTAPPLICATOR_H

#include "LightApplicator.h"

namespace PAG {

    class PointLightApplicator : public LightApplicator {
    public:
        const char* nombre() const override {
            return "Puntual";
        }

        void aplicaLuz(GLuint program,
                        const LightProperties& p,
                        const glm::mat4& V) override;
    };

}

#endif //PRACTICA1PAG_POINTLIGHTAPPLICATOR_H
