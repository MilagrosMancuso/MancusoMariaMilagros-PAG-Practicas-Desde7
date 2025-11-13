#version 410 core

in vec3 vNormal;
in vec3 vWorldPos;

out vec4 colorFragmento;

// MATERIAL
uniform vec3 uKa;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

// ILUMINACION
uniform vec3 uLightPos;
uniform vec3 uViewPos;

// ---- SUBRUTINAS ----
subroutine vec4 fModoColor();
subroutine uniform fModoColor uMetodoColor;

// MODO 1: ALAMBRE
subroutine(fModoColor)
vec4 modoAlambre() {
return vec4(1, 0, 0, 1);  // rojo
}

// MODO 2: SOLIDO CON ILUMINACION PHONG
subroutine(fModoColor)
vec4 modoSolido() {

vec3 N = normalize(vNormal);
vec3 L = normalize(uLightPos - vWorldPos);
vec3 V = normalize(uViewPos - vWorldPos);
vec3 R = reflect(-L, N);

// Ambient
vec3 ambient = uKa;

// Difuso
float diff = max(dot(N, L), 0.0);
vec3 diffuse = uKd * diff;

// Especular
float spec = pow(max(dot(R, V), 0.0), uShininess);
vec3 specular = uKs * spec;

vec3 finalColor = ambient + diffuse + specular;

return vec4(finalColor, 1.0);
}

void main() {
colorFragmento = uMetodoColor();
}

