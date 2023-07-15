#version 460 core
#define MAXITERATIONS 900
uniform float targetx;
uniform float targety;
uniform float rangex;
uniform float rangey;

in vec2 pixel;
out vec4 colorOut;

void main(){
    float x1 = 0.0;
    float y1 = 0.0;
    float x2 = targetx + rangex * (pixel.x + 1.0) / 2;
    float y2 = targety + rangey * (pixel.y + 1.0) / 2;

    int iteration = 0;
    float tempx;
    while (x1*x1 + y1*y1 < 4 && iteration < MAXITERATIONS){
        tempx = x1;
        x1 = x1*x1 - y1*y1 + x2;
        y1 = 2*tempx*y1 + y2;
        iteration++;
    }


    colorOut = vec4(float(iteration) / float(MAXITERATIONS), 0.0, 0.0, 1.0);
}