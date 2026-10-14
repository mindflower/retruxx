#pragma once
#include "physicobj.h"
#include "server/objects/physicbodies/simplephysicbody.h"

namespace ai
{
    class SimplePhysicObjPrototypeInfo : public ai::PhysicObjPrototypeInfo
    {
    public:
        SimplePhysicObjPrototypeInfo();
        virtual ~SimplePhysicObjPrototypeInfo() override;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::SimplePhysicBody* CreatePhysicBody() const;
        CVector GetSize() const;
        float GetRadius() const;
        CStr const& GetEngineModelName() const;
        float GetMassValue() const;
        retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo>> m_collisionInfos;

    protected:
        bool m_bCollisionTrimeshAllowed;
        void _SetGeomType(ai::GeomType geomType);

    private:
        ai::GeomType m_geomType;
        CStr m_engineModelName;
        CVector m_size;
        float m_radius;
        float m_massValue;
    };

    class SimplePhysicObj : public ai::PhysicObj
    {
    protected:
        virtual ~SimplePhysicObj() override;

    protected:
        SimplePhysicObj(ai::SimplePhysicObjPrototypeInfo const& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classSimplePhysicObj;
        virtual ai::SimplePhysicObjPrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(char const* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(
            retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const override;
        virtual void GetPropertiesIDs(
            retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, m3d::AIParam const& newValue) override;
        virtual int GetPropertyId(char const* PropertyName) const override;

    protected:
        static inline retruxx::
            map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int>>>
                m_propertiesMap;
        static inline retruxx::map<
            int,
            enum ai::eGObjPropertySaveStatus,
            retruxx::less<int>,
            retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus>>>
            m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        virtual void Remove() override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SetBelong(int newBelong) override;
        ai::SimplePhysicBody* GetPhysicBody();
        ai::SimplePhysicBody const* GetPhysicBody() const;
        virtual CVector GetGeometricCenter() const override;
        virtual void ReceiveNodesToLink(
            retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*>>& nodelist) const override;
        virtual void TransferPhysicParamsToSceneGraphNode() override;
        virtual void RelinkSceneGraphNode() override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void TransferToSpace(dxSpace* newSpace) override;
        virtual void RenderDebugInfo() const override;
        virtual void DisableGeometry(bool changePhysicState) override;
        virtual void EnableGeometry(bool changePhysicState) override;
        virtual void LinkGeomsToCollisionCells() override;
        virtual void UnlinkGeomsFromCollisionCells() override;
        virtual void RelinkGeomsToCollisionCells() override;
        virtual ai::Geom::CellAabb GetCollisionCellAabb() const override;
        void SetMass(float newMassValue);
        virtual void SetPassedToAnotherMapStatus() override;
        virtual void SetSkin(int skin) override;
        static void __fastcall Registration();
        virtual void SetVisible() override;
        virtual void SetInvisible() override;
        void SetScale(float scale, bool recalcMass);
        float GetScale();
        virtual void SetDeadTimer(int resttime, bool testVisibility);
        bool bDeadTimerActive();
        virtual void SetNodeAction(int action, bool forceRestartAction);
        virtual void SetNextForAnimation(int action, int nextAction);
        virtual bool IsVisible() override;

    protected:
        virtual void _Construct();
        virtual void _InternalPostLoad() override;
        virtual void _InternalCreateVisualPart() override;
        virtual void _UnlinkBodyFromGeoms() override;
        virtual void _LinkBodyToGeoms() override;
        virtual void _SetPositionToGeoms(CVector const& pos) override;
        virtual void _SetRotationToGeoms(Quaternion const& rot) override;
        void _UpdatePhysicBodyByCollisionInfo(
            retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo>> const& collisionInfos);
        void _UpdateFullPhysicBodyByCollisionInfo(
            retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo>> const& collisionInfos);
        void _UpdateCollisionInfoFromPhysicBody();
        ai::SimplePhysicBody* m_physicBody;
        retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo>> m_collisionInfos;
        float m_scale;
        bool m_deadTimerActive;
        float m_deadTimer;
        bool m_testVisibility;
    };
}  // namespace ai
