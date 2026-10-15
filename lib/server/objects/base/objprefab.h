#pragma once
#include "simplephysicobj.h"

namespace ai
{
    class Team;
    class Vehicle;

    class ObjPrefabPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    public:
        struct ObjInfo
        {
            int m_prototypeId;
            CVector m_relPos;
            Quaternion m_relRot;
            float m_scale;
            CStr m_modelName;
            ObjInfo(ai::ObjPrefabPrototypeInfo::ObjInfo const& __that);
            ObjInfo();
            void PostLoad();
            CStr m_prototypeName;
        };

        using ObjInfoVector = retruxx::
            vector<ai::ObjPrefabPrototypeInfo::ObjInfo, retruxx::allocator<ai::ObjPrefabPrototypeInfo::ObjInfo>>;

    public:
        ObjPrefabPrototypeInfo();
        retruxx::vector<ai::ObjPrefabPrototypeInfo::ObjInfo, retruxx::allocator<ai::ObjPrefabPrototypeInfo::ObjInfo>>
            m_objInfos;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        virtual void PostLoad() override;
    };

    class ObjPrefab : public ai::SimplePhysicObj
    {
        // CreateTargetObject builds one of these.
        friend class ObjPrefabPrototypeInfo;

    protected:
        virtual ~ObjPrefab() override;

    protected:
        ObjPrefab(ai::ObjPrefabPrototypeInfo const& prototypeInfo);
        ObjPrefab(ai::ObjPrefab const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classObjPrefab;
        virtual ai::ObjPrefabPrototypeInfo const* GetPrototypeInfo() const override;

        using PhysicObjSet =
            retruxx::set<ai::PhysicObj*, retruxx::less<ai::PhysicObj*>, retruxx::allocator<ai::PhysicObj*>>;
        using IdsVector = retruxx::vector<int, retruxx::allocator<int>>;

    public:
        virtual void Remove() override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pChild) override;
        virtual void SetPositionSelf(CVector const& pos) override;
        virtual void SetPosition(CVector const& pos) override;
        virtual void SetRotation(Quaternion const& rot) override;
        virtual void CreateChildren() override;

    protected:
        virtual void _InternalPostLoad() override;

    private:
        retruxx::set<ai::PhysicObj*, retruxx::less<ai::PhysicObj*>, retruxx::allocator<ai::PhysicObj*>> m_physicObjs;
        retruxx::set<ai::Obj*, retruxx::less<ai::Obj*>, retruxx::allocator<ai::Obj*>> m_otherChildren;
        ai::Team* m_team;
        retruxx::vector<int, retruxx::allocator<int>> m_VehiclesForAdd;
        void AddVehicleChild(ai::Vehicle* obj);
        void AddTeam();
    };
}  // namespace ai
