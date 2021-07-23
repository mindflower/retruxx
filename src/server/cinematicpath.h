#pragma once
#include <vector>

class CStr;

namespace m3d
{
    namespace cmn
    {
        class XmlFile;
        class XmlNode;
    }
}

class CVector;
class CVector2;

namespace ai
{
    class CinematicPath
    {
    public:
        void ShiftCameraPoints(CVector const&);
        void SetCameraPoints(std::vector<CVector, std::allocator<CVector> > const&);
        void clear();
        std::vector<CVector, std::allocator<CVector> > const& GetCameraPoints() const;
        void SetVehiclePoints(std::vector<CVector2, std::allocator<CVector2> > const&);
        void LoadFromXml(CStr const&, m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        void RenderDebugInfo() const;
        std::vector<CVector2, std::allocator<CVector2> > const& GetVehiclePoints() const;
        void SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;

    private:
        std::vector<CVector2> m_vehiclePoints;
        std::vector<CVector> m_cameraPoints;
    };
}
