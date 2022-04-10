#version 330 core

in vec4 FragmentColor;
in vec4 FragmentWorldPosition;
in vec3 FragmentNormal;
in vec2 FragmentTextureCoord;

out vec4 FragColor;

uniform bool HasImage;
uniform sampler2D Image;

void main()
{	
	vec4 color;
	if (HasImage)
	{
		color = texture(Image, FragmentTextureCoord);
	}
	else
	{
		color = FragmentColor;
	}
	
	if (FragmentNormal != vec3(0.0f, 0.0f, 0.0f))
	{
		vec3 LightPosition = vec3(4.5, -4.5, 1000.0);
		vec3 L = normalize(LightPosition - FragmentWorldPosition.xyz);
		vec3 N = normalize(FragmentNormal);

		float ambient = 0.5;
		float diffuse = max(dot(L, N), 0.0);

		color.rgb = color.rgb*ambient + color.rgb*diffuse;
	}
	FragColor = color;
}