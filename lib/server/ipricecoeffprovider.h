#pragma once

namespace ai
{
    class IPriceCoeffProvider
    {
    public:
        virtual ~IPriceCoeffProvider() = default;
        virtual float GetPriceCoeffForObj(Obj const*) const = 0;
    };
}
