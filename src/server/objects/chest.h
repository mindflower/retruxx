#pragma once
#include "base/simplephysicobj.h"

namespace ai
{
    class ChestPrototypeInfo : public SimplePhysicObjPrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
        ChestPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        float m_LifeTime;
        bool m_WithLifeTime;
    };

    class Chest :  public SimplePhysicObj
    {
    public:
        virtual void RenderDebugInfo() const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        virtual ChestPrototypeInfo const * GetPrototypeInfo() const ;
        virtual m3d::Class * GetClass() const ;
        virtual bool RemoveChild(Obj *);
        virtual void AddChild(Obj *);
        void FillFromGroundRepository(GeomRepository *,bool);
        Chest(ChestPrototypeInfo const &);
        static m3d::Class * GetBaseClass();
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual void Update(float,unsigned int);
        GeomRepository * GetRepository();
        bool RemoveChildUnsafe(Obj *);
        virtual void SetPositionSelf(CVector const &);

    protected:
        virtual ~Chest();

    private:
        virtual m3d::Object * Clone();
        static m3d::Object * CreateObject();

    private:
        GeomRepository *m_repository;
        float m_LifeTime;
    };
}
