#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include <math/vector.h>

namespace ai
{
    class Vehicle;

    class VehicleRecollectionPrototypeInfo : public PrototypeInfo
    {
    public:
        virtual ai::Obj* CreateTargetObject() const;
        VehicleRecollectionPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    };

    class VehicleRecollection : public Obj
    {
        friend class VehicleRecollectionPrototypeInfo;
    public:
        class ReollectionItem
        {
        public:
            ReollectionItem(CVector const&, float);

            virtual void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
            virtual void SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;

        public:
            CVector pos;
            float time;
        };

    public:
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void RenderDebugInfo() const;
        virtual void Update(float, unsigned int);
        void Clear();
        virtual ai::VehicleRecollectionPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;
        VehicleRecollection(ai::VehicleRecollectionPrototypeInfo const&);
        CVector GetRecollectionPosition(float) const;
        static m3d::Class* GetBaseClass();
        Vehicle* GetVehicle() const;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        void SetVehicle(Vehicle const*);

    protected:
        virtual ~VehicleRecollection();
        virtual void _InternalCreateVisualPart();

    private:
        static m3d::Object* CreateObject();
        virtual m3d::Object* Clone();

    public:
        RT_CLASS_DECLARE(VehicleRecollection);

    private:
        std::vector<ReollectionItem> m_recollectionItems;
        int m_vehicleId;
    };
}
