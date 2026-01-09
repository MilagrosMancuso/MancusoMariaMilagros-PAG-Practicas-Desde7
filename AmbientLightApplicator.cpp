//
// Created by Mili on 19/11/2025.
//

#include "AmbientLightApplicator.h"
#include <glm/gtc/type_ptr.hpp>

namespace PAG {

    void AmbientLightApplicator::aplicaLuz(
            GLuint program,
            const LightProperties& p,
            const glm::mat4& V)
    {
        GLuint index = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "luzAmbiente");
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index);

        // Enviar el color ambiente
        glUniform3fv(glGetUniformLocation(program, "uAmbiente_Ia"), 1, glm::value_ptr(p.Ia));

    }


    GLuint AmbientLightApplicator::getSubroutineIndex(GLuint program) const
    {
        return glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "luzAmbiente");
    }

}