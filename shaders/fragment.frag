#version 330 core

in vec2 fragmentTexCoord;

uniform sampler2D colorBuffer;

out vec4 color;

void main()
{
    vec4 pixel = texture(colorBuffer, fragmentTexCoord);
    color = vec4(pixel);
}