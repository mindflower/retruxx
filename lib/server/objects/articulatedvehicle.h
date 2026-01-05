#pragma once
#include "vehicle.h"

namespace ai
{
    class ArticulatedVehiclePrototypeInfo : public ai::VehiclePrototypeInfo
    {
    protected:
        virtual void _InternalCopyFrom(const ai::PrototypeInfo& rhs) override /* 0x00 */;

    public:
        /* 0x012c */ int m_trailerPrototypeId;
        ArticulatedVehiclePrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
        virtual void PostLoad() override /* 0x00 */;

    private:
        /* 0x0130 */ CStr m_trailerPrototypeName;
    }; /* size: 0x013c */

    static_assert(sizeof(ArticulatedVehiclePrototypeInfo) == 0x013c);

    class ArticulatedVehicle : public ai::Vehicle
    {
    protected:
        virtual  ~ArticulatedVehicle() override /* 0x00 */;

    private:
        ArticulatedVehicle(const ai::ArticulatedVehiclePrototypeInfo& prototypeInfo);
        ArticulatedVehicle(const ai::ArticulatedVehicle&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classArticulatedVehicle;
        virtual const ai::ArticulatedVehiclePrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;

    protected:
        static void __fastcall RegisterProperty(const char*, int, ai::eGObjPropertySaveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x00 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x00 */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x00 */;
        virtual CStr GetPropertyName(int id) const override /* 0x00 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x00 */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x00 */;

    protected:
        static retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;

    public:
        virtual void CreateChildren() override /* 0x00 */;
        virtual void Remove() override /* 0x54 */;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x00 */;
        virtual bool RemoveChild(ai::Obj* pObj) override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        virtual void SetPassedToAnotherMapStatus() override /* 0x50 */;
        virtual void SetPositionSelf(const CVector& pos) override /* 0x00 */;
        virtual void SetRotationSelf(const Quaternion& rot) override /* 0x00 */;
        ai::Vehicle* GetTrailer();

    protected:
        virtual void _KeepSteer(float elapsedTime) override /* 0x1cc */;
        virtual void _InternalPostLoad() override /* 0x00 */;

    private:
        /* 0x04f4 */ int m_trailerObjId;
        /* 0x04f8 */ dxJoint* m_trailerJoint;
        /* 0x04fc */ CVector m_relJointPosOnMe;
        /* 0x0508 */ CVector m_relJointPosOnTrailer;
        CStr _GetTrailerName() const;
        ai::Vehicle* _GetTrailer() const;
        void _AdjustTrailerPosition();
    }; /* size: 0x0514 */

    static_assert(sizeof(ArticulatedVehicle) == 0x0514);
}
