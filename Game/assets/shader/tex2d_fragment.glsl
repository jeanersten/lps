#version 330 core

in vec4 vcolor;
in vec2 vtexcoords;

out vec4 fcolor;

uniform sampler2D img_texture;

void main()
{
    fcolor = texture(img_texture, vtexcoords) * vcolor;
}
