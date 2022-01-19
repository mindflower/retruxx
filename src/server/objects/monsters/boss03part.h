#pragma once
#include <server/objects/physicbodies/vehiclepart.h>

struct dContact;

namespace ai
{
    class Boss03PartPrototypeInfo : public VehiclePartPrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    };

    class Boss03Part :  public VehiclePart
    {
    public:
        static int __fastcall CollideBoss03PartAndVehiclePart(Boss03Part *,VehiclePart *,dContact *,unsigned int &,bool);
        virtual Boss03PartPrototypeInfo const * GetPrototypeInfo() const ;
        Boss03Part(Boss03PartPrototypeInfo const &);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void SetDamageable(bool);
        static m3d::Class * GetBaseClass();
        virtual m3d::Class * GetClass() const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;

    protected:
        virtual ~Boss03Part();
        virtual bool _OnDurabilityValueBeforeApplyModifier(Modifier const &,float &);
        virtual void _OnDurabilityValueAfterChange(float);

    private:
        virtual m3d::Object * Clone();
        static m3d::Object * CreateObject();

    public:
        RT_CLASS_INLINE_DECLARE(Boss03Part);

    private:
        bool m_bIsDamageable;
    };
}
