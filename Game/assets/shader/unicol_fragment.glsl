#version 330 core

in vec4 vcolor;
out vec4 fcolor;

uniform vec4 uni_color;

void main()
{
    fcolor = vcolor * uni_color;
}
