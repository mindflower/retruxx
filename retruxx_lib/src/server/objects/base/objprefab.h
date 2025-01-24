#pragma once
#include "simplephysicobj.h"

namespace ai
{
    class Team;
    class Vehicle;

    class ObjPrefabPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    public:
        struct ObjInfo;
        using ObjInfoVector = retruxx::vector<ai::ObjPrefabPrototypeInfo::ObjInfo, retruxx::allocator<ai::ObjPrefabPrototypeInfo::ObjInfo> >;

    public:
        ObjPrefabPrototypeInfo();
        /* 0x0080 */ retruxx::vector<ai::ObjPrefabPrototypeInfo::ObjInfo, retruxx::allocator<ai::ObjPrefabPrototypeInfo::ObjInfo> > m_objInfos;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
        virtual void PostLoad() override /* 0x00 */;
    }; /* size: 0x0090 */

    static_assert(sizeof(ObjPrefabPrototypeInfo) == 0x0090);

    class ObjPrefab : public ai::SimplePhysicObj
    {
    protected:
        virtual  ~ObjPrefab() override /* 0x00 */;

    protected:
        ObjPrefab(const ai::ObjPrefabPrototypeInfo& prototypeInfo);
        ObjPrefab(const ai::ObjPrefab&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classObjPrefab;
        virtual const ai::ObjPrefabPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

        using PhysicObjSet = retruxx::set<ai::PhysicObj*, retruxx::less<ai::PhysicObj*>, retruxx::allocator<ai::PhysicObj*> >;
        using IdsVector = retruxx::vector<int, retruxx::allocator<int> >;

    public:
        virtual void Remove() override /* 0x00 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x00 */;
        virtual bool RemoveChild(ai::Obj* pChild) override /* 0x00 */;
        virtual void SetPositionSelf(const CVector& pos) override /* 0x00 */;
        virtual void SetPosition(const CVector& pos) override /* 0x00 */;
        virtual void SetRotation(const Quaternion& rot) override /* 0x00 */;
        virtual void CreateChildren() override /* 0x00 */;

    protected:
        virtual void _InternalPostLoad() override /* 0x00 */;

    private:
        /* 0x0144 */ retruxx::set<ai::PhysicObj*, retruxx::less<ai::PhysicObj*>, retruxx::allocator<ai::PhysicObj*> > m_physicObjs;
        /* 0x0150 */ retruxx::set<ai::Obj*, retruxx::less<ai::Obj*>, retruxx::allocator<ai::Obj*> > m_otherChildren;
        /* 0x015c */ ai::Team* m_team;
        /* 0x0160 */ retruxx::vector<int, retruxx::allocator<int> > m_VehiclesForAdd;
        void AddVehicleChild(ai::Vehicle* obj);
        void AddTeam();
    }; /* size: 0x0170 */

    static_assert(sizeof(ObjPrefab) == 0x0170);
}
