#include "thunderbolt.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Thunderbolt)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Thunderbolt);

    ThunderboltPrototypeInfo::ThunderboltPrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ThunderboltPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ThunderboltPrototypeInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::Obj* ThunderboltPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::~Thunderbolt()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::Thunderbolt(const ai::ThunderboltPrototypeInfo& prototypeInfo) :
        Obj(prototypeInfo),
        m_lifeTime(0.0, 0.0,prototypeInfo.m_flyTime, 1.0)
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

    const ai::ThunderboltPrototypeInfo* Thunderbolt::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::ThunderboltPoint::ThunderboltPoint(const ai::Thunderbolt::ThunderboltPoint& __that)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::ThunderboltPoint::ThunderboltPoint(const CVector& point)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::ThunderboltPoint::ThunderboltPoint(const int& objId, bool launcher)
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

    const CVector& Thunderbolt::ThunderboltPoint::getPoint() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::ThunderboltPoint::setPoint(const CVector& point)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::ThunderboltPoint::setDependencies(
        const retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint>>&
        dependencies)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const CVector& Thunderbolt::ThunderboltPoint::getThunderboltPoint()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const retruxx::vector<CVector, retruxx::allocator<CVector>>& Thunderbolt::ThunderboltSegment::SegmentFinding(
        const Segment& seg) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::ThunderboltSegment::ThunderboltSegment(const ai::Thunderbolt::ThunderboltSegment& __that)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::ThunderboltSegment::ThunderboltSegment(const ai::ThunderboltPrototypeInfo* thunderboltPrototype,
        const ai::Thunderbolt::ThunderboltPoint& Start, const ai::Thunderbolt::ThunderboltPoint& Finish)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Thunderbolt::ThunderboltSegment::~ThunderboltSegment()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const retruxx::vector<CVector, retruxx::allocator<CVector>>& Thunderbolt::ThunderboltSegment::getThunderboltPoints()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::ThunderboltSegment::Render(const ai::ThunderboltPrototypeInfo* thunderboltPrototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const CVector& Thunderbolt::ThunderboltSegment::getStartPoint()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const CVector& Thunderbolt::ThunderboltSegment::getFinishPoint()
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

    void Thunderbolt::setTargets(const retruxx::vector<CVector, retruxx::allocator<CVector>>& targets)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::setTargets(const retruxx::vector<int, retruxx::allocator<int>>& targets)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<int, retruxx::allocator<int>> Thunderbolt::getTargets() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Thunderbolt::setOrigin(const CVector& Origin)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const CVector& Thunderbolt::getOrigin() const
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

    bool Thunderbolt::isLookAtPoint(const CVector& lookAt, float eps) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Thunderbolt::CanLookAtTarget() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<ai::Thunderbolt::ThunderboltSegment, retruxx::allocator<ai::Thunderbolt::ThunderboltSegment>>
    Thunderbolt::getThunderboltSegmentsInt(ai::Thunderbolt::ThunderboltPoint& org,
        retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint>>&
        targets)
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
}
