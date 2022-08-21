#pragma once
#include "simplephysicobj.h"

namespace ai
{
    class Team;
    class Vehicle;

    class ObjPrefabPrototypeInfo : public SimplePhysicObjPrototypeInfo
    {
    public:
        class ObjInfo
        {
        public:
            ObjInfo();
            void PostLoad();

        private:
            int m_prototypeId;
            CVector m_relPos;
            Quaternion m_relRot;
            float m_scale;
            CStr m_modelName;
            CStr m_prototypeName;
        };

    public:
        ObjPrefabPrototypeInfo();
        virtual void PostLoad();
        virtual ai::Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        std::vector<ObjInfo> m_objInfos;
    };

    class ObjPrefab : public ai::SimplePhysicObj
    {
    public:
        virtual ai::ObjPrefabPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;
        virtual bool RemoveChild(ai::Obj*);
        virtual void SetPosition(CVector const&);
        ObjPrefab(ai::ObjPrefabPrototypeInfo const&);
        virtual void Remove();
        virtual void CreateChildren();
        virtual bool CanChildBeAdded(m3d::Class*) const;
        virtual void SetPositionSelf(CVector const&);
        virtual void SetRotation(Quaternion const&);
        virtual void AddChild(ai::Obj*);
        static m3d::Class* GetBaseClass();

    protected:
        virtual void _InternalPostLoad();
        virtual ~ObjPrefab();

    private:
        static m3d::Object* CreateObject();
        void AddTeam();
        void AddVehicleChild(Vehicle*);
        virtual m3d::Object* Clone();

    public:
        RT_CLASS_DECLARE(ObjPrefab);

    private:
        std::set<PhysicObj*> m_physicObjs;
        std::set<Obj*> m_otherChildren;
        Team* m_team;
        std::vector<int> m_VehiclesForAdd;
    };
}
