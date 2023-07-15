#version 460 core

uniform float targetx;
uniform float targety;
uniform float rangex;
uniform float rangey;

in vec2 pixel;
out vec4 colorOut;

void main(){
    colorOut = vec4((pixel.x + 1) / 2, targetx, (pixel.y + 1) / 2, 1.0);
}