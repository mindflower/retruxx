#include "thunderbolt.h"
#include "core/ini.h"

#include <ode/contact.h>

#include "core/kernel.h"
#include "m3dapp.h"
#include "thunderboltlauncher.h"
#include "math/matrix.h"
#include "math/segment.h"
#include "scene/nodes/sgnode.h"
#include "scene/scenegraph.h"
#include "server/damageinfo.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/complexphysicobj.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/physicbodies/physicbody.h"
#include "server/objects/physicbodies/physichelpers.h"
#include "server/objects/physicbodies/vehiclepart.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Thunderbolt)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Thunderbolt);

    namespace
    {
        CVector const ZERO_VECTOR(0.0f, 0.0f, 0.0f);

        // Scene node property ids that hand the polyline to the bolt's effect nodes.
        unsigned const PROP_THUNDERBOLT_POINTS_UPDATE = 9474;
        unsigned const PROP_THUNDERBOLT_POINTS_INIT = 9475;

        // Calls SetProperty(propId, points) on every node below root (not on root itself).
        void SetPointsOnSubtree(m3d::SgNode* root, unsigned propId, retruxx::vector<CVector> const& points)
        {
            std::vector<m3d::Object*> stack;
            stack.push_back(root);
            while (!stack.empty())
            {
                m3d::Object* current = stack.back();
                stack.pop_back();
                for (m3d::Object* child = current->GetFirstChild(); child; child = child->GetNextSibling())
                {
                    child->SetProperty(propId, const_cast<retruxx::vector<CVector>*>(&points));
                    if (child->GetFirstChild())
                    {
                        stack.push_back(child);
                    }
                }
            }
        }
    }  // namespace

    ThunderboltPrototypeInfo::ThunderboltPrototypeInfo()
    {
        m_flyTime = 1.0f;
        m_Damage = 0.0f;
        m_AverageSegmentLength = 0.1f;
    }

    bool ThunderboltPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_flyTime, xmlNode, "FlyTime");
            m3d::SafeFloatAttrib(m_Damage, xmlNode, "Damage");
            m3d::SafeFloatAttrib(m_AverageSegmentLength, xmlNode, "AverageSegmentLength");
            m3d::SafeStrAttrib(m_EffectName, xmlNode, "Effect");
        }
        return result;
    }

    void ThunderboltPrototypeInfo::PostLoad()
    {
        // RVA 0x7E0E40 - keeps the segment length (and so the number of kinks) sane.
        if (m_AverageSegmentLength < 0.0099999998f)
        {
            m_AverageSegmentLength = 0.0099999998f;
        }
    }

    ai::Obj* ThunderboltPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x7E50B0
        return new Thunderbolt(*this);
    }

    // RVA 0x7E4E00
    Thunderbolt::~Thunderbolt() = default;

    Thunderbolt::Thunderbolt(ai::ThunderboltPrototypeInfo const& prototypeInfo) :
        Obj(prototypeInfo),
        m_lifeTime(0.0, 0.0, prototypeInfo.m_flyTime, 1.0)
    {
        // RVA 0x7E4D60
    }

    m3d::Object* Thunderbolt::Clone()
    {
        // RVA 0x7E1110
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* Thunderbolt::CreateObject()
    {
        // RVA 0x7E12D0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Class* Thunderbolt::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    m3d::Class* Thunderbolt::GetClass() const
    {
        // RVA 0x7E0510
        return RT_CLASS_LOCAL(Thunderbolt);
    }

    ai::ThunderboltPrototypeInfo const* Thunderbolt::GetPrototypeInfo() const
    {
        // RVA 0x7E1930 - NOTE: the prototype is cast without a type check.
        return static_cast<ThunderboltPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    Thunderbolt::ThunderboltPoint::ThunderboltPoint(ai::Thunderbolt::ThunderboltPoint const& __that) :
        m_Point(__that.m_Point),
        m_ObjId(__that.m_ObjId),
        m_launcher(__that.m_launcher),
        m_dependant(__that.m_dependant),
        m_masterPoints(__that.m_masterPoints)
    {
        // RVA 0x7E2030
    }

    Thunderbolt::ThunderboltPoint::ThunderboltPoint(CVector const& point) :
        m_Point(point), m_ObjId(-1), m_launcher(false), m_dependant(false)
    {
        // RVA 0x7E1FF0
    }

    Thunderbolt::ThunderboltPoint::ThunderboltPoint(int const& objId, bool launcher) :
        m_Point(ZERO_VECTOR), m_ObjId(objId), m_launcher(launcher), m_dependant(false)
    {
        // RVA 0x7E1FA0
    }

    Thunderbolt::ThunderboltPoint::ThunderboltPoint() : m_Point(ZERO_VECTOR), m_ObjId(-1), m_launcher(false), m_dependant(false)
    {
        // RVA 0x7E1F60
    }

    int Thunderbolt::ThunderboltPoint::getObjId() const
    {
        // RVA 0x7E0520
        return m_ObjId;
    }

    void Thunderbolt::ThunderboltPoint::setObjid(int objId, bool launcher)
    {
        // RVA 0x7E0530
        m_ObjId = objId;
        m_launcher = launcher;
    }

    CVector const& Thunderbolt::ThunderboltPoint::getPoint() const
    {
        // RVA 0x7E0550
        return m_Point;
    }

    void Thunderbolt::ThunderboltPoint::setPoint(CVector const& point)
    {
        // RVA 0x7E0560
        m_Point = point;
    }

    void Thunderbolt::ThunderboltPoint::setDependencies(
        retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint>> const& dependencies)
    {
        // RVA 0x7E2340 - a branching point follows the points it was split from.
        if (dependencies.size() > 1)
        {
            m_dependant = true;
            m_masterPoints = dependencies;
        }
    }

    CVector const& Thunderbolt::ThunderboltPoint::getThunderboltPoint()
    {
        // RVA 0x7E1960 - refreshes and returns the point's position. A branching point sits halfway
        // from its last master point towards the average of the others; an object point follows
        // the object (a launcher's muzzle, or the target's geometric centre).
        if (m_dependant)
        {
            CVector const org = m_masterPoints.back().getThunderboltPoint();
            CVector averageDirection = ZERO_VECTOR;
            unsigned const count = static_cast<unsigned>(m_masterPoints.size()) - 1;
            for (unsigned i = 0; i < count; ++i)
            {
                CVector const& p = m_masterPoints[i].getThunderboltPoint();
                averageDirection.y = (p.y - org.y) + averageDirection.y;
                averageDirection.z = (p.z - org.z) + averageDirection.z;
                averageDirection.x = (p.x - org.x) + averageDirection.x;
            }
            float const invCount = static_cast<float>(1.0 / static_cast<double>(count));
            m_Point.x = invCount * averageDirection.x * 0.5f + org.x;
            m_Point.y = averageDirection.y * invCount * 0.5f + org.y;
            m_Point.z = averageDirection.z * invCount * 0.5f + org.z;
            return m_Point;
        }

        Obj* obj = theObjects->GetEntityByObjId(m_ObjId);
        if (!obj)
        {
            return m_Point;
        }
        if (m_launcher && obj->IsKindOf(RT_CLASS_LOCAL(ThunderboltLauncher)))
        {
            CMatrix const shot = static_cast<Gun*>(obj)->GetMatrixForShot(0);
            m_Point = CVector(shot._41, shot._42, shot._43);
        }
        else
        {
            m_Point = getPhysicObjOrPhysicBodyGeometricCenter(obj);
        }
        return m_Point;
    }

    retruxx::vector<CVector, retruxx::allocator<CVector>> const& Thunderbolt::ThunderboltSegment::SegmentFinding(Segment const& seg) const
    {
        // RVA 0x7E2470 - a jagged line from seg.begin() to seg.end(): m_nSteps points evenly along
        // it, each pushed off by up to one step length on every axis. NOTE: the result lives in a
        // function-local static and is overwritten by the next call.
        static retruxx::vector<CVector> points;
        points.resize(m_nSteps);

        CVector const& begin = seg.begin();
        CVector const& end = seg.end();
        float const dz = end.z - begin.z;
        float const dy = end.y - begin.y;
        float const dx = end.x - begin.x;
        float const stepLength = static_cast<float>(std::sqrt(double(dz * dz + dy * dy) + dx * dx) / static_cast<double>(m_nSteps));

        if (!points.empty())
        {
            points[0] = begin;
        }
        for (unsigned i = 1; i < points.size(); ++i)
        {
            float const k = static_cast<float>(i);
            float const ox = (end.x - begin.x) * k;
            float const oy = (end.y - begin.y) * k;
            float const oz = (end.z - begin.z) * k;
            double const invSteps = 1.0 / static_cast<double>(m_nSteps);
            float const baseX = begin.x + static_cast<float>(invSteps * ox);
            float const baseY = begin.y + static_cast<float>(oy * invSteps);
            float const baseZ = begin.z + static_cast<float>(oz * invSteps);
            // rand() / RAND_MAX in [-1, 1], drawn in the order y, x, z.
            float const jitterY = static_cast<float>(rand()) * 0.000030518509f * 2.0f - 1.0f;
            float const jitterX = static_cast<float>(rand()) * 0.000030518509f * 2.0f - 1.0f;
            float const jitterZ = static_cast<float>(rand()) * 0.000030518509f * 2.0f - 1.0f;
            points[i].x = jitterX * stepLength + baseX;
            points[i].y = jitterY * stepLength + baseY;
            points[i].z = jitterZ * stepLength + baseZ;
        }
        points.push_back(end);
        return points;
    }

    Thunderbolt::ThunderboltSegment::ThunderboltSegment(ai::Thunderbolt::ThunderboltSegment const& __that) :
        m_Start(__that.m_Start),
        m_Finish(__that.m_Finish),
        m_nSteps(__that.m_nSteps),
        m_EffectCreated(__that.m_EffectCreated),
        m_EffectNode(__that.m_EffectNode)
    {
        // RVA 0x7E2250 - NOTE: the effect node pointer is shared, and each copy's destructor
        // removes it from the scene graph.
    }

    Thunderbolt::ThunderboltSegment::ThunderboltSegment(
        ai::ThunderboltPrototypeInfo const* thunderboltPrototype,
        ai::Thunderbolt::ThunderboltPoint const& Start,
        ai::Thunderbolt::ThunderboltPoint const& Finish) :
        m_Start(Start),
        m_Finish(Finish),
        m_EffectCreated(false),
        m_EffectNode(nullptr)
    {
        // RVA 0x7E2380 - one kink per AverageSegmentLength of the current distance.
        CVector const& start = m_Start.getThunderboltPoint();
        CVector const& finish = m_Finish.getThunderboltPoint();
        float const dx = finish.x - start.x;
        float const dy = finish.y - start.y;
        float const dz = finish.z - start.z;
        m_nSteps = static_cast<unsigned>(static_cast<int>(
            std::sqrt(double(dz) * dz + double(dy) * dy + double(dx) * dx) / thunderboltPrototype->m_AverageSegmentLength));
    }

    Thunderbolt::ThunderboltSegment::~ThunderboltSegment()
    {
        // RVA 0x7E2080
        if (m_EffectNode)
        {
            m_EffectNode->GetGraph()->RemoveNode(m_EffectNode);
            m_EffectNode = nullptr;
        }
    }

    retruxx::vector<CVector, retruxx::allocator<CVector>> const& Thunderbolt::ThunderboltSegment::getThunderboltPoints()
    {
        // RVA 0x7E2720
        CVector const& finish = m_Finish.getThunderboltPoint();
        CVector const& start = m_Start.getThunderboltPoint();
        return SegmentFinding(Segment(start, finish));
    }

    void Thunderbolt::ThunderboltSegment::Render(ai::ThunderboltPrototypeInfo const* thunderboltPrototype)
    {
        // RVA 0x7E2790 - the first frame creates the effect at the start point; every frame hands
        // it a freshly jittered polyline.
        CVector const& finish = m_Finish.getThunderboltPoint();
        CVector const& start = m_Start.getThunderboltPoint();
        Segment const seg(start, finish);
        retruxx::vector<CVector> const& points = SegmentFinding(seg);

        if (m_EffectCreated)
        {
            SetPointsOnSubtree(m_EffectNode, PROP_THUNDERBOLT_POINTS_UPDATE, points);
        }
        else if (thunderboltPrototype->m_EffectName.c_str() && strlen(thunderboltPrototype->m_EffectName.c_str()))
        {
            m_EffectNode = PhysicBody::CreateEffectNode(
                thunderboltPrototype->m_EffectName, m_Start.getThunderboltPoint(), IdentityQuaternion, false, 1.0f);
            m_EffectCreated = true;
            // NOTE: the new node is not checked for null.
            SetPointsOnSubtree(m_EffectNode, PROP_THUNDERBOLT_POINTS_INIT, points);
        }
    }

    CVector const& Thunderbolt::ThunderboltSegment::getStartPoint()
    {
        // RVA 0x7E1BC0
        return m_Start.getThunderboltPoint();
    }

    CVector const& Thunderbolt::ThunderboltSegment::getFinishPoint()
    {
        // RVA 0x7E1BD0
        return m_Finish.getThunderboltPoint();
    }

    int Thunderbolt::ThunderboltSegment::getStartObjId() const
    {
        // RVA 0x7E0BA0
        return m_Start.getObjId();
    }

    int Thunderbolt::ThunderboltSegment::getFinishObjId() const
    {
        // RVA 0x7E0BB0
        return m_Finish.getObjId();
    }

    void Thunderbolt::Update(float elapsedTime, unsigned int workTime)
    {
        // RVA 0x7E2E40 - draws the bolt and burns every target a branch ends on, at the point where
        // the branch first meets one of the target's parts.
        Obj::Update(elapsedTime, workTime);
        m_lifeTime.regenerate(elapsedTime);
        if (m_lifeTime.value().get() == m_lifeTime.maxValue().get())
        {
            // NOTE: the bolt still renders and deals damage in the frame it is removed.
            Remove();
        }
        Render();

        ThunderboltPrototypeInfo const* prototype = GetPrototypeInfo();
        float const damage = prototype->m_Damage / prototype->m_flyTime * elapsedTime;

        for (ThunderboltPoint const& targetPoint : m_Targets)
        {
            Obj* obj = theObjects->GetEntityByObjId(targetPoint.getObjId());
            if (!obj)
            {
                continue;
            }
            // NOTE: the target is assumed to be a ComplexPhysicObj (its parts are traced) without a
            // type check.
            ComplexPhysicObj* target = static_cast<ComplexPhysicObj*>(obj);

            CVector hitDir = ZERO_VECTOR;
            CVector hitPos = ZERO_VECTOR;
            CVector normal = ZERO_VECTOR;
            CStr partName;

            for (ThunderboltSegment& segment : m_Segments)
            {
                int const targetId = target->GetId();
                if (segment.getStartObjId() != targetId && segment.getFinishObjId() != targetId)
                {
                    continue;
                }

                // Trace the branch in the target's local space against each part's geometry.
                CMatrix local;
                local.rotTranslate(target->GetRotation(), target->GetMassCenterPosition());
                float const invTx = 0.0f - local._41 * local._11 - local._12 * local._42 - local._13 * local._43;
                float const invTy = 0.0f - local._21 * local._41 - local._22 * local._42 - local._23 * local._43;
                float const invTz = 0.0f - local._31 * local._41 - local._32 * local._42 - local._33 * local._43;

                CVector const segStart = segment.getStartPoint();
                CVector const& f = segment.getFinishPoint();
                CVector const localEnd(local._13 * f.z + local._12 * f.y + f.x * local._11 + invTx,
                    local._23 * f.z + local._22 * f.y + local._21 * f.x + invTy,
                    local._33 * f.z + local._32 * f.y + local._31 * f.x + invTz);
                CVector const& s = segment.getStartPoint();
                CVector const localBegin(local._13 * s.z + local._12 * s.y + s.x * local._11 + invTx,
                    s.x * local._21 + local._23 * s.z + local._22 * s.y + invTy,
                    s.x * local._31 + local._33 * s.z + local._32 * s.y + invTz);
                Segment const localSeg(localBegin, localEnd);

                for (auto const& [name, part] : *target)
                {
                    retruxx::vector<Geom*> geoms;
                    part->GetGeoms(geoms);
                    dContact contact;
                    if (TraceSegment(localSeg, geoms, &contact) == -1)
                    {
                        continue;
                    }
                    float const* cp = contact.geom.pos;
                    CVector const newHitPos(cp[0] * local._11 + cp[2] * local._31 + cp[1] * local._21 + local._41,
                        cp[2] * local._32 + cp[1] * local._22 + local._12 * cp[0] + local._42,
                        cp[2] * local._33 + cp[1] * local._23 + local._13 * cp[0] + local._43);

                    // NOTE: the best hit starts at the world origin, not at infinity, so a hit
                    // farther from the branch start than the origin is ignored.
                    float const oldDistSq = (hitPos.z - segStart.z) * (hitPos.z - segStart.z) +
                        (hitPos.y - segStart.y) * (hitPos.y - segStart.y) + (hitPos.x - segStart.x) * (hitPos.x - segStart.x);
                    float const newDistSq = (newHitPos.z - segStart.z) * (newHitPos.z - segStart.z) +
                        (newHitPos.y - segStart.y) * (newHitPos.y - segStart.y) +
                        (newHitPos.x - segStart.x) * (newHitPos.x - segStart.x);
                    if (oldDistSq > newDistSq)
                    {
                        CVector const& b = localSeg.begin();
                        CVector const& e = localSeg.end();
                        hitDir.z = (e.z - b.z) * local._33 + (e.y - b.y) * local._23 + local._13 * (e.x - b.x);
                        hitDir.x = (e.z - b.z) * local._31 + (e.y - b.y) * local._21 + (e.x - b.x) * local._11;
                        hitDir.y = (e.z - b.z) * local._32 + (e.y - b.y) * local._22 + local._12 * (e.x - b.x);
                        hitPos = newHitPos;
                        float const* cn = contact.geom.normal;
                        normal.x = cn[0] * local._11 + cn[2] * local._31 + cn[1] * local._21;
                        normal.y = cn[2] * local._32 + cn[1] * local._22 + local._12 * cn[0];
                        normal.z = cn[2] * local._33 + cn[1] * local._23 + local._13 * cn[0];
                        partName = part->GetPartName();
                    }
                }
            }

            DamageInfo damageInfo;
            damageInfo.attackerId = m_Origin.getObjId();
            damageInfo.attackingAgentId = GetId();
            damageInfo.gunPrototypeId = GetPrototypeId();
            damageInfo.bDamageFriends = false;
            damageInfo.damage = damage;
            damageInfo.damageType = DAMAGE_ENERGY;
            damageInfo.damagedPartName = partName;
            damageInfo.hitPos = hitPos;
            damageInfo.hitDir = hitDir;
            damageInfo.normal = normal;
            damageInfo.decalId = -1;
            if (target->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
            {
                // A vehicle is told the attacker is the vehicle carrying the launcher.
                Obj* origin = theObjects->GetEntityByObjId(m_Origin.getObjId());
                if (origin)
                {
                    damageInfo.attackerId = static_cast<PhysicBody*>(origin)->GetOwnerId();
                }
            }
            target->InflictDamage(damageInfo);
        }
    }

    void Thunderbolt::Remove()
    {
        // RVA 0x7E0580
        Obj::Remove();
    }

    void Thunderbolt::RenderDebugInfo() const
    {
        // RVA 0x7E29F0
        for (ThunderboltSegment const& constSegment : m_Segments)
        {
            // A copy, because the next SegmentFinding call overwrites the shared result.
            ThunderboltSegment& segment = const_cast<ThunderboltSegment&>(constSegment);
            retruxx::vector<CVector> const points = segment.getThunderboltPoints();
            for (unsigned i = 1; i < points.size(); ++i)
            {
                M3D_APP->DrawLine(points[i - 1], points[i], 0xFFFEEE00);
            }
        }
    }

    void Thunderbolt::setTargets(retruxx::vector<CVector, retruxx::allocator<CVector>> const& targets)
    {
        // RVA 0x7E5AC0
        m_Targets.resize(targets.size(), ThunderboltPoint());
        for (unsigned i = 0; i < targets.size(); ++i)
        {
            m_Targets[i] = ThunderboltPoint(targets[i]);
        }
        m_Segments = getThunderboltSegmentsInt(m_Origin, m_Targets);
    }

    void Thunderbolt::setTargets(retruxx::vector<int, retruxx::allocator<int>> const& targets)
    {
        // RVA 0x7E5950
        m_Targets.resize(targets.size(), ThunderboltPoint());
        for (unsigned i = 0; i < targets.size(); ++i)
        {
            m_Targets[i] = ThunderboltPoint(targets[i], false);
        }
        m_Segments = getThunderboltSegmentsInt(m_Origin, m_Targets);
    }

    retruxx::vector<int, retruxx::allocator<int>> Thunderbolt::getTargets() const
    {
        // RVA 0x7E1CA0
        retruxx::vector<int> result;
        for (ThunderboltPoint const& target : m_Targets)
        {
            result.push_back(target.getObjId());
        }
        return result;
    }

    void Thunderbolt::setOrigin(CVector const& Origin)
    {
        // RVA 0x7E0590
        m_Origin.setPoint(Origin);
    }

    CVector const& Thunderbolt::getOrigin() const
    {
        // RVA 0x7E05B0
        return m_Origin.getPoint();
    }

    void Thunderbolt::setSourceId(int S, bool launcher)
    {
        // RVA 0x7E05C0
        m_Origin.setObjid(S, launcher);
    }

    int Thunderbolt::getSourceId() const
    {
        // RVA 0x7E05E0
        return m_Origin.getObjId();
    }

    bool Thunderbolt::isLookAtPoint(CVector const&, float) const
    {
        // RVA 0x7E05F0
        return true;
    }

    bool Thunderbolt::CanLookAtTarget() const
    {
        // RVA 0x7E0600
        return true;
    }

    retruxx::vector<ai::Thunderbolt::ThunderboltSegment, retruxx::allocator<ai::Thunderbolt::ThunderboltSegment>> Thunderbolt::getThunderboltSegmentsInt(
        ai::Thunderbolt::ThunderboltPoint& org,
        retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint>>& targets)
    {
        // RVA 0x7E5110 - builds the bolt's tree. Close targets get a branch each straight from org;
        // far ones share a trunk to a fork point halfway towards their average, where the targets
        // are split by the vertical plane through the trunk and each half recurses.
        if (targets.empty())
        {
            return {};
        }

        ThunderboltPrototypeInfo const* prototype = GetPrototypeInfo();
        if (targets.size() == 1)
        {
            return {ThunderboltSegment(prototype, org, targets[0])};
        }

        CVector const orgPoint = org.getThunderboltPoint();
        CVector average = ZERO_VECTOR;
        for (ThunderboltPoint& target : targets)
        {
            CVector const& p = target.getThunderboltPoint();
            average.y = (p.y - orgPoint.y) + average.y;
            average.z = (p.z - orgPoint.z) + average.z;
            average.x = (p.x - orgPoint.x) + average.x;
        }
        double const invCount = 1.0 / static_cast<double>(static_cast<unsigned>(targets.size()));
        average.x = static_cast<float>(average.x * invCount);
        average.y = static_cast<float>(average.y * invCount);
        average.z = static_cast<float>(average.z * invCount);

        if (std::sqrt(double(average.z) * average.z + double(average.y) * average.y + double(average.x) * average.x) <=
            prototype->m_AverageSegmentLength * 10.0)
        {
            retruxx::vector<ThunderboltSegment> segments;
            for (ThunderboltPoint& target : targets)
            {
                segments.push_back(ThunderboltSegment(GetPrototypeInfo(), org, target));
            }
            return segments;
        }

        CVector const& forkBase = org.getThunderboltPoint();
        CVector const newOrg(forkBase.x + average.x * 0.5f, forkBase.y + average.y * 0.5f, forkBase.z + average.z * 0.5f);

        // The plane holds the trunk and the vertical: normal = average x up.
        float const nz = average.x - average.y * 0.0f;
        float const ny = average.z * 0.0f - average.x * 0.0f;
        float const nx = average.y * 0.0f - average.z;
        float const invLen = static_cast<float>(
            1.0 / std::sqrt(double(nz) * nz + double(ny) * ny + double(nx) * nx + double(1.1920929e-7f)));
        CVector const planeNormal(nx * invLen, ny * invLen, nz * invLen);
        float const planeDist = planeNormal.x * newOrg.x + planeNormal.z * newOrg.z + planeNormal.y * newOrg.y;

        // NOTE: like ThunderboltLauncher::CanFire, the shipped plane's m_type is uninitialized;
        // this is the general (non-axis-aligned) side test.
        retruxx::vector<ThunderboltPoint> targetsLeft;
        retruxx::vector<ThunderboltPoint> targetsRight;
        for (ThunderboltPoint& target : targets)
        {
            CVector const& p = target.getThunderboltPoint();
            float const side = p.y * planeNormal.y + p.z * planeNormal.z + planeNormal.x * p.x;
            (side - planeDist <= 0.0f ? targetsRight : targetsLeft).push_back(target);
        }

        ThunderboltPoint forkPoint(newOrg);
        retruxx::vector<ThunderboltPoint> dependencies = targets;
        dependencies.push_back(org);
        forkPoint.setDependencies(dependencies);

        retruxx::vector<ThunderboltSegment> segmentsLeft = getThunderboltSegmentsInt(forkPoint, targetsLeft);
        retruxx::vector<ThunderboltSegment> segmentsRight = getThunderboltSegmentsInt(forkPoint, targetsRight);

        retruxx::vector<ThunderboltSegment> segments;
        segments.push_back(ThunderboltSegment(GetPrototypeInfo(), org, forkPoint));
        segments.insert(segments.end(), segmentsLeft.begin(), segmentsLeft.end());
        segments.insert(segments.end(), segmentsRight.begin(), segmentsRight.end());
        return segments;
    }

    void Thunderbolt::calcThunderboltSegments()
    {
        // RVA 0x7E58F0
        m_Segments = getThunderboltSegmentsInt(m_Origin, m_Targets);
    }

    void Thunderbolt::Render()
    {
        // RVA 0x7E2B90
        for (ThunderboltSegment& segment : m_Segments)
        {
            segment.Render(GetPrototypeInfo());
        }
    }
}  // namespace ai
