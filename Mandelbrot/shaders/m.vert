#version 460 core

in vec2 pos;
out vec2 pixel;

void main(){
    gl_Position = vec4(pos, 0.0, 1.0);
    pixel = pos;
}