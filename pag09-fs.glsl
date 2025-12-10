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

// Ambiente global
uniform vec3 uAmbiente_Ia;

// Subrutinas
subroutine vec4 modoColor();
subroutine uniform modoColor uMetodoColor;

// SIN TEXTURA
subroutine(modoColor)
vec4 usarMaterial()
{
vec3 N = normalize(vNormalWorld);
vec3 L = normalize(uLightPos - vPosWorld);
vec3 V = normalize(uViewPos - vPosWorld);
vec3 R = reflect(-L, N);

vec3 ambient = uKa * uAmbiente_Ia;
vec3 diffuse = uKd * max(dot(N, L), 0.0);
vec3 specular = uKs * pow(max(dot(R, V), 0.0), uShininess);

return vec4(ambient + diffuse + specular, 1.0);
}

// CON TEXTURA
subroutine(modoColor)
vec4 usarTextura()
{
vec3 colorTex = texture(muestreador, vUV).rgb;

vec3 N = normalize(vNormalWorld);
vec3 L = normalize(uLightPos - vPosWorld);
vec3 V = normalize(uViewPos - vPosWorld);
vec3 R = reflect(-L, N);

vec3 ambient = colorTex * uAmbiente_Ia;
vec3 diffuse = colorTex * max(dot(N, L), 0.0);
vec3 specular = uKs * pow(max(dot(R, V), 0.0), uShininess);

return vec4(ambient + diffuse + specular, 1.0);
}

void main ()
{
colorFragmento = uMetodoColor();
}