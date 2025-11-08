#include "formation.h"

#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"
#include "server/utils.h"

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
        RETRUXX_NOT_IMPLEMENTED;
    }

    float FormationPrototypeInfo::GetPolylineLength() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float FormationPrototypeInfo::GetHeadOffset() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    Formation::~Formation() = default;

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
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Formation::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Formation::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Formation::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Formation::GetPositionAndDirectionForVehicle(int numVehicle, int totalVehicles, CVector& position,
        CVector& direction) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned int Formation::GetMaxVehicles() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Formation::GetDistBetweenVehicles() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Formation::SetDistBetweenVehicles(float dist)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Formation::GetPosition() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Formation::SetPosition(const CVector& pos)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Formation::GetDirection() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Formation::SetDirection(const CVector& direction)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Formation::GetAngle() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Formation::SetAngle(float angle)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        this->m_pPath = pPath;
        if (pPath && pPath->GetSize() > 1)
        {
            if (bForceResetPathNum || this->m_numPathPoint == -1)
                this->m_numPathPoint = 1;
        }
        else
        {
            this->m_numPathPoint = -1;
        }
    }

    bool Formation::bIsMoving() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Formation::Update(float elapsedTime, unsigned int workTime)
    {
        if (m_pPath)
        {
            if (m_numPathPoint < m_pPath->GetSize() && m_numPathPoint >= 0)
            {
                CVector curPathPoint;
                ai::GetPathItem(this->m_pPath, m_numPathPoint, curPathPoint);
                auto v6 = curPathPoint.x - this->m_position.x;
                curPathPoint.y = 0.0;

                CVector v9;
                v9.y = 0.0 - this->m_position.y;
                auto v7 = curPathPoint.z - this->m_position.z;
                v9.x = v6;
                v9.z = v7;

                CVector desiredDir = v9.getNormalized();
                _UpdateVelocity(elapsedTime, desiredDir);
                _UpdateDirection(elapsedTime, desiredDir);
                _UpdatePosition(elapsedTime, desiredDir, curPathPoint);
            }
        }
    }

    void Formation::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Formation::generatePositions(int totalVehicles) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Formation::_UpdateDirection(float elapsedTime, const CVector& desiredDir)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Formation::_UpdatePosition(float elapsedTime, const CVector& desiredDir, const CVector& curPathPoint)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Formation::_UpdateVelocity(float elapsedTime, const CVector& desiredDir)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
