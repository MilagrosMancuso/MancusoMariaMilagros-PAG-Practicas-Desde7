#version 410 core

in vec3 vNormal;
in vec3 vWorldPos;

out vec4 FragColor;

void main() {
    // Color simple por normal (visualización técnica)
    vec3 N = normalize(vNormal);
    vec3 color = 0.5 * (N + vec3(1.0));
    FragColor = vec4(color, 1.0);
}