//
// Created by Mili on 06/12/2025.
//

#ifndef PRACTICA1PAG_TEXTURA_H
#define PRACTICA1PAG_TEXTURA_H
#include <glad/glad.h>
#include <string>
#include <vector>

#include "lodepng.h"

namespace PAG {

    class Textura {
    private:
        GLuint _idTextura = 0;
        unsigned _ancho = 0;
        unsigned _alto = 0;

        std::vector<unsigned char> _imagen;


    public:
        Textura() = default;
        ~Textura();

        void carga(const std::string& fichero);
        void activar(GLuint unidad = 0) const;

        GLuint id() const { return _idTextura; }
    };

}
#endif //PRACTICA1PAG_TEXTURA_H
