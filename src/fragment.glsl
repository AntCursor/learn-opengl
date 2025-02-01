#version 330 core
out vec4 FragColor;

uniform vec4 uniColor;

in vec2 oTexCoords;
uniform sampler2D oTexture;

void main()
{
    FragColor = /* uniColor**/ texture(oTexture, oTexCoords);
}
