#pragma once

namespace ai
{
    class IPriceCoeffProvider
    {
    public:
        virtual ~IPriceCoeffProvider();
        virtual float GetPriceCoeffForObj(Obj const*) const = 0;
    };
}
