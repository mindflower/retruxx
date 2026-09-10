#pragma once
class WeaponSlotList;
#include "physicobj.h"

namespace ai
{
    class CollisionInfo;
    class VehiclePart;

    class ComplexPhysicObjPartDescription : public m3d::Object
    {
    protected:
        ComplexPhysicObjPartDescription();
        ComplexPhysicObjPartDescription(const ai::ComplexPhysicObjPartDescription& rhs);

    public:
        virtual  ~ComplexPhysicObjPartDescription() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x04 */;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x34 */;
        static m3d::Class m_classComplexPhysicObjPartDescription;
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        ai::ComplexPhysicObjPartDescription* GetParent() const;
        const ai::ComplexPhysicObjPartDescription* GetChildByNameDeep(const CStr& childName) const;
        int GetPartResourceId() const;
        void GetPartNames(retruxx::vector<CStr, retruxx::allocator<CStr> >& partNames) const;
        const CStr& GetLpName(unsigned int index) const;
        unsigned int GetNumLps() const;

    private:
        /* 0x0034 */ int m_partResourceId;
        /* 0x0038 */ retruxx::vector<CStr, retruxx::allocator<CStr> > m_lpNames;
    }; /* size: 0x0048 */

    static_assert(sizeof(ComplexPhysicObjPartDescription) == 0x0048);

    class ComplexPhysicObjPrototypeInfo : public ai::PhysicObjPrototypeInfo
    {
        // WeaponSlotList sorts gun slots by whether they hang off the cabin,
        // which means walking the root part description.
        friend class ::WeaponSlotList;

    public:
        ComplexPhysicObjPrototypeInfo();
        virtual  ~ComplexPhysicObjPrototypeInfo() override = 0 /* 0x00 */;
        /* 0x0048 */ retruxx::map<CStr, int> m_partPrototypeIds;
        /* 0x0054 */ CVector m_massSize;
        /* 0x0060 */ CVector m_massTranslation;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        const ai::ComplexPhysicObjPartDescription* GetPartDescriptionByName(const CStr& partName) const;
        void GetPartNames(retruxx::vector<CStr, retruxx::allocator<CStr> >& partNames) const;
        virtual unsigned int GetBasePrice() const override /* 0x00 */;
        virtual ai::Obj* CreateRandomTargetObject() const /* 0x1c */;
        const retruxx::vector<CStr, retruxx::allocator<CStr> >& GetAllPartNames() const;

        enum MassShapes
        {
            MS_BOX = 0,
            MS_SPHERE = 1,
        };

    public:
        ai::ComplexPhysicObjPrototypeInfo::MassShapes GetMassShape() const;

    private:
        /* 0x006c */ ref_ptr<ai::ComplexPhysicObjPartDescription> m_partDescription;

        using StrStrMap = retruxx::map<CStr, CStr, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, CStr> > >;
        class StrStrMapConstIterator;

    private:
        /* 0x0070 */ retruxx::map<CStr, CStr, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, CStr> > > m_partPrototypeNames;
        /* 0x007c */ retruxx::vector<CStr, retruxx::allocator<CStr> > m_allPartNames;
        /* 0x008c */ ai::ComplexPhysicObjPrototypeInfo::MassShapes m_massShape;
    }; /* size: 0x0090 */

    static_assert(sizeof(ComplexPhysicObjPrototypeInfo) == 0x0090);

    class ComplexPhysicObj : public PhysicObj
    {
        friend class WeaponFirer;
    protected:
        virtual ~ComplexPhysicObj() override /* 0x00 */;

