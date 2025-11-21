#version 410 core

layout(location = 0) in vec3 aPos;       // posición
layout(location = 1) in vec3 aNormal;    // normal

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 vPosVS;       // posición en espacio de visión
out vec3 vNormalVS;    // normal en espacio de visión

void main()
{
    // Matriz Model-View
    mat4 MV = uView * uModel;

    // Posición en espacio de visión
    vec4 posVS4 = MV * vec4(aPos, 1.0);
    vPosVS = posVS4.xyz;

    // Normal en espacio de visión (normalMatrix = inversa de la traspuesta de la parte 3x3)
    mat3 normalMatrix = transpose(inverse(mat3(MV)));
    vNormalVS = normalize(normalMatrix * aNormal);

    // Posición final en clip space
    gl_Position = uProj * posVS4;
}