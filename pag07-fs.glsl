#version 410 core

in vec3 vNormal;
in vec3 vWorldPos;

out vec4 colorFragmento;

//  MATERIAL
uniform vec3 uKa;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;


subroutine vec4 fModoColor();
subroutine uniform fModoColor uMetodoColor;

// MODO 1: ALAMBRE (color fijo)
subroutine(fModoColor)
vec4 modoAlambre() {
return vec4(1, 0, 0, 1); // rojo
}

// MODO 2: SOLIDO /usar material Kd
subroutine(fModoColor)
vec4 modoSolido() {
return vec4(uKd, 1.0);
}

void main() {
colorFragmento = uMetodoColor();
}
