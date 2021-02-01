#include "pch.h"
#include "Noise.h"

#include <glm\gtc\noise.hpp>

Noise::Noise(int seed, int octaves, float amplitude, float smoothness, float roughness, float offset)
    : m_Seed(seed), m_Octaves(octaves), m_Amplitude(amplitude), m_Smoothness(smoothness), m_Roughness(roughness), m_Offest(offset)
{
}

float Noise::GetNoise(float blockGlobalX, float blockGlobalY)
{
    float value = 0;
    float accumulatedAmps = 0;

    for (int i = 0; i < m_Octaves; i++)
    {
        float frequency = glm::pow(2.0f, i);
        float amplitude = glm::pow(m_Roughness, i);

        float x = blockGlobalX * frequency / m_Smoothness;
        float y = blockGlobalY * frequency / m_Smoothness;

        float noise = glm::simplex(glm::vec3{ m_Seed + x, m_Seed + y, m_Seed });
        noise = (noise + 1.0f) / 2.0f;
        value += noise * amplitude;
        accumulatedAmps += amplitude;
    }

    return value / accumulatedAmps;
}
