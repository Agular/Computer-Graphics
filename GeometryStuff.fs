
#version 420 core  // 450, 330 core , compatibility
uniform sampler2D tex;
uniform vec3 lightColor;
uniform float shineDamper;
uniform float reflectivity;

in vec3 toCameraVector;
in vec3 toLightVector;
in vec3 surfaceNormal;
layout(location=0) out vec4 fColor;

in VertexData {
    vec4 Color;
    vec2 zTex;
} VertexIn;

void main() {
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitToLightVector = normalize(toLightVector);

    float nDot1 = dot(unitNormal, unitToLightVector);
    float brightness = max(nDot1, 0.2);
    vec3 diffuse = brightness * lightColor;


    vec3 unitToCameraVector = normalize(toCameraVector);
    vec3 lightDirection = -unitToLightVector;
    vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

    float specularFactor = dot(reflectedLightDirection, unitToCameraVector);
    specularFactor = max(specularFactor, 0.0f);
    float dampedFactor = pow(specularFactor, shineDamper);
    vec3 finalSpecular = dampedFactor *reflectivity* lightColor;
    
    //fColor = vec4(diffuse, 1.0f) * texture(tex, vs_tex) + vec4(finalSpecular, 1.0f);
    fColor = VertexIn.Color * texture(tex, VertexIn.zTex);
    //(fColor = vec4(diffuse, 1.0f) *vec4(toCameraVector,1.0f)* texture(tex, VertexIn.zTex) + vec4(finalSpecular, 1.0f); // for disco effect!!
 }
