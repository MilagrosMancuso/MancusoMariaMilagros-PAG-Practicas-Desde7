//
// Created by Mili on 21/11/2025.
//

#include "SpotLightApplicator.h"

namespace PAG {

    void SpotLightApplicator ::aplicaLuz(GLuint program, const PAG::LightProperties &p, const glm::mat4 &V) {

        //elijo que tipo de luz usamos en esta pasada
        GLuint sr = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "AplicarSpot");
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &sr);

        //todos los calculos hay que hacerlos en el espacio de vision
        glm::vec3 posVS = glm::vec3(V * glm::vec4(p.posicion, 1.0));
        glm::vec3 dirVS = glm::normalize(glm::mat3(V) * p.direccion);

        glUniform3fv(glGetUniformLocation(program, "uLight_PosVS"), 1, &posVS[0]);
        glUniform3fv(glGetUniformLocation(program, "uLight_DirVS"), 1, &dirVS[0]);
        glUniform3fv(glGetUniformLocation(program, "uLight_Id"),    1, &p.Id[0]);
        glUniform3fv(glGetUniformLocation(program, "uLight_Is"),    1, &p.Is[0]);

        //parametros del cono de luz
        float cosGamma = cos(glm::radians(p.aperturaGrados));
        glUniform1f(glGetUniformLocation(program, "uSpot_cosGamma"), cosGamma); //Es el coseno del angulo interior del foco.
        glUniform1f(glGetUniformLocation(program, "uSpot_exp"),      p.spotExp); //con esto suavizamos los bordes
    }

}