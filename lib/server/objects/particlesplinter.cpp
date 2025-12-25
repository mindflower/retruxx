#include "particlesplinter.h"
#include "base/prototypemanager.h"
#include <client.h>
#include "world.h"
namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(ParticleSplinter)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(ParticleSplinter);

    Obj* ParticleSplinterPrototypeInfo::CreateTargetObject() const
    {
        return new ParticleSplinter(*this);
    }

    m3d::Class* ParticleSplinter::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DummyObject);
    }

    ParticleSplinter::ParticleSplinter(ParticleSplinterPrototypeInfo const& prototype) : DummyObject(prototype)
    {
        TransferToSpace(ai::gSpaceForAllPhysicParticles);
    }

    ParticleSplinterPrototypeInfo const* ParticleSplinter::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), ParticleSplinterPrototypeInfo const);
    }

    m3d::Class* ParticleSplinter::GetClass() const
    {
        return RT_CLASS_LOCAL(ParticleSplinter);
    }

    void ParticleSplinter::Update(float elapsedTime, unsigned workTime)
    {
        ai::SimplePhysicObj::Update(elapsedTime, workTime);
        CheckDisablePhysics();

        CVector const pos = GetPosition();
        float const height =
            m3d::pClient->GetWorld().GetLandscape().getWaterHeight(static_cast<int>(pos.x * 0.03125), static_cast<int>(pos.z * 0.03125)) -
            2.0f;

        if (height > pos.y)
        {
            Remove();
        }
    }

    ParticleSplinter::~ParticleSplinter() = default;

    m3d::Object* ParticleSplinter::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* ParticleSplinter::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSplinter::CheckDisablePhysics()
    {
        if ((GetPhysicState() & 1) != 0)
        {
            CVector const vel = GetLinearVelocity();
            CVector const angularVel = GetAngularVelocity();
            if ((float)((float)((float)(vel.z * vel.z) + (float)(vel.y * vel.y)) + (float)(vel.x * vel.x)) < 0.0099999998 &&
                (float)((float)((float)(angularVel.z * angularVel.z) + (float)(angularVel.y * angularVel.y)) +
                        (float)(angularVel.x * angularVel.x)) < 0.2)
            {
                CVector const pos = GetPosition();
                float const lsHeight = m3d::pClient->GetWorld().GetLandscape().GetLsHeight(pos.x, pos.z);
                if (fabs(pos.y - lsHeight) >= 0.2)
                {
                    DisablePhysicsWithAutoEnable();
                }
                else
                {
                    DisablePhysics();
                    DisableGeometry(1);
                }
            }
        }
    }
}  // namespace ai
