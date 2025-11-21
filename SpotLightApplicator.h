//
// Created by Mili on 21/11/2025.
// esta clase se encarga de seleccionar bien la subrutina para el f. shader
// pasa los uniforms para que la luz sea tipo foco
// convierte la pos y dir de la luz para que sea la de espacio de vision.
//

#ifndef PRACTICA1PAG_SPOTLIGHTAPPLICATOR_H
#define PRACTICA1PAG_SPOTLIGHTAPPLICATOR_H


#include "LightApplicator.h"


namespace PAG {

    class SpotLightApplicator : public LightApplicator {
        public:
            const char* nombre() const override {
                return "Foco";
            }

            void aplicaLuz(GLuint program,
                            const LightProperties& p,
                            const glm::mat4& V) override;
    };

}


#endif //PRACTICA1PAG_SPOTLIGHTAPPLICATOR_H
