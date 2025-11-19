//
// Created by Mili on 19/11/2025.
//

#include "EstrategiaLuzAmbiente.h"

namespace PAG {

    void EstrategiaLuzAmbiente::aplicaLuz(
            GLuint program,
            const PropiedadesLuz& p,
            const glm::mat4& V){

        // subrutina
        GLuint sr = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "aplicaAmbiente");
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &sr);

        // Uniform ambiente
        GLint locIa = glGetUniformLocation(program, "uAmbiente_Ia");

        if (locIa >= 0)
            glUniform3fv(locIa, 1, &p.Ia[0]);
    }

}