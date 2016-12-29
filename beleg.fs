
#version 420 core  // 450, 330 core , compatibility
layout(location=0) out vec4 fColor;
uniform sampler2D tex;
uniform vec3 lightColor;
uniform float shineDamper;
uniform float reflectivity;

in vec4 Color;
in vec2 TexCoord;
in vec3 Normal;
in vec3 toLightVector;
in vec3 toCameraVector;

void main() {
	//Calculate the diffuse light of a vector.
	vec3 unitNormal = normalize(Normal);
	vec3 unitToLightVector = normalize(toLightVector);
	float brightness = dot(unitNormal, unitToLightVector);
	brightness = max(brightness, 0.2);
	vec3 diffuse = brightness * lightColor;

	//Calculate the specular light of a vector.
	vec3 unitToCameraVector = normalize(toCameraVector);
	vec3 lightVector = -toLightVector;
	vec3 reflectedLightVector = reflect(lightVector, unitNormal);
	float specularFactor = dot(reflectedLightVector, toCameraVector);
	specularFactor = max(specularFactor, 0.0);
	float dampedFactor = pow(specularFactor, shineDamper);
	vec3 finalSpecular = dampedFactor * reflectivity * lightColor;
	
	
	
	//Apply diffuse light to vectors.
	//fColor = vec4(diffuse, 1.0f) * texture(tex, TexCoord);    

	//Apply diffuselight AND specular light to vectors.
	fColor = vec4(diffuse, 1.0f) * texture(tex, TexCoord) + vec4(finalSpecular, 1.0f);
	
	
	//Apply ONLY specular light to vectors.
	//fColor = texture(tex, TexCoord) + vec4(finalSpecular, 1.0f);

}
