#version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormals;
layout (location = 2) in vec2 vTextureCoords;

uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;
uniform bool HasUV;
uniform vec3 lightPos;
uniform vec3 eyePos;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;

out vec3 posEye;
out vec3 normEye;
out vec2 uvs;

void main()
{
	uvs = vTextureCoords;
	normEye = normalize(NormalMatrix * vNormals);
	posEye = vec3(ModelViewMatrix * vec4(vPos, 1.0));

	gl_Position = MVP * vec4(vPos, 1.0);
}