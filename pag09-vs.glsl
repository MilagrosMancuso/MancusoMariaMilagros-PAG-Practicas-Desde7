#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

out vec3 vPosVS;
out vec3 vNormVS;
out vec2 vTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{

    vec4 posViewSpace = uView * uModel * vec4(aPos, 1.0);
    vPosVS = posViewSpace.xyz;

    mat4 normalMatrix = transpose(inverse(uView * uModel));
    vNormVS = normalize(normalMatrix * vec4(aNorm, 0.0)).xyz;

    vTexCoord = aTexCoord;

    gl_Position = uProj * posViewSpace;
}