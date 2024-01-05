#include "Renderer.h"
//#include <cmath> // For sin and cos functions

void Renderer::Render()
{
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); ++y)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); ++x)
		{
			glm::vec2 coordinate = {};
			
			m_ImageDataBuffer[x + y * m_FinalImage->GetWidth()] = PerPixel(coordinate);
		}
	}

	m_FinalImage->SetData(m_ImageDataBuffer);
}

uint32_t Renderer::PerPixel(glm::vec2 coordinate)
{
	return uint32_t(0xFFFF00FF);
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


