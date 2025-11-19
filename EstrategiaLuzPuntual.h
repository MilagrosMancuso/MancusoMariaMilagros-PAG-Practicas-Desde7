//
// Created by Mili on 19/11/2025.
//

#ifndef PRACTICA1PAG_ESTRATEGIALUZPUNTUAL_H
#define PRACTICA1PAG_ESTRATEGIALUZPUNTUAL_H

#include "EstrategiaLuz.h"

namespace PAG {

    class EstrategiaLuzPuntual : public EstrategiaLuz {
    public:
        const char* nombre() const override {
            return "Puntual";
        }

        void aplicaLuz(GLuint program,
                        const PropiedadesLuz& p,
                        const glm::mat4& V) override;
    };

}

#endif //PRACTICA1PAG_ESTRATEGIALUZPUNTUAL_H
