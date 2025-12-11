//
// Created by Mili on 06/12/2025.
//
#include "Textura.h"
#include "lodepng.h"
#include <iostream>

using namespace PAG;

Textura::~Textura()
{
    if (_idTextura != 0) {
        glDeleteTextures(1, &_idTextura);
        _idTextura = 0;
    }
}

void Textura::carga(const std::string& fichero) {
    unsigned error = lodepng::decode(_imagen, _ancho, _alto, fichero);
    if (error) {
        throw std::runtime_error("No se pudo cargar la textura: " + fichero);
    }

    // La textura se carga del revés, así que vamos a darle la vuelta
    unsigned char *imgPtr = &_imagen[0];
    int numeroDeComponentesDeColor = 4;
    int incrementoAncho = _ancho * numeroDeComponentesDeColor; // Ancho en bytes
    unsigned char* top = nullptr;
    unsigned char* bot = nullptr;
    unsigned char temp = 0;

    for (int i = 0; i < _alto / 2; i++){
        top = imgPtr + i * incrementoAncho;
        bot = imgPtr + (_alto - i - 1) * incrementoAncho;
        for (int j = 0; j < incrementoAncho; j++)
        { temp = *top;
            *top = *bot;
            *bot = temp;
            ++top;
            ++bot;
        }
    }

    glGenTextures(1, &_idTextura);
    glBindTexture(GL_TEXTURE_2D, _idTextura);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Cómo resolver la minificación.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);// Cómo resolver la magnificación.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// Cómo pasar de coordenadas de textura a coordenadas en el espacio de la textura en horizontal
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// Cómo pasar de coordenadas de textura a coordenadas en el espacio de la textura en vertical

    // Transferimos la información de la imagen.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 _ancho, _alto, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE,  _imagen.data () );

    // Generar mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

}


void Textura::activar(GLuint unidad) const
{
    glActiveTexture(GL_TEXTURE0 + unidad);
    glBindTexture(GL_TEXTURE_2D, _idTextura);
}