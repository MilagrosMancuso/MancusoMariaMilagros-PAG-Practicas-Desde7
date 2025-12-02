//
// Created by Mili on 19/11/2025.
//
#include "DirectionalLightApplicator.h"
#include <glm/gtc/type_ptr.hpp>

namespace PAG {

    void DirectionalLightApplicator::aplicaLuz(GLuint program, const LightProperties& p, const glm::mat4& V)
    {
        GLuint index = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "LuzDireccional");
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index);

        // Transformar dirección (vector, w=0) a View Space
        glm::vec3 dirVS = glm::normalize(glm::vec3(V * glm::vec4(p.direccion, 0.0)));

        glUniform3fv(glGetUniformLocation(program, "uLight_DirVS"), 1, glm::value_ptr(dirVS));
        glUniform3fv(glGetUniformLocation(program, "uLight_Id"),    1, glm::value_ptr(p.Id));
        glUniform3fv(glGetUniformLocation(program, "uLight_Is"),    1, glm::value_ptr(p.Is));
    }

    GLuint DirectionalLightApplicator::getSubroutineIndex(GLuint program) const
    {
        return glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "LuzDireccional");
    }
}