struct Material
{
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shininess;
	float padd[3];

	Material(const std::array<float, 4>& ambientMaterial, const std::array<float, 4>& diffuseMaterial,
		const std::array<float, 4>& specularMaterial, float shininessValue)
	{
		for (int i = 0; i < 4; ++i)
		{
			ambient[i] = ambientMaterial[i];
			diffuse[i] = diffuseMaterial[i];
			specular[i] = specularMaterial[i];
		}

		for (int i = 0; i < 3; ++i)
		{
			padd[i] = 0.0f;
		}

		shininess = shininessValue;
	}
};