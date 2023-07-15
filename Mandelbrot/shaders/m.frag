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

    
    vec3 above, below;
    float p = float(iteration) / float(MAXITERATIONS);
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
    } else if (p < 1.0){
        below = vec3(0, 2, 0);
        above = vec3(0, 7, 100);
        p = (p - 0.85) / 0.15;
    } else{
        colorOut = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }
    
    /*
    vec3 mapping[16] = vec3[16](vec3(66.0, 30.0, 15.0),
    vec3(25.0, 7.0, 26.0),
    vec3(9.0, 1.0, 47.0),
    vec3(4.0, 4.0, 73.0),
    vec3(0.0, 7.0, 100.0),
    vec3(12.0, 44.0, 138.0),
    vec3(24.0, 82.0, 177.0),
    vec3(57.0, 125.0, 209.0),
    vec3(134.0, 181.0, 229.0),
    vec3(211.0, 236.0, 248.0),
    vec3(241.0, 233.0, 191.0),
    vec3(248.0, 201.0, 95.0),
    vec3(255.0, 170.0, 0.0),
    vec3(204.0, 128.0, 0.0),
    vec3(153.0, 87.0, 0.0),
    vec3(106.0, 52.0, 3.0));

    
    if (iteration < MAXITERATIONS){
        colorOut = vec4(mapping[iteration % 16] / 255.0, 1.0);
    }
    else 
        colorOut = vec4(0.0, 0.0, 0.0, 1.0);
    */
    colorOut = vec4((below + (above - below) * p) / 255.0, 1.0);
    //colorOut = vec4(p, 0.0, 0.0, 1.0);
}