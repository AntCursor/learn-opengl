#version 330 core
out vec4 FragColor;

uniform vec4 uniColor;

in vec2 oTexCoords;
uniform sampler2D oTexture1;
uniform sampler2D oTexture2;
uniform float oMix;

void main()
{
    FragColor = uniColor * mix(texture(oTexture1, oTexCoords), texture(oTexture2, oTexCoords), oMix);
}
