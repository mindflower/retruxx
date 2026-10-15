#pragma once
class WeaponSlotList;
#include "physicobj.h"

namespace ai
{
    struct CollisionInfo;
    class VehiclePart;
    class Geom;

    class ComplexPhysicObjPartDescription : public m3d::Object
    {
    protected:
        ComplexPhysicObjPartDescription();
        ComplexPhysicObjPartDescription(ai::ComplexPhysicObjPartDescription const& rhs);

    public:
        virtual ~ComplexPhysicObjPartDescription() override;
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classComplexPhysicObjPartDescription;
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        ai::ComplexPhysicObjPartDescription* GetParent() const;
        ai::ComplexPhysicObjPartDescription const* GetChildByNameDeep(CStr const& childName) const;
        int GetPartResourceId() const;
        void GetPartNames(retruxx::vector<CStr, retruxx::allocator<CStr>>& partNames) const;
        CStr const& GetLpName(unsigned int index) const;
        unsigned int GetNumLps() const;

    private:
        int m_partResourceId;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_lpNames;
    };

    class ComplexPhysicObjPrototypeInfo : public ai::PhysicObjPrototypeInfo
    {
        // WeaponSlotList sorts gun slots by whether they hang off the cabin,
        // which means walking the root part description.
        friend class ::WeaponSlotList;

    public:
        ComplexPhysicObjPrototypeInfo();
        virtual ~ComplexPhysicObjPrototypeInfo() override = 0;
        retruxx::map<CStr, int> m_partPrototypeIds;
        CVector m_massSize;
        CVector m_massTranslation;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        ai::ComplexPhysicObjPartDescription const* GetPartDescriptionByName(CStr const& partName) const;
        void GetPartNames(retruxx::vector<CStr, retruxx::allocator<CStr>>& partNames) const;
        virtual unsigned int GetBasePrice() const override;
        virtual ai::Obj* CreateRandomTargetObject() const;
        retruxx::vector<CStr, retruxx::allocator<CStr>> const& GetAllPartNames() const;

        enum MassShapes
        {
            MS_BOX = 0,
            MS_SPHERE = 1,
        };

    public:
        ai::ComplexPhysicObjPrototypeInfo::MassShapes GetMassShape() const;

    private:
        ref_ptr<ai::ComplexPhysicObjPartDescription> m_partDescription;

        using StrStrMap =
            retruxx::map<CStr, CStr, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, CStr>>>;
        class StrStrMapConstIterator;

    private:
        retruxx::map<CStr, CStr, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, CStr>>>
            m_partPrototypeNames;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_allPartNames;
        ai::ComplexPhysicObjPrototypeInfo::MassShapes m_massShape;
    };

    class ComplexPhysicObj : public PhysicObj
    {
        friend class WeaponFirer;

    protected:
        virtual ~ComplexPhysicObj() override;

