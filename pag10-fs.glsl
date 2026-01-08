#version 410 core

in salidaVS {
    vec2 vTexCoord;
    vec3 vPosVS;
    vec4 vCoordenadasSombra;
    mat3 vTBNinv;
} entrada;

out vec4 fragColor;

uniform sampler2D muestreador;
uniform sampler2D muestreadorNormal;
uniform sampler2DShadow muestreadorSombra;

// Uniform de control para activar/desactivar Normal Mapping
uniform int uUsarNormalMap; // 1 = Activado, 0 = Desactivado

uniform vec3 uKa, uKd, uKs;
uniform float uShininess;
uniform vec3 uLight_Id, uLight_Is, uLight_PosVS, uLight_DirVS;
uniform vec3 uAmbiente_Ia;
uniform float uSpot_cosGamma, uSpot_exp;

subroutine vec4 elegirColor();
subroutine uniform elegirColor uMetodoColor;

subroutine(elegirColor) vec4 ColorTextura() {
return texture(muestreador, entrada.vTexCoord);
}

subroutine(elegirColor) vec4 ColorMaterial() {
return vec4(uKd, 1.0);
}

subroutine vec4 calcularColor(vec3 N, vec3 V, vec4 colorBase);
subroutine uniform calcularColor uMetodoLuz;

vec3 PhongTangente(vec3 N, vec3 L, vec3 V, vec3 colorBase, float s) {
float diff = max(dot(L, N), 0.0);
vec3 difusa = uLight_Id * colorBase * diff;
vec3 R = reflect(-L, N);
float spec = pow(max(dot(R, V), 0.0), uShininess);
vec3 especular = uLight_Is * uKs * spec;
return (difusa + especular) * s;
}

subroutine(calcularColor)
vec4 luzAmbiente(vec3 N, vec3 V, vec4 colorBase) {
return vec4(uAmbiente_Ia * colorBase.rgb, 1.0);
}

subroutine(calcularColor)
vec4 luzPuntual(vec3 N, vec3 V, vec4 colorBase) {
vec3 L_tg = normalize(entrada.vTBNinv * (uLight_PosVS - entrada.vPosVS));
float s = textureProj(muestreadorSombra, entrada.vCoordenadasSombra);
return vec4(PhongTangente(N, L_tg, V, colorBase.rgb, s), 1.0);
}

subroutine(calcularColor)
vec4 luzDireccional(vec3 N, vec3 V, vec4 colorBase) {
vec3 L_tg = normalize(entrada.vTBNinv * (-uLight_DirVS));
float s = textureProj(muestreadorSombra, entrada.vCoordenadasSombra);
return vec4(PhongTangente(N, L_tg, V, colorBase.rgb, s), 1.0);
}

subroutine(calcularColor)
vec4 luzFoco(vec3 N, vec3 V, vec4 colorBase) {
vec3 L = normalize(uLight_PosVS - entrada.vPosVS);
vec3 D = normalize(uLight_DirVS);
float cosTheta = dot(-L, D);
if (cosTheta < uSpot_cosGamma) return vec4(0.0, 0.0, 0.0, 1.0);
vec3 L_tg = normalize(entrada.vTBNinv * L);
float s = textureProj(muestreadorSombra, entrada.vCoordenadasSombra);
float atenuacion = pow(cosTheta, uSpot_exp);
return vec4(PhongTangente(N, L_tg, V, colorBase.rgb, s) * atenuacion, 1.0);
}

void main() {
vec3 N;

// Lógica para activar/desactivar Normal Mapping
if (uUsarNormalMap == 1) {
// Leemos del mapa y transformamos de [0,1] a [-1,1]
vec3 normalMapa = texture(muestreadorNormal, entrada.vTexCoord).rgb;
N = normalize(normalMapa * 2.0 - 1.0);
} else {
// Si está desactivado, usamos la normal por defecto del espacio tangente (Z+)
N = vec3(0.0, 0.0, 1.0);
}

// Vector de visión en espacio tangente
vec3 V = normalize(entrada.vTBNinv * (-entrada.vPosVS));

vec4 base = uMetodoColor();
fragColor = uMetodoLuz(N, V, base);
}