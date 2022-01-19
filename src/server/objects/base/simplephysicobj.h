#pragma once
#include "physicobj.h"

namespace ai
{
    class CollisionInfo;
    class SimplePhysicBody;

    class SimplePhysicObjPrototypeInfo : public PhysicObjPrototypeInfo
    {
    public:
        virtual ~SimplePhysicObjPrototypeInfo();
        CStr const& GetEngineModelName() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        SimplePhysicObjPrototypeInfo();
        float GetRadius() const;
        virtual SimplePhysicBody* CreatePhysicBody() const;
        virtual void RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        CVector GetSize() const;
        float GetMassValue() const;

    protected:
        void _SetGeomType(GeomType);

    private:
        std::vector<CollisionInfo> m_collisionInfos;
        bool m_bCollisionTrimeshAllowed;
        GeomType m_geomType;
        CStr m_engineModelName;
        CVector m_size;
        float m_radius;
        float m_massValue;
    };

    class SimplePhysicObj :  public PhysicObj
    {
    public:
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        static void __fastcall Registration();
        virtual void SetPassedToAnotherMapStatus();
        SimplePhysicObj(SimplePhysicObjPrototypeInfo const &);
        virtual int GetPropertyId(char const *) const ;
        virtual void LinkGeomsToCollisionCells();
        virtual void DisableGeometry(bool);
        SimplePhysicBody * GetPhysicBody();
        SimplePhysicBody const * GetPhysicBody() const ;
        void SetMass(float);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual void Remove();
        virtual void ReceiveNodesToLink(std::list<m3d::SgNode *,std::allocator<m3d::SgNode *> > &) const ;
        static m3d::Class * GetBaseClass();
        virtual void RelinkSceneGraphNode();
        virtual void SetDeadTimer(int,bool);
        virtual void TransferToSpace(dxSpace *);
        virtual void SetNodeAction(int,bool);
        virtual void SetSkin(int);
        virtual CStr GetPropertyName(int) const ;
        virtual m3d::Class * GetClass() const ;
        virtual SimplePhysicObjPrototypeInfo const * GetPrototypeInfo() const ;
        virtual CVector GetGeometricCenter() const ;
        virtual void RelinkGeomsToCollisionCells();
        virtual void SetNextForAnimation(int,int);
        virtual void SetInvisible();
        virtual void Update(float,unsigned int);
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        virtual void EnableGeometry(bool);
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual bool IsVisible();
        virtual Geom::CellAabb GetCollisionCellAabb() const ;
        void SetScale(float,bool);
        virtual void UnlinkGeomsFromCollisionCells();
        virtual void TransferPhysicParamsToSceneGraphNode();
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual void SetVisible();
        virtual void RenderDebugInfo() const ;
        virtual void SetBelong(int);

    protected:
        virtual void _InternalCreateVisualPart();
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        void _UpdateCollisionInfoFromPhysicBody();
        virtual void _InternalPostLoad();
        virtual void _SetPositionToGeoms(CVector const &);
        virtual void _Construct();
        virtual void _SetRotationToGeoms(Quaternion const &);
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        void _UpdatePhysicBodyByCollisionInfo(std::vector<CollisionInfo> const &);
        virtual void _LinkBodyToGeoms();
        void _UpdateFullPhysicBodyByCollisionInfo(std::vector<CollisionInfo> const &);
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual ~SimplePhysicObj();
        virtual void _UnlinkBodyFromGeoms();

    private:
        virtual m3d::Object * Clone();
        static m3d::Object * CreateObject();

    public:
        RT_CLASS_INLINE_DECLARE(SimplePhysicObj);

    private:
        SimplePhysicBody *m_physicBody;
        std::vector<CollisionInfo> m_collisionInfos;
        float m_scale;
        bool m_deadTimerActive;
        float m_deadTimer;
        bool m_testVisibility;
    };
}
