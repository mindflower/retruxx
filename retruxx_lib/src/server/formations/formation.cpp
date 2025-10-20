#include "formation.h"

#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"

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
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_linearVelocity, xmlNode, "LinearVelocity");
            m3d::SafeFloatAttrib(m_angularVelocity, xmlNode, "AngularVelocity");
            ai::FormationPrototypeInfo::loadPolylinePoints(xmlFile, xmlNode);
            return 1;
        }
        return result;
    }

    ai::Obj* FormationPrototypeInfo::CreateTargetObject() const
    {
        return new Formation(*this);
    }

    unsigned int FormationPrototypeInfo::GetMaxVehicles() const
    {
        return this->m_maxVehicles;
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
        return this->m_linearVelocity;
    }

    float FormationPrototypeInfo::GetAngularVelocity() const
    {
        return this->m_angularVelocity;
    }

    void FormationPrototypeInfo::PostLoad()
    {
        calcPolylineLengths();
    }

    void FormationPrototypeInfo::loadPolylinePoints(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        ref_ptr polylineNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(polylineNode, "Polyline");

        ref_ptr pointNode = xmlFile->CreateNode();
        for (polylineNode->GetFirstChild(pointNode, "Point"); !pointNode->IsEmpty(); pointNode->GetNextSibling(pointNode, "Point"))
        {
            CVector2 point;
            m3d::SafeVector2Attrib(point, pointNode, "Coord");
            if (point.x == 0.0 && point.y == 0.0)
            {
                this->m_headPosition = m_polylinePoints.size();
            }
            this->m_polylinePoints.push_back(point);
        }
    }

    void FormationPrototypeInfo::calcPolylineLengths()
    {
        // TODO: generated code
        // Reset accumulated lengths
        this->m_polylineLength = 0.0f;
        this->m_headOffset = 0.0f;

        // Check if we have enough points to form segments
        if (this->m_polylinePoints.empty() || this->m_polylinePoints.size() < 2)
            return;

        // Iterate through each segment of the polyline
        for (size_t i = 0; i < this->m_polylinePoints.size() - 1; ++i)
        {
            const CVector2& currentPoint = this->m_polylinePoints[i];
            const CVector2& nextPoint = this->m_polylinePoints[i + 1];

            // Calculate segment length using distance formula
            float dx = nextPoint.x - currentPoint.x;
            float dy = nextPoint.y - currentPoint.y;
            float segmentLength = sqrt(dx * dx + dy * dy);

            // Add to total polyline length
            this->m_polylineLength += segmentLength;

            // If this segment is before the head position, add to head offset
            if (i < this->m_headPosition)
            {
                this->m_headOffset += segmentLength;
            }
        }
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
        return RT_CLASS_LOCAL(Formation);
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
        return m_linearVelocity;
    }

    void Formation::SetLinearVelocity(float linearVelocity)
    {
        m_linearVelocity = linearVelocity;
    }

    float Formation::GetAngularVelocity() const
    {
        return m_angularVelocity;
    }

    void Formation::SetAngularVelocity(float angularVelocity)
    {
        this->m_angularVelocity = angularVelocity;
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
        if (m_vehicles.size() >= this->m_maxVehicles)
        {
            M3D_CRITICAL_ERROR("maximum vehicles count exceeded: " + CStr(m_maxVehicles) + " for " + GetDebugDescription());
        }
        m_vehicles.push_back(vehicle);
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
