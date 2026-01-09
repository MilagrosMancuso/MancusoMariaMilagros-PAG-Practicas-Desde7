#version 410 core

layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangente;
layout (location = 4) in vec3 bitangente;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uMatrizSombras;

out salidaVS {
    vec2 vTexCoord;
    vec3 vPosVS;
    vec4 vCoordenadasSombra;
    mat3 vTBNinv;// Matriz para pasar vectores de View - Tangent
} salida;

void main() {
    mat4 matrizMV = uView * uModel;
    vec4 posVista = matrizMV * vec4(posicion, 1.0);
    salida.vPosVS = posVista.xyz;
    salida.vTexCoord = texCoord;

    // Coordenadas para muestrear el mapa de sombras
    salida.vCoordenadasSombra = uMatrizSombras * uModel * vec4(posicion, 1.0);

    mat3 normalMatrix = transpose(inverse(mat3(matrizMV)));

    // Construcción de la matriz TBN para espacio tangente
    vec3 T = normalize(normalMatrix * tangente);
    vec3 B = normalize(normalMatrix * bitangente);
    vec3 N = normalize(normalMatrix * normal);

    // Inversa de matriz ortonormal - traspuesta
    salida.vTBNinv = transpose(mat3(T, B, N));

    gl_Position = uProj * posVista;
}