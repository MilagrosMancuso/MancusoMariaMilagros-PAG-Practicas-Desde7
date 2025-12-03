#version 410 core

layout (location = 0) in vec3 aPos;    // Posición del modelo
layout (location = 1) in vec3 aNorm;   // Normal del modelo

out vec3 vPosVS;    // Posición del vertice en View Space
out vec3 vNormVS;   // Normal del vertice en View Space

uniform mat4 uModel; // Matriz de Modelo
uniform mat4 uView;  // Matriz de Vista (Cámara)
uniform mat4 uProj;  // Matriz de Proyección

void main()
{
    //Transformar posición al Espacio de Vista
    vec4 posViewSpace = uView * uModel * vec4(aPos, 1.0);
    vPosVS = posViewSpace.xyz;

    //Transformar Normal al Espacio de Vista
    //  inversa transpuesta de la matriz
    mat4 normalMatrix = transpose(inverse(uView * uModel));
    vNormVS = normalize(normalMatrix * vec4 ( aNorm, 0.0 ) ).xyz;

    gl_Position = uProj * posViewSpace;
}