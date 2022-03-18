#version 330 core

in vec3 FragmentColor;
in vec4 FragmentWorldPosition;
in vec3 FragmentNormal;

out vec4 FragColor;


void main()
{	
	vec3 color;
	if (FragmentNormal == vec3(0.0f, 0.0f, 0.0f))
	{
		color = FragmentColor;
	}
	else
	{
		vec3 LightPosition = vec3(3.0, -3.0, 10.0);
		vec3 L = normalize(FragmentWorldPosition.xyz - LightPosition);
		vec3 N = normalize(FragmentNormal);

		float ambient = 0.2;
		float diffuse = max(dot(L, N), 0.0);

		color = FragmentColor*ambient + FragmentColor*diffuse*5;
	}
	FragColor = vec4(color, 1.0);
}