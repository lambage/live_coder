#version 330

uniform mat4 ciModelViewProjection;
in vec4 ciPosition;
in vec4 ciColor;
in vec2 ciTexCoord0;
out vec2 TexCoord;

void main(void) 
{
	gl_Position = ciModelViewProjection * ciPosition;
	TexCoord = ciTexCoord0;
}