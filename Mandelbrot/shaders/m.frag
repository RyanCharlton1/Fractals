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
    while (x1*x1 + y1*y1 < 400 && iteration < MAXITERATIONS){
        tempx = x1;
        x1 = x1*x1 - y1*y1 + x2;
        y1 = 2*tempx*y1 + y2;
        iteration++;
    }

    if(iteration == MAXITERATIONS){
        colorOut = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    vec3 above, below;
    float i = float(iteration) +2.0 - log(log(sqrt(x1*x1 + y1*y1)));
    float p = mod(i, 50.0) / 50.0;
    if (p < 0.16){
        below = vec3(0, 7, 100);
        above = vec3(32, 107, 203);
        p = p / 0.16;
    } else if (p < 0.42){
        below = vec3(32, 107, 203);
        above = vec3(237, 255, 255);
        p = (p - 0.16) / 0.26;
    } else if (p < 0.64){
        below = vec3(237, 255, 255);
        above = vec3(255, 170, 0);
        p = (p - 0.42) / 0.22;
    } else if (p < 0.85){
        below = vec3(255, 170, 0);
        above = vec3(0, 2, 0);
        p = (p - 0.64) / 0.21;
    } else{
        below = vec3(0, 2, 0);
        above = vec3(0, 7, 100);
        p = (p - 0.85) / 0.15;
    }

    colorOut = vec4((below + (above - below) * p) / 255.0, 1.0);
}