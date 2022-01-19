#pragma once
#include <server/objects/base/complexphysicobj.h>

namespace m3d {
    class CameraPath;
}

namespace ai
{
    class Boss04DronePrototypeInfo : public ComplexPhysicObjPrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
        Boss04DronePrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        float m_maxLinearVelocity;
    };

    class Boss04Drone :  public ComplexPhysicObj
    {
    public:
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        void SetCustomControl(bool);
        virtual void Update(float,unsigned int);
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        virtual void InflictDamage(DamageInfo const &);
        Boss04Drone(Boss04DronePrototypeInfo const &);
        virtual Boss04DronePrototypeInfo const * GetPrototypeInfo() const ;
        virtual m3d::Class * GetClass() const ;
        virtual CStr GetPropertyName(int) const ;
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void SetFlyPathName(CStr const &);
        virtual int GetPropertyId(char const *) const ;
        float GetMaxHealth() const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        static void __fastcall Registration();
        static m3d::Class * GetBaseClass();
        float GetHealth() const ;
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;

    protected:
        virtual ~Boss04Drone();
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;

    private:
        static m3d::Object * CreateObject();
        void _RecalcFlyPath();
        void _UpdateLookAtPlayer();
        virtual m3d::Object * Clone();

    public:
        RT_CLASS_INLINE_DECLARE(Boss04Drone);

    private:
        CStr m_flyPathName;
        m3d::CameraPath *m_currentFlyPath;
        float m_currentFlyTime;
        bool m_bCustomControl;
    };
}
