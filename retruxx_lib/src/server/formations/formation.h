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
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
        unsigned int GetMaxVehicles() const;
        const retruxx::vector<CVector2, retruxx::allocator<CVector2> >& GetPolylinePoints() const;
        float GetPolylineLength() const;
        float GetHeadOffset() const;
        float GetLinearVelocity() const;
        float GetAngularVelocity() const;
        virtual void PostLoad() override /* 0x08 */;

    private:
        /* 0x0040 */ unsigned int m_maxVehicles;
        retruxx::vector<CVector2, retruxx::allocator<CVector2> > m_polylinePoints;
        /* 0x0054 */ float m_polylineLength;
        /* 0x0058 */ float m_headOffset;
        /* 0x005c */ unsigned int m_headPosition;
        /* 0x0060 */ float m_linearVelocity;
        /* 0x0064 */ float m_angularVelocity;
        void loadPolylinePoints(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        void calcPolylineLengths();
    }; /* size: 0x0068 */

    class Formation : public ai::Obj
    {
    protected:
        virtual  ~Formation() override /* 0x00 */;

    private:
        Formation(const ai::FormationPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classFormation;
        virtual const ai::FormationPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb4 */;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xac */;
        void GetPositionAndDirectionForVehicle(int numVehicle, int totalVehicles, CVector& position, CVector& direction) const;
        unsigned int GetMaxVehicles() const;
        float GetDistBetweenVehicles() const;
        void SetDistBetweenVehicles(float dist);
        CVector GetPosition() const;
        void SetPosition(const CVector& pos);
        CVector GetDirection() const;
        void SetDirection(const CVector& direction);
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
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x80 */;
        virtual void RenderDebugInfo() const override /* 0xe4 */;

    private:
        /* 0x00c0 */ float m_distBetweenVehicles;
        /* 0x00c4 */ const unsigned int m_maxVehicles;
        /* 0x00c8 */ float m_linearVelocity;
        /* 0x00cc */ float m_angularVelocity;
        CVector m_position;
        CVector m_direction;
        retruxx::vector<CVector2, retruxx::allocator<CVector2> > m_positions;
        /* 0x00f8 */ ai::Path* m_pPath;
        /* 0x00fc */ int m_numPathPoint;

        using VehicleList = retruxx::list<ai::Vehicle*, retruxx::allocator<ai::Vehicle*> >;

    private:
        retruxx::list<ai::Vehicle*, retruxx::allocator<ai::Vehicle*> > m_vehicles;
        void generatePositions(int totalVehicles) const;
        void _UpdateDirection(float elapsedTime, const CVector& desiredDir);
        void _UpdatePosition(float elapsedTime, const CVector& desiredDir, const CVector& curPathPoint);
        void _UpdateVelocity(float elapsedTime, const CVector& desiredDir);
    }; /* size: 0x010c */
}
