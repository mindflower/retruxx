#pragma once
#include <vector>
#include <core/stringm3d.h>

namespace m3d
{
    namespace cmn
    {
        class XmlFile;
        struct XmlNode;
    }  // namespace cmn
}  // namespace m3d

struct CVector;
struct CVector2;

namespace ai
{
    class CinematicPath
    {
    public:
        void LoadFromXml(CStr const& debugDescription, m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        void SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        void clear();
        std::vector<CVector2, std::allocator<CVector2>> const& GetVehiclePoints() const;
        std::vector<CVector, std::allocator<CVector>> const& GetCameraPoints() const;
        void SetVehiclePoints(std::vector<CVector2, std::allocator<CVector2>> const& vehiclePoints);
        void SetCameraPoints(std::vector<CVector, std::allocator<CVector>> const& cameraPoints);
        void ShiftCameraPoints(CVector const& shiftPos);
        void RenderDebugInfo() const;

    private:
        /* 0x0000 */ std::vector<CVector2, std::allocator<CVector2>> m_vehiclePoints;
        /* 0x0010 */ std::vector<CVector, std::allocator<CVector>> m_cameraPoints;
    }; /* size: 0x0020 */
}  // namespace ai
