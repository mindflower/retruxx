#include "thunderbolt.h"

namespace ai
{
    ThunderboltPrototypeInfo::ThunderboltPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool ThunderboltPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void ThunderboltPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* ThunderboltPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    Thunderbolt::~Thunderbolt()
    {
        throw std::logic_error("Not implemented");
    }

    Thunderbolt::Thunderbolt(const ai::ThunderboltPrototypeInfo& prototypeInfo) :
        Obj(prototypeInfo),
        m_lifeTime(0.0, 0.0,prototypeInfo.m_flyTime, 1.0)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Thunderbolt::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Thunderbolt::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Thunderbolt::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Thunderbolt::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    const ai::ThunderboltPrototypeInfo* Thunderbolt::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    Thunderbolt::ThunderboltPoint::ThunderboltPoint(const ai::Thunderbolt::ThunderboltPoint& __that)
    {
        throw std::logic_error("Not implemented");
    }

    Thunderbolt::ThunderboltPoint::ThunderboltPoint(const CVector& point)
    {
        throw std::logic_error("Not implemented");
    }

    Thunderbolt::ThunderboltPoint::ThunderboltPoint(const int& objId, bool launcher)
    {
        throw std::logic_error("Not implemented");
    }

    Thunderbolt::ThunderboltPoint::ThunderboltPoint()
    {
        throw std::logic_error("Not implemented");
    }

    int Thunderbolt::ThunderboltPoint::getObjId() const
    {
        throw std::logic_error("Not implemented");
    }

    void Thunderbolt::ThunderboltPoint::setObjid(int objId, bool launcher)
    {
        throw std::logic_error("Not implemented");
    }

    const CVector& Thunderbolt::ThunderboltPoint::getPoint() const
    {
        throw std::logic_error("Not implemented");
    }

    void Thunderbolt::ThunderboltPoint::setPoint(const CVector& point)
    {
        throw std::logic_error("Not implemented");
    }

    void Thunderbolt::ThunderboltPoint::setDependencies(
        const retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint>>&
        dependencies)
    {
        throw std::logic_error("Not implemented");
    }

    const CVector& Thunderbolt::ThunderboltPoint::getThunderboltPoint()
    {
        throw std::logic_error("Not implemented");
    }

    const retruxx::vector<CVector, retruxx::allocator<CVector>>& Thunderbolt::ThunderboltSegment::SegmentFinding(
        const Segment& seg) const
    {
        throw std::logic_error("Not implemented");
    }

    Thunderbolt::ThunderboltSegment::ThunderboltSegment(const ai::Thunderbolt::ThunderboltSegment& __that)
    {
        throw std::logic_error("Not implemented");
    }

    Thunderbolt::ThunderboltSegment::ThunderboltSegment(const ai::ThunderboltPrototypeInfo* thunderboltPrototype,
        const ai::Thunderbolt::ThunderboltPoint& Start, const ai::Thunderbolt::ThunderboltPoint& Finish)
    {
        throw std::logic_error("Not implemented");
    }

    Thunderbolt::ThunderboltSegment::~ThunderboltSegment()
    {
        throw std::logic_error("Not implemented");
    }

    const retruxx::vector<CVector, retruxx::allocator<CVector>>& Thunderbolt::ThunderboltSegment::getThunderboltPoints()
    {
        throw std::logic_error("Not implemented");
    }

    void Thunderbolt::ThunderboltSegment::Render(const ai::ThunderboltPrototypeInfo* thunderboltPrototype)
    {
        throw std::logic_error("Not implemented");
    }

    const CVector& Thunderbolt::ThunderboltSegment::getStartPoint()
    {
        throw std::logic_error("Not implemented");
    }

    const CVector& Thunderbolt::ThunderboltSegment::getFinishPoint()
    {
        throw std::logic_error("Not implemented");
    }

    int Thunderbolt::ThunderboltSegment::getStartObjId() const
    {
        throw std::logic_error("Not implemented");
    }

    int Thunderbolt::ThunderboltSegment::getFinishObjId() const
    {
        throw std::logic_error("Not implemented");
    }

    void Thunderbolt::Update(float elapsedTime, unsigned int workTime)
    {
        throw std::logic_error("Not implemented");
    }

    void Thunderbolt::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    void Thunderbolt::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Thunderbolt::setTargets(const retruxx::vector<CVector, retruxx::allocator<CVector>>& targets)
    {
        throw std::logic_error("Not implemented");
    }

    void Thunderbolt::setTargets(const retruxx::vector<int, retruxx::allocator<int>>& targets)
    {
        throw std::logic_error("Not implemented");
    }

    retruxx::vector<int, retruxx::allocator<int>> Thunderbolt::getTargets() const
    {
        throw std::logic_error("Not implemented");
    }

    void Thunderbolt::setOrigin(const CVector& Origin)
    {
        throw std::logic_error("Not implemented");
    }

    const CVector& Thunderbolt::getOrigin() const
    {
        throw std::logic_error("Not implemented");
    }

    void Thunderbolt::setSourceId(int S, bool launcher)
    {
        throw std::logic_error("Not implemented");
    }

    int Thunderbolt::getSourceId() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Thunderbolt::isLookAtPoint(const CVector& lookAt, float eps) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Thunderbolt::CanLookAtTarget() const
    {
        throw std::logic_error("Not implemented");
    }

    retruxx::vector<ai::Thunderbolt::ThunderboltSegment, retruxx::allocator<ai::Thunderbolt::ThunderboltSegment>>
    Thunderbolt::getThunderboltSegmentsInt(ai::Thunderbolt::ThunderboltPoint& org,
        retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint>>&
        targets)
    {
        throw std::logic_error("Not implemented");
    }

    void Thunderbolt::calcThunderboltSegments()
    {
        throw std::logic_error("Not implemented");
    }

    void Thunderbolt::Render()
    {
        throw std::logic_error("Not implemented");
    }
}
