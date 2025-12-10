#version 410

in vec3 vPosWorld;
in vec3 vNormalWorld;
in vec2 vUV;


out vec4 colorFragmento;

// Material
uniform vec3 uKa;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

// Luz (una por pasada)
uniform vec3 uLightPos;
uniform vec3 uLightId;
uniform vec3 uLightIs;

// Cámara
uniform vec3 uViewPos;

// Textura
uniform sampler2D muestreador;
uniform bool uUsarTextura;

// Ambiente global
uniform vec3 uAmbiente_Ia;

// Subrutinas
//subroutine vec4 modoColor();
//subroutine uniform modoColor uMetodoColor;

// -------------------
// SUBRUTINAS DE LUZ
// -------------------
subroutine vec3 LuzFunc();
subroutine uniform LuzFunc uMetodoLuz;

// ===================
// LUZ AMBIENTE
// ===================
subroutine(LuzFunc)
vec3 LuzAmbiente()
{
vec3 baseColor = uKd;

if (uUsarTextura)
baseColor = texture(muestreador, vUV).rgb;

return baseColor * uAmbiente_Ia;
}

// ===================
// LUZ PUNTUAL
// ===================
subroutine(LuzFunc)
vec3 LuzPuntual()
{
vec3 N = normalize(vNormalWorld);
vec3 L = normalize(uLightPos - vPosWorld);
vec3 V = normalize(uViewPos - vPosWorld);
vec3 R = reflect(-L, N);

vec3 baseColor = uKd;
if (uUsarTextura)
baseColor = texture(muestreador, vUV).rgb;

float diff = max(dot(N, L), 0.0);
float spec = pow(max(dot(R, V), 0.0), uShininess);

vec3 difuso = uLightId * baseColor * diff;
vec3 especular = uLightIs * uKs * spec;

return difuso + especular;
}

void main()
{
colorFragmento = vec4(uMetodoLuz(), 1.0);
}