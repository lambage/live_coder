#version 330

in vec2 TexCoord;
out vec4 oColor;
uniform sampler2D image;
uniform sampler2D back_image;
uniform float lerp_value;

void main(void)
{
	oColor = mix(texture(back_image, TexCoord), texture(image, TexCoord), lerp_value);
}