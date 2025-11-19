//
// Created by Mili on 19/11/2025.
//

#include "EstrategiaLuzPuntual.h"

namespace PAG {

    void EstrategiaLuzPuntual::aplicaLuz(
            GLuint program,
            const PropiedadesLuz& p,
            const glm::mat4& V)
    {
        GLuint sr = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "aplicaPuntual");
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &sr);

        // Transformación posición a espacio de visión
        glm::vec3 posVS = glm::vec3(V * glm::vec4(p.posicion, 1.0));

        glUniform3fv(glGetUniformLocation(program, "uLuz_PosVS"), 1, &posVS[0]);
        glUniform3fv(glGetUniformLocation(program, "uLuz_Id"),    1, &p.Id[0]);
        glUniform3fv(glGetUniformLocation(program, "uLuz_Is"),    1, &p.Is[0]);
    }

}