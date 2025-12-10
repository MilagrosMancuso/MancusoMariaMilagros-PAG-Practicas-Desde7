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
        std::unique_ptr<LightApplicator> aplic;

    public:
        LightProperties props;

        Light(std::unique_ptr<LightApplicator> a)
                : aplic(std::move(a)) {}

        Light(Light&&) noexcept = default;
        Light& operator=(Light&&) noexcept = default;

        const char* nombreEstrategia() const {
            return aplic->nombre();
        }

        void setEstrategia(std::unique_ptr<LightApplicator> a) {
            aplic = std::move(a);
        }

        GLuint aplica(GLuint program, const glm::mat4& V) const;
    };
}

#endif //PRACTICA1PAG_LIGHT_H
