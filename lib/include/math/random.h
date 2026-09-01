#pragma once

class RandomCoeffWithDispersion
{
public:
    RandomCoeffWithDispersion() = default;
    RandomCoeffWithDispersion(float coeff, float dispersion) :
        baseCoeff(coeff),
        baseDispersion(dispersion)
    {
    }

    float baseCoeff = 1.0f;
    float baseDispersion = 0.0f;
};
