#version 410 core

in vec3 vPosVS;
in vec3 vNormalVS;

out vec4 FragColor;

// Material
uniform vec3  uKa;
uniform vec3  uKd;
uniform vec3  uKs;
uniform float uShininess;

// Luz ambiente
uniform vec3 uAmbient_Ia;

// Luz puntual / direccional / foco
uniform vec3 uLight_Id;
uniform vec3 uLight_Is;
uniform vec3 uLight_PosVS;   // para puntual/foco
uniform vec3 uLight_DirVS;   // para direccional/foco

// Foco
uniform float uSpot_cosGamma;
uniform float uSpot_exp;

// iluminación
subroutine vec4 LightFunc();
subroutine uniform LightFunc uLightFunc;


// Luz Ambiente
subroutine(LightFunc)
vec4 applyAmbient()
{
vec3 color = uKa * uAmbient_Ia;
return vec4(color, 1.0);
}


// luz Puntual
subroutine(LightFunc)
vec4 applyPoint()
{
vec3 N = normalize(vNormalVS);
vec3 L = normalize(uLight_PosVS - vPosVS);  // desde punto a la luz
float NdotL = max(dot(N, L), 0.0);

// Difusa
vec3 diffuse = uKd * uLight_Id * NdotL;

// Especular (observador en origen de VS → V = -posVS)
vec3 V = normalize(-vPosVS);
vec3 R = reflect(-L, N);
float RdotV = max(dot(R, V), 0.0);
vec3 specular = vec3(0.0);
if (NdotL > 0.0)
specular = uKs * uLight_Is * pow(RdotV, uShininess);

vec3 color = diffuse + specular;
return vec4(color, 1.0);
}


// Luz Direccional
subroutine(LightFunc)
vec4 applyDirectional()
{
vec3 N = normalize(vNormalVS);

// d se define desde la luz hacia la escena
// Vector hacia la luz = -d
vec3 L = normalize(-uLight_DirVS);

float NdotL = max(dot(N, L), 0.0);

vec3 diffuse = uKd * uLight_Id * NdotL;

vec3 V = normalize(-vPosVS);
vec3 R = reflect(-L, N);
float RdotV = max(dot(R, V), 0.0);
vec3 specular = vec3(0.0);
if (NdotL > 0.0)
specular = uKs * uLight_Is * pow(RdotV, uShininess);

vec3 color = diffuse + specular;
return vec4(color, 1.0);
}


// Foco - Spot Light
subroutine(LightFunc)
vec4 applySpot()
{
vec3 N = normalize(vNormalVS);

vec3 Lp = vPosVS - uLight_PosVS;   // desde luz hacia punto
vec3 L  = normalize(uLight_PosVS - vPosVS); // hacia la luz

// Dirección del foco (desde luz hacia escena)
vec3 D = normalize(uLight_DirVS);

// cos del ángulo entre la dirección del foco y la dirección hacia el punto
float cosTheta = dot(normalize(Lp), D);

// fuera del cono
if (cosTheta < uSpot_cosGamma)
return vec4(0.0);

// factor suave
float spotFactor = pow(cosTheta, uSpot_exp);

float NdotL = max(dot(N, L), 0.0);

vec3 diffuse = uKd * uLight_Id * NdotL;

vec3 V = normalize(-vPosVS);
vec3 R = reflect(-L, N);
float RdotV = max(dot(R, V), 0.0);
vec3 specular = vec3(0.0);

if (NdotL > 0.0)
specular = uKs * uLight_Is * pow(RdotV, uShininess);

vec3 color = (diffuse + specular) * spotFactor;
return vec4(color, 1.0);
}

void main()
{
FragColor = uLightFunc();
}
