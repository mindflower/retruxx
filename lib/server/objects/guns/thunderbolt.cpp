#include "thunderbolt.h"
#include "core/ini.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Thunderbolt)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Thunderbolt);

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
        // TODO: implement ::PostLoad
    }

    ai::Obj* ThunderboltPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::~Thunderbolt()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::Thunderbolt(ai::ThunderboltPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo), m_lifeTime(0.0, 0.0, prototypeInfo.m_flyTime, 1.0)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Thunderbolt::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Thunderbolt::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Thunderbolt::GetBaseClass()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Thunderbolt::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::ThunderboltPrototypeInfo const* Thunderbolt::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::ThunderboltPoint::ThunderboltPoint(ai::Thunderbolt::ThunderboltPoint const& __that)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::ThunderboltPoint::ThunderboltPoint(CVector const& point)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::ThunderboltPoint::ThunderboltPoint(int const& objId, bool launcher)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::ThunderboltPoint::ThunderboltPoint()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Thunderbolt::ThunderboltPoint::getObjId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::ThunderboltPoint::setObjid(int objId, bool launcher)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector const& Thunderbolt::ThunderboltPoint::getPoint() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::ThunderboltPoint::setPoint(CVector const& point)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::ThunderboltPoint::setDependencies(
        retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint>> const& dependencies)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector const& Thunderbolt::ThunderboltPoint::getThunderboltPoint()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<CVector, retruxx::allocator<CVector>> const& Thunderbolt::ThunderboltSegment::SegmentFinding(Segment const& seg) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::ThunderboltSegment::ThunderboltSegment(ai::Thunderbolt::ThunderboltSegment const& __that)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::ThunderboltSegment::ThunderboltSegment(
        ai::ThunderboltPrototypeInfo const* thunderboltPrototype,
        ai::Thunderbolt::ThunderboltPoint const& Start,
        ai::Thunderbolt::ThunderboltPoint const& Finish)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::ThunderboltSegment::~ThunderboltSegment()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<CVector, retruxx::allocator<CVector>> const& Thunderbolt::ThunderboltSegment::getThunderboltPoints()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::ThunderboltSegment::Render(ai::ThunderboltPrototypeInfo const* thunderboltPrototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector const& Thunderbolt::ThunderboltSegment::getStartPoint()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector const& Thunderbolt::ThunderboltSegment::getFinishPoint()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Thunderbolt::ThunderboltSegment::getStartObjId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Thunderbolt::ThunderboltSegment::getFinishObjId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::Update(float elapsedTime, unsigned int workTime)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::Remove()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::setTargets(retruxx::vector<CVector, retruxx::allocator<CVector>> const& targets)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::setTargets(retruxx::vector<int, retruxx::allocator<int>> const& targets)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<int, retruxx::allocator<int>> Thunderbolt::getTargets() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::setOrigin(CVector const& Origin)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector const& Thunderbolt::getOrigin() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::setSourceId(int S, bool launcher)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Thunderbolt::getSourceId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Thunderbolt::isLookAtPoint(CVector const& lookAt, float eps) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Thunderbolt::CanLookAtTarget() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<ai::Thunderbolt::ThunderboltSegment, retruxx::allocator<ai::Thunderbolt::ThunderboltSegment>> Thunderbolt::getThunderboltSegmentsInt(
        ai::Thunderbolt::ThunderboltPoint& org,
        retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint>>& targets)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::calcThunderboltSegments()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::Render()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
