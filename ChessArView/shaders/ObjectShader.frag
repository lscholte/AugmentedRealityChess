#version 330 core

in vec3 FragmentColor;
in vec4 FragmentWorldPosition;
in vec3 FragmentNormal;
in vec2 FragmentTextureCoord;

out vec4 FragColor;

uniform bool HasImage;
uniform sampler2D Image;

void main()
{	
	vec3 color;
	if (HasImage)
	{
		color = texture(Image, FragmentTextureCoord).rgb;
	}
	else
	{
		color = FragmentColor;
	}
	
	if (FragmentNormal != vec3(0.0f, 0.0f, 0.0f))
	{
		vec3 LightPosition = vec3(4.5, -4.5, 5.0);
		vec3 L = normalize(FragmentWorldPosition.xyz - LightPosition);
		vec3 N = normalize(FragmentNormal);

		float ambient = 0.2;
		float diffuse = max(dot(L, N), 0.0);

		color = color*ambient + color*diffuse*5;
	}
	FragColor = vec4(color, 1.0);
}