#include "bullet.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Bullet)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Bullet);

    bool BulletPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        const auto res = ShellPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (res)
        {
            _SetGeomType(GEOM_TYPE_RAY);
        }
        return res;
    }

    BulletPrototypeInfo::BulletPrototypeInfo() = default;

    Obj* BulletPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Bullet::LinkGeomsToCollisionCells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Bullet::SetParentBarrel(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Bullet::SetTracer(m3d::SgNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Bullet::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Bullet::Bullet(BulletPrototypeInfo const& prototype) : Shell(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Geom::CellAabb Bullet::GetCollisionCellAabb() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Bullet::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Ray* Bullet::_Ray()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Ray const* Bullet::_Ray() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Bullet::RelinkGeomsToCollisionCells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Bullet::SetRange(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Bullet::UnlinkGeomsFromCollisionCells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Bullet::SpecifyTracer(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Bullet::SetDirection(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Bullet::GetDirection() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Bullet::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Bullet::TransferPhysicParamsToSceneGraphNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Bullet::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Shell);
    }

    BulletPrototypeInfo const* Bullet::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Bullet::~Bullet()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Bullet::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Bullet::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
