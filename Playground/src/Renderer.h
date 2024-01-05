#pragma once

#include "Walnut/Image.h"
#include <memory>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer() = default;

	void Render();
	void OnResize(uint32_t width, uint32_t height);
	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

private:
	uint32_t PerPixel(glm::vec2 coordinate);

private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageDataBuffer = nullptr;

private:
	// Define the center and size of the square
	glm::vec2 squareCenter{ 0.0f };
	float squareSize{ 0.0f }; // Adjust as needed
	float rotationAngle = 0.0f; // This will be updated each frame
};