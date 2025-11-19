//
// Created by Mili on 19/11/2025.
//

#ifndef PRACTICA1PAG_ESTRATEGIALUZAMBIENTE_H
#define PRACTICA1PAG_ESTRATEGIALUZAMBIENTE_H
#include "EstrategiaLuz.h"

namespace PAG {

    class EstrategiaLuzAmbiente : public EstrategiaLuz {

        public:
            const char* nombre() const override {
                return "Ambiente";
            }

            void aplicaLuz(GLuint program,
                            const PropiedadesLuz& p,
                            const glm::mat4& V) override;
    };

}

#endif //PRACTICA1PAG_ESTRATEGIALUZAMBIENTE_H
