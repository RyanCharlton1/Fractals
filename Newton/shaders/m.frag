#version 460 core
#define MAXITERATIONS 40
#define MARGIN 0.01

uniform float targetx;
uniform float targety;
uniform float rangex;
uniform float rangey;

in vec2 pixel;
out vec4 colorOut;

vec2 cmul(vec2 z1, vec2 z2){
    return vec2(z1.x*z2.x - z1.y*z2.y, z1.x*z2.y + z1.y*z2.x);
}

vec2 cdiv(vec2 z1, vec2 z2){
    float denom = z2.x*z2.x + z2.y*z2.y;
    return vec2((z1.x*z2.x + z1.y*z2.y) / denom, (z1.y*z2.x - z1.x*z2.y) / denom);
}

vec2 cpow(vec2 z, int n){
    vec2 zcpy = z;
    for (int i = 1; i < n; i++)
        z = cmul(z, zcpy);
    
    return z;
}

vec2 csin(vec2 z){
    //sin(a+bi)=sinacoshb+icosasinhb
    return vec2(sin(z.x) * cosh(z.y), cos(z.x) * sinh(z.y));
}

vec2 ccos(vec2 z){
    //cos(a+bi)=cosacoshb−isinasinhb
    return vec2(cos(z.x) * cosh(z.y), -1.0 * sin(z.x) * sinh(z.y));
}

vec2 func(vec2 z){
    return csin(z);
}

vec2 der(vec2 z){
    return ccos(z);
}

void main(){
    vec2 z;
    z.x = targetx + rangex * (pixel.x + 1.0) / 2;
    z.y = targety + rangey * (pixel.y + 1.0) / 2;

    if(abs(length(func(z))) < MARGIN){
        colorOut = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    int interation = 0;
    while(abs(length(func(z))) > MARGIN && interation < MAXITERATIONS){
        z -= cdiv(func(z), der(z));
        interation++;
    }

    if (interation == MAXITERATIONS){
        colorOut = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    

    float p = float(interation) / float(MAXITERATIONS);
    
    colorOut = vec4(vec3(cos(z.x) / 2.0, sin(z.y), 0.0) * (1.0 - p), 1.0);
}