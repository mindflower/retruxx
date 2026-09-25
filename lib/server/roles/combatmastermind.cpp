#include "combatmastermind.h"

#include "math/vector2.h"
#include "server/relationship.h"
#include "server/obstacle.h"
#include "server/objects/team.h"
#include "server/objects/vehicle.h"
#include "server/objects/staticautogun.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/physicbodies/geoms/sphereforintersection.h"

namespace ai
{
    namespace
    {
        struct Circle
        {
            /* 0x0000 */ CVector2 m_center;
            /* 0x0008 */ float m_radius;
            /* 0x000c */ float m_sqrRadius;
        }; /* size: 0x0010 */

        // A vehicle's line of fire, on the ground plane: from its own circle to its target.
        struct AimInfo
        {
            /* 0x0000 */ Circle m_vehicleCircle;
            /* 0x0010 */ CVector2 m_targetPos;
            /* 0x0018 */ CVector2 m_direction;

            // RVA 0x8AAED0 - whether the line of fire runs through the circle: either end inside
            // it, or the line passing within its radius between the ends.
            bool bIntersectsCircle(Circle const& circle) const
            {
                float const dy0 = circle.m_center.y - m_vehicleCircle.m_center.y;
                float const dx0 = circle.m_center.x - m_vehicleCircle.m_center.x;
                if (m_vehicleCircle.m_sqrRadius > dy0 * dy0 + dx0 * dx0)
                {
                    return true;
                }
                float const dy1 = m_vehicleCircle.m_center.y - circle.m_center.y;
                float const dx1 = m_vehicleCircle.m_center.x - circle.m_center.x;
                if (circle.m_sqrRadius > dy1 * dy1 + dx1 * dx1)
                {
                    return true;
                }
                float const dy2 = m_targetPos.y - circle.m_center.y;
                float const dx2 = m_targetPos.x - circle.m_center.x;
                if (circle.m_sqrRadius > dy2 * dy2 + dx2 * dx2)
                {
                    return true;
                }
                // Distance from the circle's centre to the line, squared.
                float const ny = m_vehicleCircle.m_center.y - m_targetPos.y;
                float const nx = m_targetPos.x - m_vehicleCircle.m_center.x;
                float const d = (circle.m_center.y - m_vehicleCircle.m_center.y) * nx +
                    (circle.m_center.x - m_vehicleCircle.m_center.x) * ny;
                if (d * d / (nx * nx + ny * ny) > circle.m_sqrRadius)
                {
                    return false;
                }
                // Within reach of the line: it counts only if the centre lies between the ends.
                int side = (circle.m_center.y - m_vehicleCircle.m_center.y) * ny - nx * dx0 >= 0.0f ? 1 : -1;
                if ((circle.m_center.y - m_targetPos.y) * ny - nx * (circle.m_center.x - m_targetPos.x) < 0.0f)
                {
                    side = -side;
                }
                return side < 0;
            }
        };

