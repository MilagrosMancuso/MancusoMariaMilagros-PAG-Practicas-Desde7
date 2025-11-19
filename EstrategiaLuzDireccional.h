//
// Created by Mili on 19/11/2025.
//

#ifndef PRACTICA1PAG_ESTRATEGIALUZDIRECCIONAL_H
#define PRACTICA1PAG_ESTRATEGIALUZDIRECCIONAL_H
#include "EstrategiaLuz.h"

namespace PAG {

    class EstrategiaLuzDireccional : public EstrategiaLuz {
    public:
        const char* nombre() const override {
            return "Direccional";
        }

        void aplicaLuz(GLuint program,
                        const PropiedadesLuz& p,
                        const glm::mat4& V) override;
    };

}
#endif //PRACTICA1PAG_ESTRATEGIALUZDIRECCIONAL_H
