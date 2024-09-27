#pragma once
#include "physicobj.h"
#include <map>

namespace ai
{
    class CollisionInfo;
    class VehiclePart;

    class ComplexPhysicObjPartDescription : public m3d::Object
    {
    public:
        virtual m3d::Class* GetClass() const;
        static m3d::Object* CreateObject();
        int GetPartResourceId() const;
        unsigned int GetNumLps() const;
        void GetPartNames(oldstd::vector<CStr, oldstd::allocator<CStr> >&) const;
        void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        ComplexPhysicObjPartDescription const* GetChildByNameDeep(CStr const&) const;
        CStr const& GetLpName(unsigned int) const;
        virtual m3d::Object* Clone();
        ComplexPhysicObjPartDescription* GetParent() const;
        virtual ~ComplexPhysicObjPartDescription();
        static m3d::Class* GetBaseClass();

    protected:
        ComplexPhysicObjPartDescription();
        ComplexPhysicObjPartDescription(ComplexPhysicObjPartDescription const&);

    public:
        RT_CLASS_DECLARE(ComplexPhysicObjPartDescription);

    private:
        int m_partResourceId;
        oldstd::vector<CStr> m_lpNames;
    };

    class ComplexPhysicObjPrototypeInfo :  public PhysicObjPrototypeInfo
    {
    public:
        enum MassShapes
        {
            MS_BOX = 0x0,
            MS_SPHERE = 0x1,
        };

    public:
        MassShapes GetMassShape() const ;
        ComplexPhysicObjPartDescription const * GetPartDescriptionByName(CStr const &) const ;
        virtual Obj * CreateRandomTargetObject() const ;
        virtual bool LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void GetPartNames(oldstd::vector<CStr,oldstd::allocator<CStr> > &) const ;
        virtual ~ComplexPhysicObjPrototypeInfo();
        oldstd::vector<CStr,oldstd::allocator<CStr> > const & GetAllPartNames() const ;
        ComplexPhysicObjPrototypeInfo();
        virtual unsigned int GetBasePrice() const ;
        virtual void PostLoad();

    public:
        oldstd::map<CStr,int> m_partPrototypeIds;
        CVector m_massSize;
        CVector m_massTranslation;
        ref_ptr<ComplexPhysicObjPartDescription> m_partDescription;
        oldstd::map<CStr,CStr> m_partPrototypeNames;
        oldstd::vector<CStr> m_allPartNames;
        MassShapes m_massShape;
    };

    class ComplexPhysicObj : public PhysicObj
    {
    protected:
        virtual ~ComplexPhysicObj() override /* 0x00 */;

    public:
    //private:
        ComplexPhysicObj(const ai::ComplexPhysicObjPrototypeInfo& prototypeInfo);
        ComplexPhysicObj(const ai::ComplexPhysicObj&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
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
        oldstd::vector<CStr, oldstd::allocator<CStr> > GetAttachedPartNames() const;
        const ai::VehiclePart* GetPartByName(const CStr& partName) const;
        ai::VehiclePart* GetPartByName(const CStr& partName);
        bool CanPartBeAttached(const CStr& partName) const;
        virtual void SetPartByName(const CStr& partName, ai::VehiclePart* vehiclePart, bool bUnsafe) /* 0x1a4 */;
        virtual void ReceiveNodesToLink(oldstd::list<m3d::SgNode*, oldstd::allocator<m3d::SgNode*> >& nodelist) const override /* 0x00 */;
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
        virtual void GetGeoms(oldstd::vector<ai::Geom*, oldstd::allocator<ai::Geom*> >& geoms) const /* 0x1b4 */;
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

        using VehiclePartsMap = oldstd::map<CStr, ai::VehiclePart*, oldstd::less<CStr>, oldstd::allocator<oldstd::pair<CStr const, ai::VehiclePart*> > >;

    protected:
        /* 0x0120 */ oldstd::map<CStr, ai::VehiclePart*, oldstd::less<CStr>, oldstd::allocator<oldstd::pair<CStr const, ai::VehiclePart*> > > m_vehicleParts;
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
        oldstd::_Tree<oldstd::_Tmap_traits<CStr, ai::VehiclePart*, oldstd::less<CStr>, oldstd::allocator<oldstd::pair<CStr const, ai::VehiclePart*> >, 0> >::const_iterator begin() const;
        oldstd::_Tree<oldstd::_Tmap_traits<CStr, ai::VehiclePart*, oldstd::less<CStr>, oldstd::allocator<oldstd::pair<CStr const, ai::VehiclePart*> >, 0> >::iterator begin();
        oldstd::_Tree<oldstd::_Tmap_traits<CStr, ai::VehiclePart*, oldstd::less<CStr>, oldstd::allocator<oldstd::pair<CStr const, ai::VehiclePart*> >, 0> >::const_iterator end() const;
        oldstd::_Tree<oldstd::_Tmap_traits<CStr, ai::VehiclePart*, oldstd::less<CStr>, oldstd::allocator<oldstd::pair<CStr const, ai::VehiclePart*> >, 0> >::iterator end();
        unsigned int size() const;
    }; /* size: 0x014c */

    static_assert(sizeof(ComplexPhysicObj) == 0x014c);
}
