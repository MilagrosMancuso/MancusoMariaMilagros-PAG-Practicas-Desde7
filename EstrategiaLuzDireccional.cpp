//
// Created by Mili on 19/11/2025.
//

#include "EstrategiaLuzDireccional.h"

namespace PAG {

    void EstrategiaLuzDireccional::aplicaLuz(
            GLuint program,
            const PropiedadesLuz& p,
            const glm::mat4& V)
    {
        GLuint sr = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "aplicaDireccional");
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &sr);

        glm::vec3 dVS = glm::normalize(glm::mat3(V) * p.direccion);

        glUniform3fv(glGetUniformLocation(program, "uLuz_DirVS"), 1, &dVS[0]);
        glUniform3fv(glGetUniformLocation(program, "uLuz_Id"),    1, &p.Id[0]);
        glUniform3fv(glGetUniformLocation(program, "uLuz_Is"),    1, &p.Is[0]);
    }

}