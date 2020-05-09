#version 330 core
in vec2 TexCoord;
uniform sampler2D text;
out vec4 fragColor;
void main()
{
    fragColor=texture2D(text, TexCoord);
}