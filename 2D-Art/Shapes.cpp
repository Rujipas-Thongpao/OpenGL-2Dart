#pragma once 

#include <stdio.h>
#include <vector>
#include "Vertex.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Shapes{
public:
	static std::vector<Vertex> CreateCircle(float radius, int segments) {
		std::vector<Vertex> vertices;

		vertices.push_back({ 0.0f, 0.0f, 0.0f, 0.5f, 0.5f });

		// Generate vertices around the circle
		for (int i = 0; i <= segments; ++i) {
			float angle = 2.0f * M_PI * i / segments;

			float x = radius * cos(angle);
			float y = radius * sin(angle);

			// Texture coordinates mapped from [-radius, radius] to [0, 1]
			float u = (x / radius + 1.0f) * 0.5f;
			float v = (y / radius + 1.0f) * 0.5f;

			vertices.push_back({ x, y, 0.0f, u, v });
		}

		return vertices;
	}
};
