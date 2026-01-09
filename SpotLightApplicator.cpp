//
// Created by Mili on 21/11/2025.
//
#include "SpotLightApplicator.h"
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

namespace PAG {

    void SpotLightApplicator::aplicaLuz(GLuint program, const LightProperties& p, const glm::mat4& V)
    {
        GLuint index = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "luzFoco");
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index);

        glm::vec3 posVS = glm::vec3(V * glm::vec4(p.posicion, 1.0));
        glm::vec3 dirVS = glm::normalize(glm::vec3(V * glm::vec4(p.direccion, 0.0)));

        // Coseno del ángulo
        float cosGamma = std::cos(glm::radians(p.aperturaGrados));

        glUniform3fv(glGetUniformLocation(program, "uLight_PosVS"), 1, glm::value_ptr(posVS));
        glUniform3fv(glGetUniformLocation(program, "uLight_DirVS"), 1, glm::value_ptr(dirVS));
        glUniform3fv(glGetUniformLocation(program, "uLight_Id"),    1, glm::value_ptr(p.Id));
        glUniform3fv(glGetUniformLocation(program, "uLight_Is"),    1, glm::value_ptr(p.Is));
        glUniform1f (glGetUniformLocation(program, "uSpot_cosGamma"), cosGamma);
        glUniform1f (glGetUniformLocation(program, "uSpot_exp"),      p.spotExp);
    }

    GLuint SpotLightApplicator::getSubroutineIndex(GLuint program) const
    {
        return glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "luzFoco");
    }
}