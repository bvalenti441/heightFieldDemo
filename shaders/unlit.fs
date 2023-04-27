#version 400
uniform sampler2D tex;
in vec2 uvs;
out vec4 FragColor;

void main()
{
   vec4 c = texture(tex, uvs);
   FragColor = vec4(c);
}
