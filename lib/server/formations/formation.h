#pragma once
#include <server/objects/base/obj.h>

#include "server/path.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/prototypeinfo.h"

namespace ai
{
    class FormationPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        FormationPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        unsigned int GetMaxVehicles() const;
        retruxx::vector<CVector2, retruxx::allocator<CVector2>> const& GetPolylinePoints() const;
        float GetPolylineLength() const;
        float GetHeadOffset() const;
        float GetLinearVelocity() const;
        float GetAngularVelocity() const;
        virtual void PostLoad() override;

    private:
        unsigned int m_maxVehicles;
        retruxx::vector<CVector2, retruxx::allocator<CVector2>> m_polylinePoints;
        float m_polylineLength;
        float m_headOffset;
        unsigned int m_headPosition;
        float m_linearVelocity;
        float m_angularVelocity;
        void loadPolylinePoints(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        void calcPolylineLengths();
    };

    class Formation : public ai::Obj
    {
        friend class FormationPrototypeInfo;

    protected:
        virtual ~Formation() override;

    private:
        Formation(ai::FormationPrototypeInfo const& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classFormation;
        virtual ai::FormationPrototypeInfo const* GetPrototypeInfo() const override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        void GetPositionAndDirectionForVehicle(int numVehicle, int totalVehicles, CVector& position, CVector& direction)
            const;
        unsigned int GetMaxVehicles() const;
        float GetDistBetweenVehicles() const;
        void SetDistBetweenVehicles(float dist);
        CVector GetPosition() const;
        void SetPosition(CVector const& pos);
        CVector GetDirection() const;
        void SetDirection(CVector const& direction);
        float GetAngle() const;
        void SetAngle(float angle);
        float GetLinearVelocity() const;
        void SetLinearVelocity(float linearVelocity);
        float GetAngularVelocity() const;
        void SetAngularVelocity(float angularVelocity);
        void SetPath(ai::Path* pPath, bool bForceResetPathNum);
        bool bIsMoving() const;
        void AddVehicle(ai::Vehicle* vehicle);
        void RemoveVehicle(ai::Vehicle* vehicle);
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void RenderDebugInfo() const override;

    private:
        float m_distBetweenVehicles;
        unsigned int const m_maxVehicles;
        float m_linearVelocity;
        float m_angularVelocity;
        CVector m_position;
        CVector m_direction;
        // generatePositions rebuilds the slot shape from a const accessor.
        mutable retruxx::vector<CVector2, retruxx::allocator<CVector2>> m_positions;
        ai::Path* m_pPath;
        int m_numPathPoint;

        using VehicleList = retruxx::list<ai::Vehicle*, retruxx::allocator<ai::Vehicle*>>;

    private:
        retruxx::list<ai::Vehicle*, retruxx::allocator<ai::Vehicle*>> m_vehicles;
        void generatePositions(int totalVehicles) const;
        void _UpdateDirection(float elapsedTime, CVector const& desiredDir);
        void _UpdatePosition(float elapsedTime, CVector const& desiredDir, CVector const& curPathPoint);
        void _UpdateVelocity(float elapsedTime, CVector const& desiredDir);
    };
}  // namespace ai
