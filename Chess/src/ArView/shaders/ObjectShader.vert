#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 Color;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec2 TextureCoord;

out vec4 FragmentColor;
out vec4 FragmentWorldPosition;
out vec3 FragmentNormal;
out vec2 FragmentTextureCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    vec4 v = Projection * View * Model * vec4(Position, 1.0);
	
	//Flip y because OpenCV y axis goes downward, OpenGL's goes upward
	v.y *= -1;
	
	FragmentColor = Color;
	FragmentWorldPosition = Model * vec4(Position, 1.0);
	FragmentNormal = Normal;
	FragmentTextureCoord = TextureCoord;
	gl_Position = v;
}