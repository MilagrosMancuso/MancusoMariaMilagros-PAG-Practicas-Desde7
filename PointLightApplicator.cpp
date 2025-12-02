//
// Created by Mili on 19/11/2025.
//
#include "PointLightApplicator.h"
#include <glm/gtc/type_ptr.hpp>

namespace PAG {

    void PointLightApplicator::aplicaLuz(GLuint program, const LightProperties& p, const glm::mat4& V)
    {
        // Seleccionar subrutina "LuzPuntual"
        GLuint index = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "LuzPuntual");
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index);

        // Transformar posición a View Space
        glm::vec3 posVS = glm::vec3(V * glm::vec4(p.posicion, 1.0));

        // Enviar uniforms
        glUniform3fv(glGetUniformLocation(program, "uLight_PosVS"), 1, glm::value_ptr(posVS));
        glUniform3fv(glGetUniformLocation(program, "uLight_Id"),    1, glm::value_ptr(p.Id));
        glUniform3fv(glGetUniformLocation(program, "uLight_Is"),    1, glm::value_ptr(p.Is));
    }

    GLuint PointLightApplicator::getSubroutineIndex(GLuint program) const
    {
        return glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "LuzPuntual");
    }
}