#include "formation.h"
#include "server/objects/base/prototypemanager.h"

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
        // RVA 0x7ECA70
        return m_polylinePoints;
    }

    float FormationPrototypeInfo::GetPolylineLength() const
    {
        // RVA 0x7ECA50
        return m_polylineLength;
    }

    float FormationPrototypeInfo::GetHeadOffset() const
    {
        // RVA 0x7ECA60
        return m_headOffset;
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
        // RVA 0x7ED900 - measures the shape once: the whole polyline, and how far along it
        // the head vehicle's point sits.
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
        // RVA 0x7ED580
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* Formation::CreateObject()
    {
        // RVA 0x7ED740
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
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
        // RVA 0x7EDE60
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), FormationPrototypeInfo const);
    }

    void Formation::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7ED980
        Obj::SaveToXML(xmlFile, xmlNode);
        xmlNode->SetAttribute("DistBetweenVehicles", CStr(m_distBetweenVehicles).c_str());
        xmlNode->SetAttribute("Pos", CStr(m_position).c_str());
        xmlNode->SetAttribute("Direction", CStr(m_direction).c_str());
        xmlNode->SetAttribute("LinearVelocity", CStr(m_linearVelocity).c_str());
        xmlNode->SetAttribute("AngularVelocity", CStr(m_angularVelocity).c_str());
        xmlNode->SetAttribute("NumPathPoint", CStr(m_numPathPoint).c_str());
    }

    void Formation::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        // RVA 0x7ECD80
        Obj::LoadFromXML(xmlFile, xmlNode);
        m3d::SafeFloatAttrib(m_distBetweenVehicles, xmlNode, "DistBetweenVehicles");
        m3d::SafeVectorAttrib(m_position, xmlNode, "Pos");
        m3d::SafeVectorAttrib(m_direction, xmlNode, "Direction");
        m3d::SafeFloatAttrib(m_linearVelocity, xmlNode, "LinearVelocity");
        m3d::SafeFloatAttrib(m_angularVelocity, xmlNode, "AngularVelocity");
        m3d::SafeIntAttrib(m_numPathPoint, xmlNode, "NumPathPoint");
    }

    void Formation::GetPositionAndDirectionForVehicle(int numVehicle, int totalVehicles, CVector& position,
        CVector& direction) const
    {
        // RVA 0x7EE9D0 - the slot shape is generated in formation-local units and then scaled by
        // the spacing and turned to face the way the formation is heading.
        if (totalVehicles != static_cast<int>(m_positions.size()))
        {
            generatePositions(totalVehicles);
        }

        CVector const relPos(m_positions[numVehicle].x * m_distBetweenVehicles, 0.0f,
            m_positions[numVehicle].y * m_distBetweenVehicles);

        CVector const INITIAL_UP_DIRECTION(0.0, 1.0, 0.0);
        Quaternion rot;
        rot.FromAxisAngle(INITIAL_UP_DIRECTION, GetAngle());
        CMatrix const rotMatrix = rot.ToMatrix();

        position.x = m_position.x + rotMatrix._11 * relPos.x + rotMatrix._21 * relPos.y +
            rotMatrix._31 * relPos.z;
        position.y = m_position.y + rotMatrix._12 * relPos.x + rotMatrix._22 * relPos.y +
            rotMatrix._32 * relPos.z;
        position.z = m_position.z + rotMatrix._13 * relPos.x + rotMatrix._23 * relPos.y +
            rotMatrix._33 * relPos.z;

        // Every vehicle in a formation faces the same way.
        direction = m_direction;
    }

    unsigned int Formation::GetMaxVehicles() const
    {
        // RVA 0x7ECB30 - NOTE: this reads the prototype's limit, not the m_maxVehicles the
        // constructor copied into the object.
        return GetPrototypeInfo()->GetMaxVehicles();
    }

    float Formation::GetDistBetweenVehicles() const
    {
        // RVA 0x7ECB40
        return m_distBetweenVehicles;
    }

    void Formation::SetDistBetweenVehicles(float dist)
    {
        // RVA 0x7ECB50
        m_distBetweenVehicles = dist;
    }

    CVector Formation::GetPosition() const
    {
        // RVA 0x7ECE70
        return m_position;
    }

    void Formation::SetPosition(const CVector& pos)
    {
        // RVA 0x7ECA80 - a formation is a flat shape, so it never carries a height.
        m_position = pos;
        m_position.y = 0.0f;
    }

    CVector Formation::GetDirection() const
    {
        return m_direction;
    }

    void Formation::SetDirection(const CVector& direction)
    {
        // RVA 0x7ECAB0 - NOTE: unlike SetPosition this keeps whatever y it is handed.
        m_direction = direction;
    }

    float Formation::GetAngle() const
    {
        // RVA 0x7ECED0 - NOTE: the arguments are (x, z), not the usual (z, x), so the angle runs
        // the opposite way round from atan2 of a heading.
        return atan2f(m_direction.x, m_direction.z);
    }

    void Formation::SetAngle(float angle)
    {
        // RVA 0x7ECEE0 - the inverse of GetAngle, so cos feeds x and sin feeds z.
        m_direction.x = cosf(angle);
        m_direction.y = 0.0f;
        m_direction.z = sinf(angle);
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
        if (m_pPath)
        {
            if (m_numPathPoint < m_pPath->GetSize() && m_numPathPoint > 0)
            {
                return true;
            }
        }
        return false;
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
        // RVA 0x7EDD90
        auto const it = std::find(m_vehicles.begin(), m_vehicles.end(), vehicle);
        if (it != m_vehicles.end())
        {
            m_vehicles.erase(it);
        }
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
        // RVA 0x7EED50 - draws a ring at each slot the formation is currently holding open.
        int const totalVehicles = static_cast<int>(m_positions.size());
        for (int i = 0; i < totalVehicles; ++i)
        {
            CVector pos;
            CVector dummy;
            GetPositionAndDirectionForVehicle(i, totalVehicles, pos, dummy);
            ai::DebugCircle(pos, 3.0f, 0xFF00FFFF);
        }
    }

    void Formation::generatePositions(int totalVehicles) const
    {
        // RVA 0x7EE6D0 - walks the prototype's polyline dropping one slot every unit of arc
        // length, starting far enough in that the head vehicle lands on the polyline's head
        // point and the shape still fits either side of it.
        m_positions.clear();
        if (totalVehicles == 1)
        {
            m_positions.push_back(CVector2(0.0f, 0.0f));
            return;
        }

        FormationPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        auto const& points = prototypeInfo->GetPolylinePoints();

        float const halfLength = (totalVehicles - 1) * 0.5f;
        float const lastStart = prototypeInfo->GetPolylineLength() - halfLength;
        float start = prototypeInfo->GetHeadOffset();
        if (start <= halfLength)
        {
            start = halfLength;
        }
        if (start > lastStart)
        {
            start = lastStart;
        }

        float offset = start - halfLength;
        for (unsigned i = 0; i + 1 < points.size(); ++i)
        {
            CVector2 const segment(points[i + 1].x - points[i].x, points[i + 1].y - points[i].y);
            float const lengthSq = segment.x * segment.x + segment.y * segment.y;
            float const segmentLength = sqrtf(lengthSq);

            while (segmentLength >= offset)
            {
                if (static_cast<int>(m_positions.size()) >= totalVehicles)
                {
                    break;
                }
                // A degenerate segment contributes no direction, so the slot sits on its start.
                CVector2 dir(0.0f, 0.0f);
                if (lengthSq > 0.0000099999997f)
                {
                    float const invLength = 1.0f / sqrtf(lengthSq);
                    dir.x = segment.x * invLength;
                    dir.y = segment.y * invLength;
                }
                m_positions.push_back(
                    CVector2(points[i].x + dir.x * offset, points[i].y + dir.y * offset));

                bool const segmentExhausted = segmentLength < offset + 1.0f;
                offset += 1.0f;
                if (segmentExhausted)
                {
                    break;
                }
            }

            if (static_cast<int>(m_positions.size()) >= totalVehicles)
            {
                break;
            }
            // Whatever was left over carries into the next segment.
            offset -= segmentLength;
        }
    }

    void Formation::_UpdateDirection(float elapsedTime, const CVector& desiredDir)
    {
        // RVA 0x7ECF20 - turns towards desiredDir at the formation's angular velocity, and snaps
        // onto it once the turn would carry past it.
        float const side = m_direction.z * desiredDir.x - desiredDir.z * m_direction.x;
        int turnSign = 0;
        if (side > 0.000001f)
        {
            turnSign = 1;
        }
        else if (side < -0.000001f)
        {
            turnSign = -1;
        }

        CVector const INITIAL_UP_DIRECTION(0.0, 1.0, 0.0);
        Quaternion rot;
        rot.FromAxisAngle(INITIAL_UP_DIRECTION, turnSign * m_angularVelocity * elapsedTime);
        CMatrix const rotMatrix = rot.ToMatrix();

        CVector newDir;
        newDir.x = rotMatrix._11 * m_direction.x + rotMatrix._21 * m_direction.y +
            rotMatrix._31 * m_direction.z;
        newDir.y = rotMatrix._12 * m_direction.x + rotMatrix._22 * m_direction.y +
            rotMatrix._32 * m_direction.z;
        newDir.z = rotMatrix._13 * m_direction.x + rotMatrix._23 * m_direction.y +
            rotMatrix._33 * m_direction.z;

        // If the turn crossed the target - the side it lies on flipped - take the target exactly.
        float const newSide = newDir.z * desiredDir.x - desiredDir.z * newDir.x;
        int newSign = 0;
        if (newSide > 0.000001f)
        {
            newSign = 1;
        }
        else if (newSide < -0.000001f)
        {
            newSign = -1;
        }
        if (desiredDir.lengthSq() < 0.0099999998f || newSign * turnSign <= 0)
        {
            newDir = desiredDir;
        }

        float const invLength = 1.0f / sqrtf(newDir.lengthSq() + 0.00000011920929f);
        m_direction = newDir * invLength;
    }

    void Formation::_UpdatePosition(float elapsedTime, const CVector& desiredDir, const CVector& curPathPoint)
    {
        // RVA 0x7ED330 - a step that would carry the formation past the path point it is heading
        // for lands on the point instead, and the path advances.
        CVector const step = desiredDir * (m_linearVelocity * elapsedTime);
        CVector const newPos = m_position + step;

        CVector const toPoint = curPathPoint - newPos;
        if (toPoint.x * desiredDir.x + toPoint.y * desiredDir.y + toPoint.z * desiredDir.z >= 0.0f)
        {
            m_position.x = newPos.x;
            m_position.z = newPos.z;
        }
        else
        {
            m_position.x = curPathPoint.x;
            m_position.z = curPathPoint.z;
            ++m_numPathPoint;
        }
        m_position.y = 0.0f;
    }

    void Formation::_UpdateVelocity(float elapsedTime, const CVector& desiredDir)
    {
        // RVA 0x7EEDB0 - the formation accelerates until it hits twice the speed of its slowest
        // truck, and brakes hard whenever anyone is falling out of position behind it.
        if (m_vehicles.empty())
        {
            return;
        }

        float maxVelocity = 1000000.0f;
        for (auto const* vehicle : m_vehicles)
        {
            float const cruisingSpeed = vehicle->GetCruisingSpeed();
            if (cruisingSpeed < maxVelocity)
            {
                maxVelocity = cruisingSpeed;
            }
        }
        maxVelocity *= 2.0f;

        // How far a truck may trail its slot before the whole formation slows for it.
        float allowedLag = m_distBetweenVehicles * 3.0f;
        if (allowedLag > 30.0f)
        {
            allowedLag = 30.0f;
        }

        int const totalVehicles = static_cast<int>(m_vehicles.size());
        bool bMustDecreaseVelocity = false;
        for (auto* vehicle : m_vehicles)
        {
            CVector const pos = vehicle->GetPosition();
            CVector desiredPos;
            CVector dummy;
            GetPositionAndDirectionForVehicle(
                vehicle->GetIndexInTeam(), totalVehicles, desiredPos, dummy);
            // NOTE: the y term uses desiredPos.y alone rather than the difference, unlike x and z.
            float const lag = desiredDir.x * (desiredPos.x - pos.x) + desiredDir.y * desiredPos.y +
                desiredDir.z * (desiredPos.z - pos.z);
            bMustDecreaseVelocity |= lag > allowedLag;
        }

        if (bMustDecreaseVelocity)
        {
            m_linearVelocity *= 0.5f;
            if (m_linearVelocity < 1.0f)
            {
                m_linearVelocity = 1.0f;
            }
            return;
        }

        float const accelerated = m_linearVelocity * 1.2f;
        m_linearVelocity = accelerated < maxVelocity ? accelerated : maxVelocity;
        if (m_linearVelocity > 1000000.0f)
        {
            M3D_LOG_ERR("Error: linear velocity of formation is too high");
            m_linearVelocity = 1000.0f;
        }
    }
}
