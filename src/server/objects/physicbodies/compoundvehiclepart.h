#pragma once
#include "vehiclepart.h"

namespace ai
{
    class CompoundVehiclePartPrototypeInfo : public VehiclePartPrototypeInfo
    {
    public:
        class TPartInfo
        {
        private:
            int prototypeId;
            CStr prototypeName;
            unsigned int index;
        };

    public:
        CompoundVehiclePartPrototypeInfo();
        virtual void PostLoad();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        std::map<CStr, TPartInfo> m_PartInfo;
    };

    class CompoundVehiclePart : public VehiclePart
    {
    public:
        class TVehiclePart
        {
        public:
            TVehiclePart(::ai::VehiclePart*, unsigned int);

        private:
            VehiclePart* vp;
            unsigned int index;
        };

    private:
        std::map<CStr,TVehiclePart> m_vehicleParts;
    };
}