        std::vector<int> getAllVehicles()
        {
            // RVA 0x5E19A0 - the ids of every vehicle there is.
            std::vector<int> result;
            for (Obj* obj : *theObjects)
            {
                if (obj->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
                {
                    result.push_back(obj->GetId());
                }
            }
            return result;
        }

        std::vector<int> getAllStaticAutoGuns()
        {
            // RVA 0x742CE0 - the ids of every static auto gun there is.
            std::vector<int> result;
            for (Obj* obj : *theObjects)
            {
                if (obj->IsKindOf(RT_CLASS_LOCAL(StaticAutoGun)))
                {
                    result.push_back(obj->GetId());
                }
            }
            return result;
        }

        // RVA 0x8ABE10 - the vehicle's target for this purpose: the first live vehicle or static
        // auto gun, anywhere, that its side is at war with.
        PhysicObj* GetTarget(Vehicle* v)
        {
            if (!v || !v->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
            {
                return nullptr;
            }
            std::vector<int> targets = getAllVehicles();
            std::vector<int> const guns = getAllStaticAutoGuns();
            targets.insert(targets.end(), guns.begin(), guns.end());
            for (int const id : targets)
            {
                // NOTE: the ids come from the container, so the object is not checked for null.
                Obj* obj = theObjects->GetEntityByObjId(id);
                if (obj->IsAlive() && theRelationship->CheckTolerance(v->GetBelong(), obj->GetBelong()) <= RS_ENEMY)
                {
                    return static_cast<PhysicObj*>(obj);
                }
            }
            return nullptr;
        }
    }  // namespace

    CombatMastermind::CombatMastermind(int teamId)
    {
        // RVA 0x8AAB40
        this->m_teamId = teamId;
    }

    void CombatMastermind::GenerateLineOfFireAvoidanceSteeringForces(retruxx::map<int, CVector>& steeringForces)
    {
        // RVA 0x8AC110 - keeps the team's vehicles out of each other's line of fire. Each vehicle
        // counts the team-mates whose line of fire passes over it, +1 for each one to its left and
        // -1 for each one to its right (seen along its own aim), and is pushed sideways, across its
        // aim, towards the side the balance points to.
        // NOTE: the team is not checked for null.
        auto* team = static_cast<Team*>(theObjects->GetEntityByObjId(m_teamId));
        std::vector<AimInfo> aims;
        for (Vehicle* vehicle : team->GetVehicles())
        {
            PhysicObj* target = GetTarget(vehicle);
            if (!target)
            {
                continue;
            }
            CVector const pos = vehicle->GetPosition();
            CVector const targetPos = target->GetPosition();
            float const radius = vehicle->m_intersectionObstacle
                ? static_cast<float>(vehicle->m_intersectionObstacle->GetSphere()->GetRadius())
                : 0.0f;
            AimInfo aim;
            aim.m_targetPos = CVector2(targetPos.x, targetPos.z);
            aim.m_vehicleCircle.m_center = CVector2(pos.x, pos.z);
            aim.m_vehicleCircle.m_radius = radius;
            aim.m_vehicleCircle.m_sqrRadius = radius * radius;
            aim.m_direction = CVector2(targetPos.x - pos.x, targetPos.z - pos.z);
            aims.push_back(aim);
        }

        std::vector<int> avoidances;
        for (size_t i = 0; i < aims.size(); ++i)
        {
            int avoidance = 0;
            AimInfo const& own = aims[i];
            for (size_t k = 0; k < aims.size(); ++k)
            {
                if (k == i || !own.bIntersectsCircle(aims[k].m_vehicleCircle))
                {
                    continue;
                }
                Circle const& other = aims[k].m_vehicleCircle;
                if (own.m_direction.x * (other.m_center.y - own.m_vehicleCircle.m_center.y) -
                        own.m_direction.y * (other.m_center.x - own.m_vehicleCircle.m_center.x) >=
                    0.0f)
                {
                    ++avoidance;
                }
                else
                {
                    --avoidance;
                }
            }
            avoidances.push_back(avoidance);
        }

        // NOTE: the aims were collected only for vehicles that have a target, but the forces are
        // written back by position in the team, so any vehicle without one shifts the rest.
        steeringForces.clear();
        for (size_t i = 0; i < aims.size(); ++i)
        {
            Vehicle* vehicle = i < team->GetVehicles().size() ? team->GetVehicles()[i] : nullptr;
            AimInfo const& aim = aims[i];
            float const sideX = aim.m_vehicleCircle.m_center.y - aim.m_targetPos.y;
            float const sideZ = aim.m_targetPos.x - aim.m_vehicleCircle.m_center.x;
            CVector force;
            if (avoidances[i] > 0)
            {
                force = CVector(sideX, 0.0f, sideZ);
            }
            else if (avoidances[i] < 0)
            {
                force = CVector(0.0f - sideX, 0.0f, 0.0f - sideZ);
            }
            else
            {
                force = ZeroVector;
            }
            // NOTE: a missing vehicle is not checked for null.
            steeringForces[vehicle->GetId()] = force;
        }
    }
}  // namespace ai
