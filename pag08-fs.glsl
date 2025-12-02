#version 410 core

in vec3 vPosVS;
in vec3 vNormVS;

out vec4 fragColor;

uniform vec3 uKa;      // K Ambiente
uniform vec3 uKd;      // K Difuso
uniform vec3 uKs;      // K Especular
uniform float uShininess;

// Luces
uniform vec3 uAmbiente_Ia; // Color Ambiente de la luz
uniform vec3 uLight_Id;    // Color Difuso
uniform vec3 uLight_Is;    // Color Especular

uniform vec3 uLight_PosVS; // Posición Luz
uniform vec3 uLight_DirVS; // Dirección Luz

// Parámetros Foco
uniform float uSpot_cosGamma; // Coseno del ángulo de apertura
uniform float uSpot_exp;      // Exponente de atenuación

//Subrutinas
subroutine vec3 LightCalculation(vec3 N, vec3 V);
subroutine uniform LightCalculation uMetodoLuz;

//Función Phong
vec3 Phong(vec3 N, vec3 L, vec3 V) {
    float diff = max(dot(N, L), 0.0);
    vec3 difusa = uLight_Id * uKd * diff;

    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(V, R), 0.0), uShininess);
    vec3 especular = uLight_Is * uKs * spec;

    return difusa + especular;
}

//  LUZ AMBIENTE
subroutine(LightCalculation)
    vec3 LuzAmbiente(vec3 N, vec3 V) {
    return uAmbiente_Ia * uKa;
}

//  LUZ PUNTUAL
subroutine(LightCalculation)
vec3 LuzPuntual(vec3 N, vec3 V) {
    vec3 L = normalize(uLight_PosVS - vPosVS);
    return Phong(N, L, V);
}

//  LUZ DIRECCIONAL
subroutine(LightCalculation)
vec3 LuzDireccional(vec3 N, vec3 V) {
    vec3 L = normalize(-uLight_DirVS);
    return Phong(N, L, V);
}

//  LUZ FOCO (SPOT)
subroutine(LightCalculation)
vec3 LuzSpot(vec3 N, vec3 V) {
vec3 L = normalize(uLight_PosVS - vPosVS);
vec3 D = normalize(uLight_DirVS);

float cosTheta = dot(-L, D);

if(cosTheta > uSpot_cosGamma) {
    float spotFactor = pow(cosTheta, uSpot_exp);
    return Phong(N, L, V) * spotFactor;
}
    else {
        return vec3(0.0);
    }
}

void main() {
    vec3 N = normalize(vNormVS);
    vec3 V = normalize(-vPosVS);

    vec3 colorFinal = uMetodoLuz(N, V);

    fragColor = vec4(colorFinal, 1.0);
}