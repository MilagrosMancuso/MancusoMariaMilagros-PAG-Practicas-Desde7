//
// Created by Mili on 19/11/2025.
//

#ifndef PRACTICA1PAG_LIGHT_H
#define PRACTICA1PAG_LIGHT_H

#include "LightProperties.h"
#include "LightApplicator.h"
#include <memory>

namespace PAG {
    class Light {
    private:
        //@todo ojo, no estoy segura de que esto funcione bien
        //con esto puedo elegir uno de los tipos de luz
        std::unique_ptr<LightApplicator> applicator;

        LightProperties props;

    public:
        Light(std::unique_ptr<LightApplicator> aplica);

        Light(Light&&) noexcept = default;
        Light& operator=(Light&&) noexcept = default;


        LightApplicator* estrategia() {
            return applicator.get();
        }

        const LightApplicator* estrategia() const {
            return applicator.get();
        }

        const char* nombreEstrategia() const {
            return applicator->nombre();
        }

        void setEstrategia(std::unique_ptr<LightApplicator> e);

        void aplica(GLuint program, const glm::mat4& V) const;
    };
}

#endif //PRACTICA1PAG_LIGHT_H
