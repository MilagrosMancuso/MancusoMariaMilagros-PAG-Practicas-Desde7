//
// Created by Mili on 19/11/2025.
// Clase para guardar los datos de la luz.
//

#ifndef PRACTICA1PAG_PROPIEDADESLUZ_H
#define PRACTICA1PAG_PROPIEDADESLUZ_H

#include <glm/glm.hpp>

namespace PAG {

    class PropiedadesLuz {

        public:
            // Estado general
            bool activa = true;

            // Ambiente
            glm::vec3 Ia {0.2, 0.2, 0.2};

            // Difusa y especular
            glm::vec3 Id {1.0, 1.0, 1.0};
            glm::vec3 Is {1.0, 1.0, 1.0};

            // Posición
            glm::vec3 posicion {2.0, 2.0, 2.0};

            // Dirección
            glm::vec3 direccion {-1.0, -1.0, -1.0};

            // Foco
            float aperturaGrados = 25.0;
            float spotExp = 10.0;
    };

}


#endif //PRACTICA1PAG_PROPIEDADESLUZ_H
