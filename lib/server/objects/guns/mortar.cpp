#include "mortar.h"

#include <math/matrix.h>
#include <stdexcept>
#include <utility>

#include "core/kernel.h"
#include "mortarshell.h"
#include "scene/nodes/sgnode.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/physicbodies/physichelpers.h"
#include "server/objects/physicbodies/geoms/geomtransform.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Mortar)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Mortar);

    namespace
    {
        // Gravity used by the ballistic solver, and its derived constants.
        float const GRAVITY = 9.8100004f;
        float const HALF_GRAVITY = 4.9050002f;
        float const GRAVITY_SQ = 96.236107f;
        float const TWO_OVER_GRAVITY_SQ = 0.020782221f;

        // Marks a trajectory that does not exist.
        float const NO_SOLUTION = 1.0e10f;
    }  // namespace

    void MortarPrototypeInfo::PostLoad()
    {
        // RVA 0x8493E0
        GunPrototypeInfo::PostLoad();
    }

    MortarPrototypeInfo::MortarPrototypeInfo()
    {
        m_damageType = DAMAGE_BLAST;
        m_WithShellsPoolLimit = 1;
        m_initialVelocity = 50.0f;
    }

    bool MortarPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = GunPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_initialVelocity, xmlNode, "InitialVelocity");
        }
        return result;
    }

    Obj* MortarPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x8494A0
        return new Mortar(*this);
    }

    CVector Mortar::TMortarAngles::ToVector() const
    {
        // RVA 0x849420 - phi is the heading about the vertical axis, theta the elevation.
        double const cosTheta = std::cos(theta);
        return CVector(
            static_cast<float>(std::sin(phi) * cosTheta),
            static_cast<float>(std::sin(theta)),
            static_cast<float>(std::cos(phi) * cosTheta));
    }

    Mortar::TMortarAngles::TMortarAngles(float _phi, float _theta) : phi(_phi), theta(_theta)
    {
        // RVA 0x8493A0
    }

    Mortar::TMortarAngles::TMortarAngles()
    {
        // RVA 0x849390 - the angles are left uninitialized.
    }

    Mortar::Mortar(MortarPrototypeInfo const& prototype) : Gun(prototype)
    {
        // RVA 0x8493F0
    }

    void Mortar::LookAtPoint(CVector const& lookAt, float elapsedTime)
    {
        // RVA 0x849E40 - aims the barrel along the ballistic launch direction instead of straight
        // at the target, by handing Gun a point 1000 units out along that direction.
        MortarPrototypeInfo const* prototype = GetPrototypeInfo();
        CVector const pos = GetBarrelNode() ? GetBarrelNode()->GetOriginWorldAbs() : GetNodeAbsolutePosition();
        CVector const dir = _CalcDirForNextShot();
        CVector const wishedDir = GetMortarDirection(dir, pos, lookAt, prototype->m_initialVelocity);
        CVector const aimPoint(wishedDir.x * 1000.0f + pos.x, wishedDir.y * 1000.0f + pos.y, wishedDir.z * 1000.0f + pos.z);
        Gun::LookAtPoint(aimPoint, elapsedTime);
    }

    m3d::Class* Mortar::GetClass() const
    {
        // RVA 0x8493D0
        return RT_CLASS_LOCAL(Mortar);
    }

    CVector Mortar::GetMortarShellPosition(CVector const& source, CVector const& velocity, float time)
    {
        // RVA 0x849550 - the shell's position after time seconds of free flight.
        return CVector(
            velocity.x * time + source.x,
            (velocity.y - time * HALF_GRAVITY) * time + source.y,
            velocity.z * time + source.z);
    }

    bool Mortar::isLookAtPoint(CVector const& lookAt, float eps) const
    {
        // RVA 0x849F30 - the check that matches LookAtPoint.
        MortarPrototypeInfo const* prototype = GetPrototypeInfo();
        CVector const pos = GetBarrelNode() ? GetBarrelNode()->GetOriginWorldAbs() : GetNodeAbsolutePosition();
        CVector const dir = _CalcDirForNextShot();
        CVector const wishedDir = GetMortarDirection(dir, pos, lookAt, prototype->m_initialVelocity);
        CVector const aimPoint(wishedDir.x * 1000.0f + pos.x, wishedDir.y * 1000.0f + pos.y, wishedDir.z * 1000.0f + pos.z);
        return Gun::isLookAtPoint(aimPoint, eps);
    }

    MortarPrototypeInfo const* Mortar::GetPrototypeInfo() const
    {
        // RVA 0x84A0C0 - NOTE: the prototype is cast without a type check.
        return static_cast<MortarPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Class* Mortar::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    // RVA 0x849410
    Mortar::~Mortar() = default;

    void Mortar::_LaunchShells()
    {
        // RVA 0x84A0F0
        Gun::_LaunchShells();

        int const shellId = theObjects->CreateNewObject(GetShellPrototypeId(), "", -1, -1);
        MortarShell* shell = static_cast<MortarShell*>(theObjects->GetEntityByObjId(shellId));
        // NOTE: the shell is not checked for null before use.
        shell->SetGunObjId(GetId());
        PhysicObj* owner = GetOwner();
        if (owner)
        {
            shell->SetBelong(owner->GetBelong());
        }

        CVector const dir = _CalcDirForNextShot();
        float const initialVelocity = GetPrototypeInfo()->m_initialVelocity;
        shell->SetInitialVelocity(CVector(dir.x * initialVelocity, dir.y * initialVelocity, initialVelocity * dir.z));
        shell->SetInitialPosition(_CalcPosForNextShot());
        shell->SetPosition(shell->GetInitialPosition());
        shell->SetDirection(dir);
    }

    CVector Mortar::GetVelocityByDirection(CVector const& dir) const
    {
        // RVA 0x849510
        float const initialVelocity = GetPrototypeInfo()->m_initialVelocity;
        return CVector(dir.x * initialVelocity, dir.y * initialVelocity, dir.z * initialVelocity);
    }

    void Mortar::setCurrentMortarAngles(TMortarAngles const& angles)
    {
        // RVA 0x84A200 - the heading turns the whole gun, the elevation only the barrel.
        float const halfPhi = angles.phi * 0.5f;
        Quaternion const quatHorizRotation(
            0.0f, static_cast<float>(std::sin(halfPhi)), 0.0f, static_cast<float>(std::cos(halfPhi)));
        SetNodeAbsoluteRotation(quatHorizRotation);

        float const halfTheta = (0.0f - angles.theta) * 0.5f;
        Quaternion const quatElevation(
            static_cast<float>(std::sin(halfTheta)), 0.0f, 0.0f, static_cast<float>(std::cos(halfTheta)));
        GetBarrelNode()->SetRotation(quatElevation);
        CommonGeomMovedCallback(m_pGeoms[0]->GetGeomId());
    }

    m3d::Object* Mortar::Clone()
    {
        // RVA 0x849AC0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    float Mortar::targetFunction2(CVector const& src, CVector const& dst, float phi, float, float time, float theta) const
    {
        // RVA 0x8495B0 - how far a shell fired at heading phi and elevation theta lands from dst
        // after time seconds. NOTE: the initVel argument is ignored; the prototype's initial
        // velocity is used instead.
        double const cosTheta = std::cos(theta);
        float const dirX = static_cast<float>(std::sin(phi) * cosTheta);
        float const dirY = static_cast<float>(std::sin(theta));
        float const dirZ = static_cast<float>(std::cos(phi) * cosTheta);

        float const initVel = GetPrototypeInfo()->m_initialVelocity;
        float const dx = dirX * initVel * time + src.x - dst.x;
        float const dy = (dirY * initVel - time * HALF_GRAVITY) * time + src.y - dst.y;
        float const dz = dirZ * initVel * time + src.z - dst.z;
        return static_cast<float>(std::sqrt(double(dz) * dz + double(dy) * dy + double(dx) * dx));
    }

    float Mortar::targetFunction(CVector const& src, CVector const& dst, float phi, float initVel, float time) const
    {
        // RVA 0x84A020 - the miss distance of the elevation that covers the horizontal distance
        // in time seconds.
        double const dz = double(dst.z) - src.z;
        double const dx = double(dst.x) - src.x;
        float const dist = static_cast<float>(std::sqrt(dz * dz + dx * dx));
        float const invVel = static_cast<float>(1.0 / initVel);
        float const invTime = static_cast<float>(1.0 / time);
        if (double(invTime) * invVel * dist > 1.0)
        {
            return NO_SOLUTION;
        }
        float const theta = static_cast<float>(std::acos(double(invTime) * invVel * dist));
        // NOTE: the shipped call swaps the last two arguments: the elevation is passed as the
        // flight time and the flight time as the elevation.
        return targetFunction2(src, dst, phi, initVel, theta, time);
    }

    CVector Mortar::GetMortarDirection(CVector const& currentDir, CVector const& src, CVector const& dst, float initVel) const
    {
        // RVA 0x849680 - solves for the launch elevations that hit dst at speed initVel. There are
        // up to two (a flat and a lobbed shot); each is checked by simulating it, and the lower one
        // that the gun can reach vertically wins. Without a solution the current direction is kept.
        float const dx = dst.x - src.x;
        float const dz = dst.z - src.z;
        float const dy = dst.y - src.y;
        float const B = dy * GRAVITY - initVel * initVel;
        float const dist = std::sqrt(dx * dx + dz * dz);
        float const phi = static_cast<float>(std::atan2(dx, dz));
        float const range = static_cast<float>(std::sqrt(double(dy) * dy + dx * dx + dz * dz));

        // The discriminant of the flight time's quadratic in t^2.
        float const discriminant = B * B - range * range * GRAVITY_SQ;
        if (discriminant < 0.0f)
        {
            return currentDir;
        }
        float const root = std::sqrt(discriminant);

        float theta1 = NO_SOLUTION;
        float const time1Sq = (root - B) * TWO_OVER_GRAVITY_SQ;
        if (time1Sq >= 0.0f)
        {
            float const time1 = std::sqrt(time1Sq);
            double const cosTheta = 1.0 / initVel / time1 * dist;
            if (std::fabs(cosTheta) <= 1.0)
            {
                theta1 = static_cast<float>(std::acos(static_cast<float>(cosTheta)));
                if (std::fabs(targetFunction2(src, dst, phi, initVel, time1, theta1)) > 3.0f)
                {
                    theta1 = 0.0f - theta1;
                }
                if (std::fabs(targetFunction2(src, dst, phi, initVel, time1, theta1)) > 5.0f)
                {
                    theta1 = NO_SOLUTION;
                }
            }
        }

        float theta2 = NO_SOLUTION;
        float const time2Sq = ((0.0f - B) - root) * TWO_OVER_GRAVITY_SQ;
        if (time2Sq >= 0.0f)
        {
            float const time2 = std::sqrt(time2Sq);
            double const cosTheta = 1.0 / initVel / time2 * dist;
            if (std::fabs(cosTheta) <= 1.0)
            {
                theta2 = static_cast<float>(std::acos(static_cast<float>(cosTheta)));
                if (std::fabs(targetFunction2(src, dst, phi, initVel, time2, theta2)) > 3.0f)
                {
                    theta2 = 0.0f - theta2;
                }
                if (std::fabs(targetFunction2(src, dst, phi, initVel, time2, theta2)) > 5.0f)
                {
                    theta2 = NO_SOLUTION;
                }
            }
        }

        if (theta1 > theta2)
        {
            std::swap(theta1, theta2);
        }

        CVector dir1(0.0f, 0.0f, 0.0f);
        if (theta1 != NO_SOLUTION)
        {
            dir1 = TMortarAngles(phi, theta1).ToVector();
            if (_IsDirVerticallyReachable(dir1))
            {
                return dir1;
            }
        }
        if (theta2 != NO_SOLUTION)
        {
            CVector const dir2 = TMortarAngles(phi, theta2).ToVector();
            if (_IsDirVerticallyReachable(dir2))
            {
                return dir2;
            }
        }
        // Neither is reachable: fall back to the lower solution, if any.
        return theta1 == NO_SOLUTION ? currentDir : dir1;
    }

    m3d::Object* Mortar::CreateObject()
    {
        // RVA 0x849C80
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai
