#include "vehiclerecollection.h"
#include "base/objcontainer.h"
#include <stdexcept>
#include <algorithm>
#include "base/globalproperties.h"
#include "vehicle.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRecollection)
        RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRecollection);

    ai::Obj* VehicleRecollectionPrototypeInfo::CreateTargetObject() const
    {
        return new VehicleRecollection(*this);
    }

    VehicleRecollectionPrototypeInfo::VehicleRecollectionPrototypeInfo() = default;

    bool VehicleRecollectionPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode) != 0;
    }

    VehicleRecollection::ReollectionItem::ReollectionItem(CVector const& _pos, float _time)
    {
        pos = _pos;
        time = _time;
    }

    void VehicleRecollection::ReollectionItem::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRecollection::ReollectionItem::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRecollection::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRecollection::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRecollection::Update(float, unsigned)
    {
        const float currentTime = theObjects->GetGameTimeDiff();
        const float removalThreshold = currentTime - (theGlobProp.m_gameTimeMult * 3.0f);

        if (!m_recollectionItems.empty() && m_recollectionItems.back().time > currentTime)
        {
            m_recollectionItems.clear();
        }

        // Remove old entries that exceed the time threshold
        while (!m_recollectionItems.empty())
        {
            const auto& oldestItem = m_recollectionItems.front();
            if (removalThreshold <= oldestItem.time)
            {
                break;
            }

            // Remove the oldest item
            m_recollectionItems.erase(m_recollectionItems.begin());
        }

        // Check if we should add a new recollection entry
        const bool shouldAddNewEntry =
            m_recollectionItems.empty() || (currentTime > (m_recollectionItems.back().time + theGlobProp.m_gameTimeMult * 0.3f));

        if (shouldAddNewEntry)
        {
            if (m_vehicleId >= 0)
            {
                // Get the object record for the vehicle
                auto* vehicleObj = RT_DYNCAST(theObjects->GetEntityByObjId(m_vehicleId), Vehicle);
                if (vehicleObj != nullptr)
                {
                    // Create new recollection item with current position
                    ReollectionItem newItem(vehicleObj->GetGeometricCenter(), currentTime);
                    m_recollectionItems.push_back(std::move(newItem));
                    return;
                }
            }

            // If we get here, the vehicle is no longer valid - remove this recollection
            Remove();
        }
    }

    void VehicleRecollection::Clear()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::VehicleRecollectionPrototypeInfo const* VehicleRecollection::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRecollection::GetClass() const
    {
        return RT_CLASS_LOCAL(VehicleRecollection);
    }

    VehicleRecollection::VehicleRecollection(ai::VehicleRecollectionPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        m_vehicleId = -1;
    }

    // Helper function to check if time is within an interval (with epsilon tolerance)
    bool IsTimeInInterval(float time, float startTime, float endTime, float epsilon)
    {
        if (endTime <= startTime)
        {
            return (time >= (endTime - epsilon)) && (time <= (startTime + epsilon));
        }
        else
        {
            return (time >= (startTime - epsilon)) && (time <= (endTime + epsilon));
        }
    }

    CVector VehicleRecollection::GetRecollectionPosition(float time) const
    {
        // TODO: generated code VehicleRecollection::GetRecollectionPosition
        if (m_recollectionItems.empty())
        {
            auto* physObj = RT_DYNCAST(theObjects->GetEntityByObjId(m_vehicleId), PhysicObj);
            if (physObj)
            {
                return physObj->GetPosition();
            }
            return ZeroVector;
        }

        // Get recollection items array info
        ai::VehicleRecollection::ReollectionItem const* firstItem = &this->m_recollectionItems.front();
        int itemCount = this->m_recollectionItems.size();

        // Handle single item case
        if (itemCount == 1)
        {
            return firstItem->pos;
        }

        // Search for the appropriate time interval
        unsigned int foundIndex = 1;
        float const epsilon = 0.0001f;  // 0.000099999997

        // Optimized search through recollection items
        if (itemCount >= 4)
        {
            float const* timePtr = &firstItem[1].time;

            for (unsigned int i = 1; i <= itemCount - 4; i += 4)
            {
                // Check first interval in this block
                float time1 = timePtr[0];
                float prevTime1 = timePtr[-5];

                if (!IsTimeInInterval(time, prevTime1, time1, epsilon))
                {
                    // Check second interval
                    float time2 = timePtr[5];
                    float prevTime2 = timePtr[0];

                    if (!IsTimeInInterval(time, prevTime2, time2, epsilon))
                    {
                        // Check third interval
                        float time3 = timePtr[10];
                        float prevTime3 = timePtr[5];

                        if (!IsTimeInInterval(time, prevTime3, time3, epsilon))
                        {
                            // Check fourth interval
                            float time4 = timePtr[15];
                            float prevTime4 = timePtr[10];

                            if (!IsTimeInInterval(time, prevTime4, time4, epsilon))
                            {
                                timePtr += 20;
                                foundIndex += 4;
                                continue;
                            }
                            foundIndex += 3;
                            break;
                        }
                        foundIndex += 2;
                        break;
                    }
                    foundIndex += 1;
                    break;
                }
                break;
            }
        }

        // Linear search for remaining items
        for (; foundIndex < (unsigned int)itemCount; foundIndex++)
        {
            float currentTime = firstItem[foundIndex].time;
            float prevTime = firstItem[foundIndex - 1].time;

            if (IsTimeInInterval(time, prevTime, currentTime, epsilon))
            {
                break;
            }
        }

        CVector result;

        // Handle different interpolation cases
        if (foundIndex == (unsigned int)itemCount)
        {
            // Time is before first item or after last item
            if (time < firstItem->time)
            {
                // Extrapolate backwards from first interval
                float timeDiff = time - firstItem->time;
                float intervalDuration = firstItem[1].time - firstItem->time;
                float invDuration = 1.0f / intervalDuration;

                result.x = firstItem->pos.x + ((firstItem[1].pos.x - firstItem->pos.x) * invDuration * timeDiff);
                result.y = firstItem->pos.y + ((firstItem[1].pos.y - firstItem->pos.y) * invDuration * timeDiff);
                result.z = firstItem->pos.z + ((firstItem[1].pos.z - firstItem->pos.z) * invDuration * timeDiff);
            }
            else if (time > firstItem[itemCount - 1].time)
            {
                // Extrapolate forwards from last interval
                float timeDiff = time - firstItem[itemCount - 1].time;
                float intervalDuration = firstItem[itemCount - 1].time - firstItem[itemCount - 2].time;
                float invDuration = 1.0f / intervalDuration;

                result.x = firstItem[itemCount - 1].pos.x + ((firstItem[itemCount - 1].pos.x - firstItem[itemCount - 2].pos.x) * invDuration * timeDiff);
                result.y = firstItem[itemCount - 1].pos.y + ((firstItem[itemCount - 1].pos.y - firstItem[itemCount - 2].pos.y) * invDuration * timeDiff);
                result.z = firstItem[itemCount - 1].pos.z + ((firstItem[itemCount - 1].pos.z - firstItem[itemCount - 2].pos.z) * invDuration * timeDiff);
            }
            else
            {
                // Should not happen if search is correct
                result = ZeroVector;
            }
        }
        else
        {
            // Normal interpolation between two recollection items
            ai::VehicleRecollection::ReollectionItem const* prevItem = &firstItem[foundIndex - 1];
            ai::VehicleRecollection::ReollectionItem const* currItem = &firstItem[foundIndex];

            float timeDiff = time - prevItem->time;
            float intervalDuration = currItem->time - prevItem->time;
            float invDuration = 1.0f / intervalDuration;
            float t = timeDiff * invDuration;

            // Linear interpolation
            result.x = prevItem->pos.x + (currItem->pos.x - prevItem->pos.x) * t;
            result.y = prevItem->pos.y + (currItem->pos.y - prevItem->pos.y) * t;
            result.z = prevItem->pos.z + (currItem->pos.z - prevItem->pos.z) * t;
        }

        return result;
    }

    m3d::Class* VehicleRecollection::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    Vehicle* VehicleRecollection::GetVehicle() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRecollection::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRecollection::SetVehicle(Vehicle const* vehicle)
    {
        m_vehicleId = vehicle->GetId();
    }

    VehicleRecollection::~VehicleRecollection() = default;

    void VehicleRecollection::_InternalCreateVisualPart()
    {
        m_recollectionItems.clear();
    }

    m3d::Object* VehicleRecollection::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRecollection::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
