#version 410 core

in vec3 vPosVS;
in vec3 vNormVS;
in vec2 vTexCoord;

out vec4 fragColor;

// Materiales
uniform vec3 uKa;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

// Luces
uniform vec3 uLight_Id;
uniform vec3 uLight_Is;
uniform vec3 uLight_PosVS;
uniform vec3 uLight_DirVS;

uniform float uSpot_cosGamma; //es necesario?
uniform float uSpot_exp; //es necesario?

// Textura
uniform sampler2D muestreador; // sampler

// Ambiente acumulado
uniform vec3 uAmbiente_Ia;


// SUBRUTINA 1: color base Material o Textura
subroutine vec4 elegirColor();
subroutine uniform elegirColor uMetodoColor;


subroutine(elegirColor)
vec4 ColorTextura() {
return texture(muestreador, vTexCoord);
}

subroutine(elegirColor)
vec4 ColorMaterial() {
return vec4(uKd, 1.0); // Color difuso del material
}

// funcion auxiliar de phong
vec3 Phong(vec3 N, vec3 L, vec3 V, vec3 colorBase) {
// Difusa
float diff = max(dot(L, N), 0.0);
vec3 difusa = uLight_Id * colorBase * diff;

// Especular
vec3 R = reflect(-L, N);
float spec = pow(max(dot(R, V), 0.0), uShininess);
vec3 especular = uLight_Is * uKs * spec;

return difusa + especular;
}

// SUBRUTINA 2: Calculo de Luz
subroutine vec4 calcularColor (vec3 N, vec3 V,vec4 colorDePartida); // esta bien pasar N Y V Para lo de los reflejos
subroutine uniform calcularColor uMetodoLuz;

subroutine (calcularColor)
vec4 luzAmbiente (vec3 N, vec3 V,vec4 colorDePartida){
return vec4(uAmbiente_Ia * colorDePartida.rgb, 1.0);
}

subroutine (calcularColor)
vec4 luzPuntual (vec3 N, vec3 V, vec4 colorDePartida){
vec3 L = normalize(uLight_PosVS - vPosVS);
vec3 rgb = Phong(N, L, V, colorDePartida.rgb);
return vec4(rgb, 1.0);
}

subroutine (calcularColor)
vec4 luzDireccional (vec3 N, vec3 V, vec4 colorDePartida){
vec3 L = normalize(-uLight_DirVS);
vec3 rgb = Phong(N, L, V, colorDePartida.rgb);
return vec4(rgb, 1.0);
}

subroutine (calcularColor)
vec4 luzFoco (vec3 N, vec3 V, vec4 colorDePartida){
vec3 L = normalize(uLight_PosVS - vPosVS);
vec3 D = normalize(uLight_DirVS);

float cosTheta = dot(-L, D);

if (cosTheta < uSpot_cosGamma) {
return vec4(0.0, 0.0, 0.0, 1.0); // Fuera del cono
}

float attenuation = pow(cosTheta, uSpot_exp);
vec3 rgb = Phong(N, L, V, colorDePartida.rgb) * attenuation;

return vec4(rgb, 1.0);
}


void main() {
vec3 N = normalize(vNormVS);
vec3 V = normalize(-vPosVS);

// Obtenemos el color base
vec4 base = uMetodoColor();

// Aplicamos la luz a ese color
vec4 final = uMetodoLuz(N, V, base);

fragColor = final;
}