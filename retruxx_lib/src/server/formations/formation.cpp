#include "formation.h"

#include "core/ini.h"

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Formation)
    RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Formation);

    FormationPrototypeInfo::FormationPrototypeInfo()
    {
        this->m_maxVehicles = 5;
        this->m_polylineLength = 0.0;
        this->m_headOffset = 0.0;
        this->m_linearVelocity = 100.0;
        this->m_headPosition = 0;
        this->m_bIsUpdating = 0;
        this->m_angularVelocity = 0.5;
    }

    bool FormationPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        // TODO: implement FormationPrototypeInfo::LoadFromXML
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        return result;

        //auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        //if (result)
        //{
        //    m3d::SafeFloatAttrib(m_linearVelocity, xmlNode, "LinearVelocity");
        //    m3d::SafeFloatAttrib(m_angularVelocity, xmlNode, "AngularVelocity");
        //    ai::FormationPrototypeInfo::loadPolylinePoints(xmlFile, xmlNode);
        //    return 1;
        //}
        //return result;
    }

    ai::Obj* FormationPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned int FormationPrototypeInfo::GetMaxVehicles() const
    {
        throw std::logic_error("Not implemented");
    }

    const retruxx::vector<CVector2, retruxx::allocator<CVector2>>& FormationPrototypeInfo::GetPolylinePoints() const
    {
        throw std::logic_error("Not implemented");
    }

    float FormationPrototypeInfo::GetPolylineLength() const
    {
        throw std::logic_error("Not implemented");
    }

    float FormationPrototypeInfo::GetHeadOffset() const
    {
        throw std::logic_error("Not implemented");
    }

    float FormationPrototypeInfo::GetLinearVelocity() const
    {
        throw std::logic_error("Not implemented");
    }

    float FormationPrototypeInfo::GetAngularVelocity() const
    {
        throw std::logic_error("Not implemented");
    }

    void FormationPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    void FormationPrototypeInfo::loadPolylinePoints(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void FormationPrototypeInfo::calcPolylineLengths()
    {
        throw std::logic_error("Not implemented");
    }

    Formation::~Formation()
    {
        throw std::logic_error("Not implemented");
    }

    Formation::Formation(const ai::FormationPrototypeInfo& prototypeInfo) : Obj(prototypeInfo),
        m_maxVehicles(prototypeInfo.GetMaxVehicles())
    {
        this->m_pPath = 0;
        this->m_numPathPoint = -1;
        this->m_distBetweenVehicles = 30.0;
        this->m_linearVelocity = prototypeInfo.GetLinearVelocity();
        this->m_angularVelocity = prototypeInfo.GetAngularVelocity();
        this->m_position = CVector(0.0, 0.0, 0.0);
        this->m_direction = CVector(0.0, 0.0, 1.0);
    }

    m3d::Object* Formation::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Formation::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Formation::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    m3d::Class* Formation::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    const ai::FormationPrototypeInfo* Formation::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::GetPositionAndDirectionForVehicle(int numVehicle, int totalVehicles, CVector& position,
        CVector& direction) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned int Formation::GetMaxVehicles() const
    {
        throw std::logic_error("Not implemented");
    }

    float Formation::GetDistBetweenVehicles() const
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::SetDistBetweenVehicles(float dist)
    {
        throw std::logic_error("Not implemented");
    }

    CVector Formation::GetPosition() const
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::SetPosition(const CVector& pos)
    {
        throw std::logic_error("Not implemented");
    }

    CVector Formation::GetDirection() const
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::SetDirection(const CVector& direction)
    {
        throw std::logic_error("Not implemented");
    }

    float Formation::GetAngle() const
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::SetAngle(float angle)
    {
        throw std::logic_error("Not implemented");
    }

    float Formation::GetLinearVelocity() const
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::SetLinearVelocity(float linearVelocity)
    {
        throw std::logic_error("Not implemented");
    }

    float Formation::GetAngularVelocity() const
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::SetAngularVelocity(float angularVelocity)
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::SetPath(ai::Path* pPath, bool bForceResetPathNum)
    {
        throw std::logic_error("Not implemented");
    }

    bool Formation::bIsMoving() const
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::AddVehicle(ai::Vehicle* vehicle)
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::RemoveVehicle(ai::Vehicle* vehicle)
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::Update(float elapsedTime, unsigned int workTime)
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::generatePositions(int totalVehicles) const
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::_UpdateDirection(float elapsedTime, const CVector& desiredDir)
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::_UpdatePosition(float elapsedTime, const CVector& desiredDir, const CVector& curPathPoint)
    {
        throw std::logic_error("Not implemented");
    }

    void Formation::_UpdateVelocity(float elapsedTime, const CVector& desiredDir)
    {
        throw std::logic_error("Not implemented");
    }
}
