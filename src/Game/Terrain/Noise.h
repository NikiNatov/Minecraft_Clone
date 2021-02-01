#pragma once

#include <glm\glm.hpp>

/// <summary>
/// Noise algorithm from Hopson97's terrain generation
/// https://github.com/Hopson97/open-builder/blob/master/src/server/world/terrain_generation.cpp
/// </summary>

class Noise
{
public:
    Noise(int seed, int octaves, float amplitude, float smoothness, float roughness, float offset);

    float GetNoise(float blockGlobalX, float blockGlobalY);

    inline void SetSeed(int seed) { m_Seed = seed; }
    inline void SetOctaves(int octaves) { m_Octaves = octaves; }
    inline void SetAmplitude(float amplitude) { m_Amplitude = amplitude; }
    inline void SetSmoothness(float smoothness) { m_Smoothness = smoothness; }
    inline void SetRoughness(float roughness) { m_Roughness = roughness; }
    inline void SetOffset(float offset) { m_Offest = offset; }

    inline int GetSeed() { return m_Seed; }
    inline int GetOctaves() { return m_Octaves; }
    inline float GetAmplitude() { return m_Amplitude; }
    inline float GetSmoothness() { return m_Smoothness; }
    inline float GetRoughness() { return m_Roughness; }
    inline float GetOffset() { return m_Offest; }
private:
    int m_Seed;
    int m_Octaves;
    float m_Amplitude;
    float m_Smoothness;
    float m_Roughness;
    float m_Offest;
};