    public:
    //private:
        ComplexPhysicObj(const ai::ComplexPhysicObjPrototypeInfo& prototypeInfo);
        ComplexPhysicObj(const ai::ComplexPhysicObj&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classComplexPhysicObj;
        virtual const ai::ComplexPhysicObjPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        virtual void SetBelong(int newBelong) override /* 0x00 */;
        virtual void Remove() override /* 0x54 */;
        virtual void SetPassedToAnotherMapStatus() override /* 0x50 */;
        virtual void CreateChildren() override /* 0x00 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x00 */;
        virtual bool RemoveChild(ai::Obj* pObj) override /* 0x00 */;
        virtual void RemoveComponent(ai::Obj* pComponent) override /* 0x00 */;
        retruxx::vector<CStr, retruxx::allocator<CStr> > GetAttachedPartNames() const;
        const ai::VehiclePart* GetPartByName(const CStr& partName) const;
        ai::VehiclePart* GetPartByName(const CStr& partName);
        bool CanPartBeAttached(const CStr& partName) const;
        virtual void SetPartByName(const CStr& partName, ai::VehiclePart* vehiclePart, bool bUnsafe) /* 0x1a4 */;
        virtual void ReceiveNodesToLink(retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*> >& nodelist) const override /* 0x00 */;
        virtual void TransferPhysicParamsToSceneGraphNode() override /* 0x00 */;
        int GetNumPhysicBodies() const;
        virtual void TransferToSpace(dxSpace* newSpace) override /* 0x178 */;
        virtual void RenderDebugInfo() const override /* 0xe4 */;
        virtual void DisableGeometry(bool changePhysicState) override /* 0x15c */;
        virtual void EnableGeometry(bool changePhysicState) override /* 0x160 */;
        virtual void LinkGeomsToCollisionCells() override /* 0x164 */;
        virtual void UnlinkGeomsFromCollisionCells() override /* 0x168 */;
        virtual void RelinkGeomsToCollisionCells() override /* 0x16c */;
        virtual ai::Geom::CellAabb GetCollisionCellAabb() const override /* 0x174 */;
        virtual void Flow(ai::Obj* partToFlow, float averageSpeed) /* 0x1a8 */;
        virtual void Blow(ai::Obj* partToBlow) /* 0x1ac */;
        void FlowUnattachableParts(float averageSpeed);
        bool SetNewPart(const CStr& partName, const CStr& newPartPrototypeName);
        ai::VehiclePart* TakeOffPart(const CStr& partName);
        virtual void SetSkin(int skin) override /* 0x184 */;
        virtual void SetRandomSkin() /* 0x1b0 */;
        virtual unsigned int GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const override /* 0x00 */;
        unsigned int GetRepairPrice() const;
        virtual void SetVisible() override /* 0xc8 */;
        virtual void SetInvisible() override /* 0xcc */;
        virtual void DisablePhysics() override /* 0x154 */;
        virtual void EnablePhysics() override /* 0x158 */;
        virtual void GetGeoms(retruxx::vector<ai::Geom*, retruxx::allocator<ai::Geom*> >& geoms) const /* 0x1b4 */;
        virtual ai::Obj* CloneObj() override /* 0x00 */;
        virtual void ClearSavedStatus() override /* 0x00 */;
        virtual void DumpPhysicInfo(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x1a0 */;
        void PutContour();
        void RemoveContour();
        bool bIsContoured() const;
        void SetContourColor(unsigned int color);
        void SetContourWidth(float width);
        int GetGunHorizontalStopAngles(const CStr& gunPartName, int index, float& leftStopAngle, float& rightStopAngle) const;
        void RefreshMass();
        virtual bool IsVisible() override /* 0x188 */;
        CVector GetSmoothTargetPointForObj(const ai::Obj* target, float elapsedTime);

        using VehiclePartsMap = retruxx::map<CStr, ai::VehiclePart*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::VehiclePart*> > >;

    protected:
        /* 0x0120 */ retruxx::map<CStr, ai::VehiclePart*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::VehiclePart*> > > m_vehicleParts;
        virtual void _InternalCreateVisualPart() override /* 0x100 */;
        virtual void _ConstructVehiclePart(const CStr& name, ai::VehiclePart* vehiclePart, int index, bool bForAnimation) /* 0x1b8 */;
        virtual void _Construct(bool bForAnimation) /* 0x1bc */;
        virtual void _UnlinkBodyFromGeoms() override /* 0x18c */;
        virtual void _LinkBodyToGeoms() override /* 0x190 */;
        virtual void _SetPositionToGeoms(const CVector& pos) override /* 0x194 */;
        virtual void _SetRotationToGeoms(const Quaternion& rot) override /* 0x198 */;
        virtual float _CalcMassForBody() const /* 0x1c0 */;
        void _SetCorrectBoundSphereRadius();
        void _DestroyHierarchy();
        virtual void _PutContour() /* 0x1c4 */;
        virtual void _RemoveContour() /* 0x1c8 */;
        /* 0x012c */ unsigned int m_contourColor;
        /* 0x0130 */ float m_contourWidth;

    private:
        /* 0x0134 */ bool m_isContoured;
        /* 0x0135 */ char Padding_89[3];
        /* 0x0138 */ int m_targetId;
        /* 0x013c */ float m_timeoutForReAimGuns;
        /* 0x0140 */ CVector m_currentTargetPos;
        void _CreateSplinterFromSgNode(ai::VehiclePart* vp, int splinterPrototypeId, const CVector& dir, float averageSpeed, m3d::SgNode* sgNode, const ai::CollisionInfo* collisionInfo);
        void _TearOffPart(ai::VehiclePart* vp, float averageSpeed);

        class iterator;
        class const_iterator;

    public:
        VehiclePartsMap::const_iterator begin() const;
        VehiclePartsMap::iterator begin();
        VehiclePartsMap::const_iterator end() const;
        VehiclePartsMap::iterator end();
        unsigned int size() const;
    }; /* size: 0x014c */

    static_assert(sizeof(ComplexPhysicObj) == 0x014c);
}
