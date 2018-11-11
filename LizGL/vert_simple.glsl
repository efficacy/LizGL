#version 330 core
attribute vec2 position;
attribute vec4 color;
varying vec4 colour;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
	colour = color;
} 
