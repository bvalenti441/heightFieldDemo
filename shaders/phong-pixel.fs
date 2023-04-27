#version 400

uniform sampler2D tex;
uniform vec3 eyePos;
uniform vec3 lightPos;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;

in vec3 posEye;
in vec3 normEye;
in vec2 uvs;

out vec4 FragColor;

void main()
{
	vec3 lightDir = normalize(lightPos - posEye);
	vec3 diffuse = material.diffuse * max(dot(normEye, lightDir), 0.0);
	
	vec3 viewDir = normalize(eyePos - posEye);
	vec3 reflectDir = reflect(-lightDir, normEye);
	vec3 specular = material.specular * pow(max(dot(viewDir, reflectDir), 0.0), 32);
	FragColor = vec4(texture(tex, uvs*5.0f).xyz + (material.ambient * diffuse * specular), 1.0f);
}
