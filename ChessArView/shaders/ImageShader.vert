#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 3) in vec2 TextureCoords;

out vec2 FragmentTextureCoords;

void main()
{
    gl_Position = vec4(Position, 1.0);
    FragmentTextureCoords = TextureCoords;
}