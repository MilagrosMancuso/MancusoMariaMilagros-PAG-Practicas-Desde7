//
// Created by Mili on 19/11/2025.
// es la Pieza central del patron estrategia, nos permite cambiar el tipo de luz sin perder informacion
// Interfaz
//

#ifndef PRACTICA1PAG_LIGHTAPPLICATOR_H
#define PRACTICA1PAG_LIGHTAPPLICATOR_H

#include <glad/glad.h>
#include "LightProperties.h"
#include <glm/glm.hpp>

namespace PAG {

    class LightApplicator {

        public:
            virtual ~LightApplicator() = default;

            // Selecciona la subrutina GLSL y pasa los uniforms
            virtual void aplicaLuz(GLuint program,
                                    const LightProperties& props,
                                    const glm::mat4& V) = 0; // matriz de vision

            // Nombre para GUI
            virtual const char* nombre() const = 0;
        };

}


#endif //PRACTICA1PAG_LIGHTAPPLICATOR_H
