//
// Created by Mili on 27/11/2025.
//
#include "Light.h"
#include <glm/gtc/type_ptr.hpp>

namespace PAG {

    void Light::aplica(GLuint program, const glm::mat4& V) const
    {

        // Transformación a espacio de vista
        glm::vec3 posVS = glm::vec3(V * glm::vec4(props.posicion, 1.0));
        glm::vec3 dirVS = glm::normalize(glm::vec3(V * glm::vec4(props.direccion, 0.0)));


        //  uniforms
        glUniform3fv(glGetUniformLocation(program, "uAmbiente_Ia"),
                     1, glm::value_ptr(props.Ia));

        glUniform3fv(glGetUniformLocation(program, "uLight_Id"),
                     1, glm::value_ptr(props.Id));

        glUniform3fv(glGetUniformLocation(program, "uLight_Is"),
                     1, glm::value_ptr(props.Is));

        glUniform3fv(glGetUniformLocation(program, "uLight_PosVS"),
                     1, glm::value_ptr(posVS));

        glUniform3fv(glGetUniformLocation(program, "uLight_DirVS"),
                     1, glm::value_ptr(dirVS));

        // Foco
        float cosGamma = glm::cos(glm::radians(props.aperturaGrados));
        glUniform1f(glGetUniformLocation(program, "uSpot_cosGamma"), cosGamma);

        glUniform1f(glGetUniformLocation(program, "uSpot_exp"), props.spotExp);


        GLuint idx = aplic->getSubroutineIndex(program);

        // Carga de subrutina al pipeline ( 1 por pasada)
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &idx);
    }

}
