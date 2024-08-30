#pragma once
#include "physicobj.h"

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
        void GetPartNames(std::vector<CStr, std::allocator<CStr> >&) const;
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
        std::vector<CStr> m_lpNames;
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
        void GetPartNames(std::vector<CStr,std::allocator<CStr> > &) const ;
        virtual ~ComplexPhysicObjPrototypeInfo();
        std::vector<CStr,std::allocator<CStr> > const & GetAllPartNames() const ;
        ComplexPhysicObjPrototypeInfo();
        virtual unsigned int GetBasePrice() const ;
        virtual void PostLoad();

    public:
        std::map<CStr,int> m_partPrototypeIds;
        CVector m_massSize;
        CVector m_massTranslation;
        ref_ptr<ComplexPhysicObjPartDescription> m_partDescription;
        std::map<CStr,CStr> m_partPrototypeNames;
        std::vector<CStr> m_allPartNames;
        MassShapes m_massShape;
    };

    class ComplexPhysicObj : public PhysicObj
    {
    public:
        virtual void UnlinkGeomsFromCollisionCells();
        virtual void GetGeoms(std::vector<Geom*, std::allocator<Geom*> >&) const;
        virtual void SetPassedToAnotherMapStatus();
        virtual void EnableGeometry(bool);
        virtual void RenderDebugInfo() const;
        std::vector<CStr, std::allocator<CStr> > GetAttachedPartNames() const;
        virtual void DumpPhysicInfo(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual void SetPartByName(CStr const&, VehiclePart*, bool);
        virtual void Remove();
        ComplexPhysicObj(ComplexPhysicObjPrototypeInfo const&);
        static m3d::Class* GetBaseClass();
        void PutContour();
        virtual bool IsVisible();
        virtual void SetSkin(int);
        virtual void CreateChildren();
        virtual void TransferPhysicParamsToSceneGraphNode();
        virtual void ClearSavedStatus();
        VehiclePart const* GetPartByName(CStr const&) const;
        VehiclePart* GetPartByName(CStr const&);
        virtual void SetVisible();
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void EnablePhysics();
        VehiclePart* TakeOffPart(CStr const&);
        virtual void AddChild(Obj*);
        virtual unsigned int GetPrice(IPriceCoeffProvider const*) const;
        virtual void ReceiveNodesToLink(std::list<m3d::SgNode*, std::allocator<m3d::SgNode*> >&) const;
        void RemoveContour();
        virtual void RelinkGeomsToCollisionCells();
        virtual ComplexPhysicObjPrototypeInfo const* GetPrototypeInfo() const;
        virtual void DisableGeometry(bool);
        void SetContourWidth(float);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        unsigned int size() const;
        virtual void TransferToSpace(dxSpace*);
        bool bIsContoured() const;
        virtual void DisablePhysics();
        virtual void LinkGeomsToCollisionCells();
        virtual void SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual Obj* CloneObj();
        virtual void SetRandomSkin();
        virtual void SetBelong(int);
        virtual void SetInvisible();
        virtual m3d::Class* GetClass() const;
        virtual Geom::CellAabb GetCollisionCellAabb() const;
        void SetContourColor(unsigned int);
        int GetNumPhysicBodies() const;
        virtual bool RemoveChild(Obj*);
        bool CanPartBeAttached(CStr const&) const;
        int GetGunHorizontalStopAngles(CStr const&, int, float&, float&) const;
        virtual void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void Blow(Obj*);
        virtual void Flow(Obj*, float);
        unsigned int GetRepairPrice() const;
        void RefreshMass();
        CVector GetSmoothTargetPointForObj(Obj const*, float);
        void FlowUnattachableParts(float);
        bool SetNewPart(CStr const&, CStr const&);
        virtual void RemoveComponent(Obj*);

    protected:
        void _DestroyHierarchy();
        virtual void _UnlinkBodyFromGeoms();
        virtual void _LinkBodyToGeoms();
        void _SetCorrectBoundSphereRadius();
        virtual void _RemoveContour();
        virtual void _InternalCreateVisualPart();
        virtual void _SetPositionToGeoms(CVector const&);
        virtual void _ConstructVehiclePart(CStr const&, VehiclePart*, int, bool);
        virtual float _CalcMassForBody() const;
        virtual ~ComplexPhysicObj();
        virtual void _PutContour();
        virtual void _SetRotationToGeoms(Quaternion const&);
        virtual void _Construct(bool);

    private:
        static m3d::Object* CreateObject();
        virtual m3d::Object* Clone();
        void _CreateSplinterFromSgNode(VehiclePart*, int, CVector const&, float, m3d::SgNode*, CollisionInfo const*);
        void _TearOffPart(VehiclePart*, float);

    public:
        RT_CLASS_DECLARE(ComplexPhysicObj);

    private:
        std::map<CStr, VehiclePart*> m_vehicleParts;
        unsigned int m_contourColor;
        float m_contourWidth;
        bool m_isContoured;
        int m_targetId;
        float m_timeoutForReAimGuns;
        CVector m_currentTargetPos;
    };
}
