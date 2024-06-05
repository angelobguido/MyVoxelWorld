#version 330 core

in vec2 fragmentTexCoord;

uniform sampler2D colorBuffer;

out vec4 color;

const float dotSize = 0.005;

void main()
{

    ivec2 resolution = textureSize(colorBuffer,0);
    float aspectRatio = float(resolution.y)/resolution.x;
    vec4 pixel = texture(colorBuffer, fragmentTexCoord);
    if((abs(fragmentTexCoord.x - 0.5) < dotSize*aspectRatio) && (abs(fragmentTexCoord.y - 0.5) < dotSize)){
        pixel = vec4(1); //Add a white dot at the center of the screen
    }


    color = vec4(pixel);
}