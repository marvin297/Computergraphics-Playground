#include "Renderer.h"

void Renderer::Render()
{
	// Update rotation angle for spinning effect
	rotationAngle += 0.01f; // Adjust rotation speed as needed
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

    ApplyAntialiasing();
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

void Renderer::ApplyAntialiasing()
{
    for (uint32_t y = 1; y < m_FinalImage->GetHeight() - 1; ++y)
    {
        for (uint32_t x = 1; x < m_FinalImage->GetWidth() - 1; ++x)
        {
            // Compute average color of the neighboring pixels
            uint32_t avgColor = AverageColor(x, y);

            // Blend the current pixel color with the average color
            uint32_t currentColor = m_ImageDataBuffer[x + y * m_FinalImage->GetWidth()];
            m_ImageDataBuffer[x + y * m_FinalImage->GetWidth()] = BlendColors(currentColor, avgColor, 0.5f);
        }
    }

    m_FinalImage->SetData(m_ImageDataBuffer);
}

uint32_t Renderer::AverageColor(uint32_t x, uint32_t y)
{
    uint32_t sum[4] = { 0 }; // RGBA channels
    uint32_t count = 0;

    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (dx == 0 && dy == 0) continue; // Skip the center pixel

            uint32_t color = m_ImageDataBuffer[(x + dx) + (y + dy) * m_FinalImage->GetWidth()];
            sum[0] += (color >> 24) & 0xFF; // Red
            sum[1] += (color >> 16) & 0xFF; // Green
            sum[2] += (color >> 8) & 0xFF;  // Blue
            sum[3] += color & 0xFF;         // Alpha

            count++;
        }
    }

    // Calculate average for each channel
    for (int i = 0; i < 4; ++i)
    {
        sum[i] /= count;
    }

    // Reconstruct the color
    return (sum[0] << 24) | (sum[1] << 16) | (sum[2] << 8) | sum[3];
}

uint32_t Renderer::BlendColors(uint32_t color1, uint32_t color2, float blendFactor = 0.5)
{
    // Extract individual channels
    uint32_t r1 = (color1 >> 24) & 0xFF;
    uint32_t g1 = (color1 >> 16) & 0xFF;
    uint32_t b1 = (color1 >> 8) & 0xFF;
    uint32_t a1 = color1 & 0xFF;

    uint32_t r2 = (color2 >> 24) & 0xFF;
    uint32_t g2 = (color2 >> 16) & 0xFF;
    uint32_t b2 = (color2 >> 8) & 0xFF;
    uint32_t a2 = color2 & 0xFF;

    // Blend the channels
    uint32_t r = static_cast<uint32_t>(r1 * (1 - blendFactor) + r2 * blendFactor);
    uint32_t g = static_cast<uint32_t>(g1 * (1 - blendFactor) + g2 * blendFactor);
    uint32_t b = static_cast<uint32_t>(b1 * (1 - blendFactor) + b2 * blendFactor);
    uint32_t a = static_cast<uint32_t>(a1 * (1 - blendFactor) + a2 * blendFactor);

    return (r << 24) | (g << 16) | (b << 8) | a;
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


