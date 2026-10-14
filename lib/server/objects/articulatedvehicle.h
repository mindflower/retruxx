#pragma once
#include "vehicle.h"

namespace ai
{
    class ArticulatedVehiclePrototypeInfo : public ai::VehiclePrototypeInfo
    {
    protected:
        virtual void _InternalCopyFrom(ai::PrototypeInfo const& rhs) override;

    public:
        int m_trailerPrototypeId;
        ArticulatedVehiclePrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        virtual void PostLoad() override;

    private:
        CStr m_trailerPrototypeName;
    };

    class ArticulatedVehicle : public ai::Vehicle
    {
        friend class ArticulatedVehiclePrototypeInfo;

    protected:
        virtual ~ArticulatedVehicle() override;

    private:
        ArticulatedVehicle(ai::ArticulatedVehiclePrototypeInfo const& prototypeInfo);
        ArticulatedVehicle(ai::ArticulatedVehicle const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classArticulatedVehicle;
        virtual ai::ArticulatedVehiclePrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(char const*, int, ai::eGObjPropertySaveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(
            retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const override;
        virtual void GetPropertiesIDs(
            retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, m3d::AIParam const& newValue) override;
        virtual int GetPropertyId(char const* PropertyName) const override;

    protected:
        static retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int>>>
            m_propertiesMap;
        static retruxx::map<
            int,
            enum ai::eGObjPropertySaveStatus,
            retruxx::less<int>,
            retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus>>>
            m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        virtual void CreateChildren() override;
        virtual void Remove() override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pObj) override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void SetPassedToAnotherMapStatus() override;
        virtual void SetPositionSelf(CVector const& pos) override;
        virtual void SetRotationSelf(Quaternion const& rot) override;
        ai::Vehicle* GetTrailer();

    protected:
        virtual void _KeepSteer(float elapsedTime) override;
        virtual void _InternalPostLoad() override;

    private:
        int m_trailerObjId;
        dxJoint* m_trailerJoint;
        CVector m_relJointPosOnMe;
        CVector m_relJointPosOnTrailer;
        CStr _GetTrailerName() const;
        ai::Vehicle* _GetTrailer() const;
        void _AdjustTrailerPosition();
    };
}  // namespace ai