    public:
        //private:
        ComplexPhysicObj(ai::ComplexPhysicObjPrototypeInfo const& prototypeInfo);
        ComplexPhysicObj(ai::ComplexPhysicObj const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classComplexPhysicObj;
        virtual ai::ComplexPhysicObjPrototypeInfo const* GetPrototypeInfo() const override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SetBelong(int newBelong) override;
        virtual void Remove() override;
        virtual void SetPassedToAnotherMapStatus() override;
        virtual void CreateChildren() override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pObj) override;
        virtual void RemoveComponent(ai::Obj* pComponent) override;
        retruxx::vector<CStr, retruxx::allocator<CStr>> GetAttachedPartNames() const;
        ai::VehiclePart const* GetPartByName(CStr const& partName) const;
        ai::VehiclePart* GetPartByName(CStr const& partName);
        bool CanPartBeAttached(CStr const& partName) const;
        virtual void SetPartByName(CStr const& partName, ai::VehiclePart* vehiclePart, bool bUnsafe);
        virtual void ReceiveNodesToLink(
            retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*>>& nodelist) const override;
        virtual void TransferPhysicParamsToSceneGraphNode() override;
        int GetNumPhysicBodies() const;
        virtual void TransferToSpace(dxSpace* newSpace) override;
        virtual void RenderDebugInfo() const override;
        virtual void DisableGeometry(bool changePhysicState) override;
        virtual void EnableGeometry(bool changePhysicState) override;
        virtual void LinkGeomsToCollisionCells() override;
        virtual void UnlinkGeomsFromCollisionCells() override;
        virtual void RelinkGeomsToCollisionCells() override;
        virtual ai::Geom::CellAabb GetCollisionCellAabb() const override;
        virtual void Flow(ai::Obj* partToFlow, float averageSpeed);
        virtual void Blow(ai::Obj* partToBlow);
        void FlowUnattachableParts(float averageSpeed);
        bool SetNewPart(CStr const& partName, CStr const& newPartPrototypeName);
        ai::VehiclePart* TakeOffPart(CStr const& partName);
        virtual void SetSkin(int skin) override;
        virtual void SetRandomSkin();
        virtual unsigned int GetPrice(ai::IPriceCoeffProvider const* priceCoeffProvider) const override;
        unsigned int GetRepairPrice() const;
        virtual void SetVisible() override;
        virtual void SetInvisible() override;
        virtual void DisablePhysics() override;
        virtual void EnablePhysics() override;
        virtual void GetGeoms(retruxx::vector<ai::Geom*, retruxx::allocator<ai::Geom*>>& geoms) const;
        virtual ai::Obj* CloneObj() override;
        virtual void ClearSavedStatus() override;
        virtual void DumpPhysicInfo(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        void PutContour();
        void RemoveContour();
        bool bIsContoured() const;
        void SetContourColor(unsigned int color);
        void SetContourWidth(float width);
        int GetGunHorizontalStopAngles(CStr const& gunPartName, int index, float& leftStopAngle, float& rightStopAngle)
            const;
        void RefreshMass();
        virtual bool IsVisible() override;
        CVector GetSmoothTargetPointForObj(ai::Obj const* target, float elapsedTime);

        using VehiclePartsMap = retruxx::map<
            CStr,
            ai::VehiclePart*,
            retruxx::less<CStr>,
            retruxx::allocator<retruxx::pair<CStr const, ai::VehiclePart*>>>;

    protected:
        // ai::CollideGeom walks every part's geometry directly, exactly as the
        // shipped code does.
        friend bool CollideGeom(ai::Geom const&, bool, bool, bool, bool);
        // Workshop::GetObjectRepairPrice adds up the repair price of every part.
        friend class Workshop;
        // BlastWave shares an explosion's damage and push over all of a vehicle's parts.
        friend class BlastWave;
        // Reads the parts of its stations.
        friend class Boss04;
        // Obstacle::RenderDebugInfo checks the first part's node.
        friend class Obstacle;
        retruxx::map<
            CStr,
            ai::VehiclePart*,
            retruxx::less<CStr>,
            retruxx::allocator<retruxx::pair<CStr const, ai::VehiclePart*>>>
            m_vehicleParts;
        virtual void _InternalCreateVisualPart() override;
        virtual void
            _ConstructVehiclePart(CStr const& name, ai::VehiclePart* vehiclePart, int index, bool bForAnimation);
        virtual void _Construct(bool bForAnimation);
        virtual void _UnlinkBodyFromGeoms() override;
        virtual void _LinkBodyToGeoms() override;
        virtual void _SetPositionToGeoms(CVector const& pos) override;
        virtual void _SetRotationToGeoms(Quaternion const& rot) override;
        virtual float _CalcMassForBody() const;
        void _SetCorrectBoundSphereRadius();
        void _DestroyHierarchy();
        virtual void _PutContour();
        virtual void _RemoveContour();
        unsigned int m_contourColor;
        float m_contourWidth;

    private:
        bool m_isContoured;
        int m_targetId;
        float m_timeoutForReAimGuns;
        CVector m_currentTargetPos;
        void _CreateSplinterFromSgNode(
            ai::VehiclePart* vp,
            int splinterPrototypeId,
            CVector const& dir,
            float averageSpeed,
            m3d::SgNode* sgNode,
            ai::CollisionInfo const* collisionInfo);
        void _TearOffPart(ai::VehiclePart* vp, float averageSpeed);

        class iterator;
        class const_iterator;

    public:
        VehiclePartsMap::const_iterator begin() const;
        VehiclePartsMap::iterator begin();
        VehiclePartsMap::const_iterator end() const;
        VehiclePartsMap::iterator end();
        unsigned int size() const;
    };
}  // namespace ai
