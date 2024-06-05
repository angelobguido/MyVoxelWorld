#version 450 core

#define EPS 0.001

out vec4 fragmentTexColor;
in vec2 fragmentTexCoord;

uniform sampler2D colorBuffer;

//const float[] kernel = {
//    1.0/16.0, 2.0/16.0, 1.0/16.0,
//    2.0/16.0, 4.0/16.0, 2.0/16.0,
//    1.0/16.0, 2.0/16.0, 1.0/16.0
//};

const float[] kernel = {
    1.0/9.0, 1.0/9.0, 1.0/9.0,
    1.0/9.0, 1.0/9.0, 1.0/9.0,
    1.0/9.0, 1.0/9.0, 1.0/9.0
};

float convolute(inout float[9] channel){
    float sum = 0;
    for (int i =0; i < 9; i++) {
        sum += channel[i]*kernel[i];
    }

    return sum;
}


vec4 applyBlur(vec2 xy)
{
    ivec2 iResolution = textureSize(colorBuffer,0);

    vec4 c = texture(colorBuffer, xy);
    float x1 = 1./iResolution.x;
    float y1 = 1./iResolution.y;
    float redChannel[9];
    float greenChannel[9];
    float blueChannel[9];

    vec2 positions[9] = {
    xy+vec2(-x1, y1), xy+vec2(0, y1), xy+vec2(x1, y1),
    xy+vec2(-x1, 0), xy+vec2(0, 0), xy+vec2(x1, 0),
    xy+vec2(-x1, -y1), xy+vec2(0, -y1), xy+vec2(x1, -y1),
    };

    //Initialize arrays
    for (int i =0; i < 9; i++) {
        c = texture(colorBuffer, positions[i]);
        redChannel[i] = c.r;
        greenChannel[i] = c.g;
        blueChannel[i] = c.b;
    }

    //Sort red channel
    float red = convolute(redChannel);

    //Sort green channel
    float green = convolute(greenChannel);

    //Sort blue channel
    float blue = convolute(blueChannel);

    vec4 color = vec4(1);
    color.r = red;
    color.g = green;
    color.b = blue;
    return color;

}
void main()
{
    fragmentTexColor = applyBlur(fragmentTexCoord);
}