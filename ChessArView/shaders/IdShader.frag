#version 330 core

in vec3 FragmentColor;

out vec4 FragColor;


void main()
{	
	FragColor = vec4(FragmentColor, 1.0);
}