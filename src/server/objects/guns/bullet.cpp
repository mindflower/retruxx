#include "bullet.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Bullet)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Bullet);

    bool BulletPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    BulletPrototypeInfo::BulletPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* BulletPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    void Bullet::LinkGeomsToCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    void Bullet::SetParentBarrel(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Bullet::SetTracer(m3d::SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Bullet::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Bullet::Bullet(BulletPrototypeInfo const& prototype) : Shell(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    Geom::CellAabb Bullet::GetCollisionCellAabb() const
    {
        throw std::logic_error("Not implemented");
    }

    void Bullet::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    Ray* Bullet::_Ray()
    {
        throw std::logic_error("Not implemented");
    }

    Ray const* Bullet::_Ray() const
    {
        throw std::logic_error("Not implemented");
    }

    void Bullet::RelinkGeomsToCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    void Bullet::SetRange(float)
    {
        throw std::logic_error("Not implemented");
    }

    void Bullet::UnlinkGeomsFromCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    void Bullet::SpecifyTracer(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Bullet::SetDirection(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    CVector Bullet::GetDirection() const
    {
        throw std::logic_error("Not implemented");
    }

    void Bullet::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Bullet::TransferPhysicParamsToSceneGraphNode()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Bullet::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Shell);
    }

    BulletPrototypeInfo const* Bullet::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    Bullet::~Bullet()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Bullet::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Bullet::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}
