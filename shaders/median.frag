#version 450 core

#define EPS 0.001

out vec4 fragmentTexColor;
in vec2 fragmentTexCoord;

uniform sampler2D colorBuffer;

void sortArray(inout float[9] array){
    int i, j;
    float key;
    for (i = 1; i < 9; i++) {
        key = array[i];
        j = i - 1;

        // Move elements of array[0..i-1], that are greater than key,
        // to one position ahead of their current position
        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }
}

vec4 applyMedian(vec2 xy)
{
    ivec2 iResolution = textureSize(colorBuffer,0);

    vec4 c;
    float x1 = 1./iResolution.x;
    float y1 = 1./iResolution.y;
    float array[9];
    float array1[9];
    float array2[9];

    vec2 positions[9] = {
        xy+vec2(-x1, y1), xy+vec2(0, y1), xy+vec2(x1, y1),
        xy+vec2(-x1, 0), xy+vec2(0, 0), xy+vec2(x1, 0),
        xy+vec2(-x1, -y1), xy+vec2(0, -y1), xy+vec2(x1, -y1),
    };

    //Initialize arrays
    for (int i =0; i < 9; i++) {
        c = texture(colorBuffer, positions[i]);
        array[i] = c.r;
        array1[i] = c.g;
        array2[i] = c.b;
    }

    //Sort red channel
    sortArray(array);

    //Sort green channel
    sortArray(array1);

    //Sort blue channel
    sortArray(array2);

    vec4 median;
    median.r = array[4];
    median.g = array1[4];
    median.b = array2[4];
    return median;

}
void main()
{
    fragmentTexColor = applyMedian(fragmentTexCoord);
}