#include <array>

struct Vertex
{
	float pos[3];
	float uv[2];
	float normal[3];

	Vertex(const std::array<float, 3>& position, const std::array<float, 2>& uvCoords, const std::array<float, 3>& normal)
	{
		for (int i = 0; i < 3; ++i)
		{
			pos[i] = position[i];
			this->normal[i] = abs(normal[i]);
		}

		uv[0] = uvCoords[0];
		uv[1] = uvCoords[1];
	}
};