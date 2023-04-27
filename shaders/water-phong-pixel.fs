#version 400

uniform sampler2D tex;
uniform samplerCube cubemap;
uniform vec3 eyePos;

in vec3 posEye;
in vec3 normEye;
in vec2 uvs;

out vec4 FragColor;

void main()
{
	vec3 v = normalize(posEye-eyePos);
	vec3 reflectDir = reflect(v, normEye);
	vec4 reflectColor = texture(cubemap, reflectDir);

	//vec3 lightDir = normalize(lightPos - posEye);
	//vec3 diffuse = material.diffuse * max(dot(normEye, lightDir), 0.0);
	
	//vec3 specular = material.specular * pow(max(dot(viewDir, reflectDir), 0.0), 32);
	FragColor = vec4(reflectColor.xyz * texture(tex, uvs).xyz, 1.0f);
}
