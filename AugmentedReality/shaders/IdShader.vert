#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;

out vec3 FragmentColor;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    vec4 v = Projection * View * Model * vec4(Position, 1.0);
	
	//Flip y because OpenCV y axis goes downward, OpenGL's goes upward
	v.y *= -1;
	
	FragmentColor = Color;
	gl_Position = v;
}