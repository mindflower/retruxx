#pragma once
#include "base/complexphysicobj.h"
#include <server/components/numericinrange.h>
#include <server/components/numericinrangeregenerating.h>

namespace ai
{
    class Gun;

    class StaticAutoGunPrototypeInfo : public ComplexPhysicObjPrototypeInfo
    {
    public:
        StaticAutoGunPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    protected:
        virtual void _InternalCopyFrom(PrototypeInfo const&);

    private:
        float m_maxHealth;
        CStr m_destroyedModelName;
    };

    class StaticAutoGun :  public ComplexPhysicObj
    {
    public:
        void WeaponLookAtPoint(CVector const &,float);
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual bool RemoveChild(Obj *);
        virtual m3d::Class * GetClass() const ;
        NumericInRange<float> const & Health() const ;
        NumericInRange<float> & Health();
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual CStr GetPropertyName(int) const ;
        virtual StaticAutoGunPrototypeInfo const * GetPrototypeInfo() const ;
        virtual int GetPropertyId(char const *) const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void RenderDebugInfo() const ;
        static void __fastcall Registration();
        bool PointIsReachableForGun(CVector const &,std::vector<int,std::allocator<int> > const &);
        Gun * getGun();
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        virtual void InflictDamage(DamageInfo const &);
        void FireFromWeapon(bool);
        virtual void SetPositionSelf(CVector const &);
        virtual bool ApplyModifier(Modifier const &);
        virtual void Update(float,unsigned int);
        StaticAutoGun(StaticAutoGunPrototypeInfo const &);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        static m3d::Class * GetBaseClass();
        float GetMaxFiringRangeAI() const ;
        virtual void AddChild(Obj *);
        virtual bool SetPropertyById(int,m3d::AIParam const &);

    protected:
        virtual void _InternalPostLoad();
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual void _Construct(bool);
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual ~StaticAutoGun();
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;

    private:
        static m3d::Object * CreateObject();
        bool _OnHealthValueBeforeApplyModifier(Modifier const &,float &);
        //MemberFunctionOneArg<StaticAutoGun,float,void>::MemberFunctionOneArg<StaticAutoGun,float,void>(StaticAutoGun &,void (*const)(float));
        void _OnHealthValueAfterChange(float);
        virtual m3d::Object * Clone();

    private:
        //MemberFunctionTwoArgsRef<StaticAutoGun,Modifier,float,bool>::MemberFunctionTwoArgsRef<StaticAutoGun,Modifier,float,bool>(StaticAutoGun &,bool (*const)(Modifier const &,float &));
        NumericInRange<float> m_health;
        NumericInRangeRegenerating<float> m_timeForNextCheck;
        CStr m_destroyedModelName;
        std::set<m3d::Class *> m_targetClasses;
        int m_currentEnemyId;
    };
}
