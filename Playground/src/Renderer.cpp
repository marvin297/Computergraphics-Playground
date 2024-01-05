#include "Renderer.h"

void Renderer::Render()
{

	// Update rotation angle for spinning effect
	rotationAngle += 0.04f; // Adjust rotation speed as needed
	squareCenter = { m_FinalImage->GetWidth() / 2.0f, m_FinalImage->GetHeight() / 2.0f };
	squareSize = glm::min(m_FinalImage->GetWidth(), m_FinalImage->GetHeight()) / glm::sqrt(2.0f);

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); ++y)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); ++x)
		{
			glm::vec2 coordinate = {x, y};
			
			m_ImageDataBuffer[x + y * m_FinalImage->GetWidth()] = PerPixel(coordinate);
		}
	}

	m_FinalImage->SetData(m_ImageDataBuffer);
}

uint32_t Renderer::PerPixel(glm::vec2 coordinate)
{
	// Rotate the point around the center of the square
	float s = glm::sin(rotationAngle);
	float c = glm::cos(rotationAngle);

	// Translate point back to origin
	coordinate -= squareCenter;

	// Rotate point
	float xnew = coordinate.x * c - coordinate.y * s;
	float ynew = coordinate.x * s + coordinate.y * c;

	// Translate point back
	coordinate = { xnew + squareCenter.x, ynew + squareCenter.y };

	// Check if the coordinate is within the square
	if (glm::abs(coordinate.x - squareCenter.x) <= squareSize / 2 &&
		glm::abs(coordinate.y - squareCenter.y) <= squareSize / 2)
	{
		return uint32_t(0xFFFFFFFF); // Square color
	}

	return uint32_t(0x00000000); // Background color
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageDataBuffer;
	m_ImageDataBuffer = new uint32_t[width * height];

}


