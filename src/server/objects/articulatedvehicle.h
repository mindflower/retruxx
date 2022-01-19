#pragma once
#include "vehicle.h"

namespace ai
{
    class ArticulatedVehiclePrototypeInfo : public VehiclePrototypeInfo
    {
    public:
        virtual void PostLoad();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        ArticulatedVehiclePrototypeInfo();
        virtual Obj* CreateTargetObject() const;

    protected:
        virtual void _InternalCopyFrom(PrototypeInfo const&);

    private:
        int m_trailerPrototypeId;
        CStr m_trailerPrototypeName;
    };

    class ArticulatedVehicle :  public Vehicle
    {
    public:
        virtual CStr GetPropertyName(int) const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        ArticulatedVehicle(ArticulatedVehiclePrototypeInfo const &);
        virtual void CreateChildren();
        virtual void Remove();
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual m3d::Class * GetClass() const ;
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual void SetPassedToAnotherMapStatus();
        virtual ArticulatedVehiclePrototypeInfo const * GetPrototypeInfo() const ;
        virtual bool RemoveChild(Obj *);
        virtual void AddChild(Obj *);
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual void SetPositionSelf(CVector const &);
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual int GetPropertyId(char const *) const ;
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        virtual void SetRotationSelf(Quaternion const &);
        virtual void Update(float,unsigned int);
        static m3d::Class * GetBaseClass();

    protected:
        virtual void _InternalPostLoad();
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual ~ArticulatedVehicle();
        virtual void _KeepSteer(float);
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;

    private:
        CStr _GetTrailerName() const ;
        virtual m3d::Object * Clone();
        void _AdjustTrailerPosition();
        static m3d::Object * CreateObject();
        Vehicle * _GetTrailer() const ;

    public:
        RT_CLASS_INLINE_DECLARE(ArticulatedVehicle);

    private:
        int m_trailerObjId;
        dxJoint *m_trailerJoint;
        CVector m_relJointPosOnMe;
        CVector m_relJointPosOnTrailer;
    };
}
