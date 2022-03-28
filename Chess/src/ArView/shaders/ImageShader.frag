#version 330 core

out vec4 FragColor;

in vec2 FragmentTextureCoords;

uniform sampler2D Image;

void main()
{
    vec3 color = texture(Image, FragmentTextureCoords).rgb;

	FragColor = vec4(color, 1.0);
}