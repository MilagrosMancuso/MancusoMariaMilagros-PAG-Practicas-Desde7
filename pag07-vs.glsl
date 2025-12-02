/*#version 410 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 vNormalVS;
out vec3 vPosVS;

void main()
{
    mat4 MV = uView * uModel;

    vPosVS    = vec3(MV * vec4(aPos, 1.0));
    vNormalVS = normalize(mat3(MV) * aNormal);

    gl_Position = uProj * vec4(vPosVS, 1.0);
}
*/

#version 410 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 vNormalVS;
out vec3 vPosVS;

void main()
{
    mat4 MV = uView * uModel;

    vPosVS    = vec3(MV * vec4(aPos, 1.0));
    vNormalVS = normalize(mat3(MV) * aNormal);

    gl_Position = uProj * vec4(vPosVS, 1.0);
}
