#pragma once

struct RandomCoeffWithDispersion
{
    float baseCoeff = 1.0f;
    float baseDispersion = 0.0f;

    RandomCoeffWithDispersion() = default;
    RandomCoeffWithDispersion(float coeff, float dispersion) :
        baseCoeff(coeff),
        baseDispersion(dispersion)
    {
    }
};
