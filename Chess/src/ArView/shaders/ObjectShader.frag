#version 330 core

in vec4 FragmentColor;
in vec4 FragmentWorldPosition;
in vec3 FragmentNormal;
in vec2 FragmentTextureCoord;

out vec4 FragColor;

uniform bool HasImage;
uniform sampler2D TextureDiffuse;

uniform vec3 CameraWorldPosition;

void main()
{	
	vec4 color;
	if (HasImage)
	{
		color = texture(TextureDiffuse, FragmentTextureCoord);
	}
	else
	{
		color = FragmentColor;
	}
	
	if (FragmentNormal != vec3(0.0, 0.0, 0.0))
	{
		vec3 LightPosition = vec3(4.5, -4.5, 1000.0);
		vec3 LightColor = vec3(1.0, 1.0, 1.0);

		vec3 L = normalize(LightPosition - FragmentWorldPosition.xyz);
		vec3 N = normalize(FragmentNormal);

		float ambientStrength = 0.5;
		vec3 ambientIllumination = ambientStrength * LightColor;
		
		float diffuse = max(dot(L, N), 0.0);
		vec3 diffuseIllumination = diffuse * LightColor;
		
		vec3 specularIllumination = vec3(0.0, 0.0, 0.0);
		if (diffuse > 0)
		{
			vec3 V = normalize(CameraWorldPosition - FragmentWorldPosition.xyz);
			vec3 R = normalize(L - 2.0*dot(L, N)*N);
			float specularAngle = max(dot(R, V), 0.0);
			float shininess = 10.0;
			float specular = pow(specularAngle, shininess);
			float specularStrength = 2.0;
			specularIllumination = specularStrength * specular * LightColor;
		}


		color.rgb = color.rgb * (ambientIllumination + diffuseIllumination + specularIllumination);
	}
	FragColor = color;
}