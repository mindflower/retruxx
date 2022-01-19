#pragma once
#include <server/objects/base/shell.h>
namespace ai
{
    class Ray;

    class BulletPrototypeInfo : public ShellPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        BulletPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
    };

    class Bullet : public Shell
    {
    public:
        virtual void LinkGeomsToCollisionCells();
        void SetParentBarrel(unsigned int);
        void SetTracer(m3d::SgNode *);
        virtual m3d::Class * GetClass() const ;
        Bullet(BulletPrototypeInfo const &);
        virtual Geom::CellAabb GetCollisionCellAabb() const ;
        virtual void RenderDebugInfo() const ;
        Ray * _Ray();
        Ray const * _Ray() const ;
        virtual void RelinkGeomsToCollisionCells();
        void SetRange(float);
        virtual void UnlinkGeomsFromCollisionCells();
        void SpecifyTracer(CVector const &);
        virtual void SetDirection(CVector const &);
        virtual CVector GetDirection() const ;
        virtual void Update(float,unsigned int);
        virtual void TransferPhysicParamsToSceneGraphNode();
        static m3d::Class * GetBaseClass();
        virtual BulletPrototypeInfo const * GetPrototypeInfo() const ;

    protected:
        virtual ~Bullet();

    public:
        RT_CLASS_INLINE_DECLARE(Bullet);

    private:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();
        unsigned __int16 m_explosionType;
        int m_framesToLive;
        m3d::SgNode *m_tracer;
        unsigned int m_parentBarrel;
    };
}
