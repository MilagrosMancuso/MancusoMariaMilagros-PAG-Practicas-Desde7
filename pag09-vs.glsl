#version 410

layout (location = 0) in vec3 posicion;   // Atributo 0
layout (location = 1) in vec3 normal;     // Atributo 1
layout (location = 2) in vec2 cTextura;   // Atributo 2

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 vPosWorld;
out vec3 vNormalWorld;
out vec2 vUV;

void main ()
{
    vec4 posWorld = uModel * vec4(posicion, 1.0);

    vPosWorld = posWorld.xyz;
    vNormalWorld = mat3(transpose(inverse(uModel))) * normal;
    vUV = cTextura;

    gl_Position = uProj * uView * posWorld;
}