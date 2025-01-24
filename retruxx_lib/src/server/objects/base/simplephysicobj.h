#pragma once
#include "physicobj.h"

namespace ai
{
	class CollisionInfo;
	class SimplePhysicBody;

    class SimplePhysicObjPrototypeInfo : public ai::PhysicObjPrototypeInfo
    {
    public:
        SimplePhysicObjPrototypeInfo();
        virtual  ~SimplePhysicObjPrototypeInfo() override /* 0x00 */;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual ai::SimplePhysicBody* CreatePhysicBody() const /* 0x1c */;
        CVector GetSize() const;
        float GetRadius() const;
        const CStr& GetEngineModelName() const;
        float GetMassValue() const;
        /* 0x0048 */ retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo> > m_collisionInfos;

    protected:
        /* 0x0058 */ bool m_bCollisionTrimeshAllowed;
        void _SetGeomType(ai::GeomType geomType);
        /* 0x0059 */ char Padding_57[3];

    private:
        /* 0x005c */ ai::GeomType m_geomType;
        /* 0x0060 */ CStr m_engineModelName;
        /* 0x006c */ CVector m_size;
        /* 0x0078 */ float m_radius;
        /* 0x007c */ float m_massValue;
    }; /* size: 0x0080 */

    static_assert(sizeof(SimplePhysicObjPrototypeInfo) == 0x0080);

    class SimplePhysicObj : public ai::PhysicObj
    {
    protected:
        virtual  ~SimplePhysicObj() override /* 0x00 */;

    protected:
        SimplePhysicObj(const ai::SimplePhysicObjPrototypeInfo& prototypeInfo);
        SimplePhysicObj(const ai::SimplePhysicObj&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classSimplePhysicObj;
        virtual const ai::SimplePhysicObjPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x00 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x00 */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x00 */;
        virtual CStr GetPropertyName(int id) const override /* 0x00 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x00 */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x00 */;

    protected:
        static retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;

    public:
        virtual void Remove() override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void SetBelong(int newBelong) override /* 0x00 */;
        ai::SimplePhysicBody* GetPhysicBody();
        const ai::SimplePhysicBody* GetPhysicBody() const;
        virtual CVector GetGeometricCenter() const override /* 0x00 */;
        virtual void ReceiveNodesToLink(retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*> >& nodelist) const override /* 0x00 */;
        virtual void TransferPhysicParamsToSceneGraphNode() override /* 0x00 */;
        virtual void RelinkSceneGraphNode() override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        virtual void TransferToSpace(dxSpace* newSpace) override /* 0x00 */;
        virtual void RenderDebugInfo() const override /* 0x00 */;
        virtual void DisableGeometry(bool changePhysicState) override /* 0x00 */;
        virtual void EnableGeometry(bool changePhysicState) override /* 0x00 */;
        virtual void LinkGeomsToCollisionCells() override /* 0x00 */;
        virtual void UnlinkGeomsFromCollisionCells() override /* 0x00 */;
        virtual void RelinkGeomsToCollisionCells() override /* 0x00 */;
        virtual ai::Geom::CellAabb GetCollisionCellAabb() const override /* 0x00 */;
        void SetMass(float newMassValue);
        virtual void SetPassedToAnotherMapStatus() override /* 0x00 */;
        virtual void SetSkin(int skin) override /* 0x00 */;
        static void __fastcall Registration();
        virtual void SetVisible() override /* 0x00 */;
        virtual void SetInvisible() override /* 0x00 */;
        void SetScale(float scale, bool recalcMass);
        float GetScale();
        virtual void SetDeadTimer(int resttime, bool testVisibility) /* 0x1a4 */;
        bool bDeadTimerActive();
        virtual void SetNodeAction(int action, bool forceRestartAction) /* 0x1a8 */;
        virtual void SetNextForAnimation(int action, int nextAction) /* 0x1ac */;
        virtual bool IsVisible() override /* 0x00 */;

    protected:
        virtual void _Construct() /* 0x1b0 */;
        virtual void _InternalPostLoad() override /* 0x00 */;
        virtual void _InternalCreateVisualPart() override /* 0x00 */;
        virtual void _UnlinkBodyFromGeoms() override /* 0x00 */;
        virtual void _LinkBodyToGeoms() override /* 0x00 */;
        virtual void _SetPositionToGeoms(const CVector& pos) override /* 0x00 */;
        virtual void _SetRotationToGeoms(const Quaternion& rot) override /* 0x00 */;
        void _UpdatePhysicBodyByCollisionInfo(const retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo> >& collisionInfos);
        void _UpdateFullPhysicBodyByCollisionInfo(const retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo> >& collisionInfos);
        void _UpdateCollisionInfoFromPhysicBody();
        /* 0x0120 */ ai::SimplePhysicBody* m_physicBody;
        /* 0x0124 */ retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo> > m_collisionInfos;
        /* 0x0134 */ float m_scale;
        /* 0x0138 */ bool m_deadTimerActive;
        /* 0x0139 */ char Padding_12[3];
        /* 0x013c */ float m_deadTimer;
        /* 0x0140 */ bool m_testVisibility;
    }; /* size: 0x0144 */

    static_assert(sizeof(SimplePhysicObj) == 0x0144);
}
