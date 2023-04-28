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
	FragColor = vec4(reflectColor.xyz * texture(tex, uvs).xyz, 1.0f);
}
