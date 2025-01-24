#pragma once
#include <math/vector2.h>
#include <server/objects/base/complexphysicobj.h>
#include <server/components/functions/memberfunctiononearg.h>
#include <server/components/functions/memberfunctionstwoargsref.h>

namespace ai
{
    class Boss04Drone;
    class Boss04Station;

    class Boss04PrototypeInfo : public ai::ComplexPhysicObjPrototypeInfo
    {
    public:
        using StringVectorVector = retruxx::vector<retruxx::vector<CStr, retruxx::allocator<CStr> >, retruxx::allocator<retruxx::vector<CStr, retruxx::allocator<CStr> > > >;

    public:
        /* 0x0090 */ int m_stationPrototypeId;
        /* 0x0094 */ int m_dronePrototypeId;
        /* 0x0098 */ CVector2 m_timeBetweenDrones;
        /* 0x00a0 */ unsigned int m_maxDrones;
        /* 0x00a4 */ retruxx::vector<retruxx::vector<CStr, retruxx::allocator<CStr> >, retruxx::allocator<retruxx::vector<CStr, retruxx::allocator<CStr> > > > m_stationToPartBindings;
        /* 0x00b4 */ retruxx::vector<int, retruxx::allocator<int> > m_droneSpawningLpIds;
        Boss04PrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;

    private:
        /* 0x00c4 */ CStr m_stationPrototypeName;
        /* 0x00d0 */ CStr m_dronePrototypeName;
        /* 0x00dc */ retruxx::vector<CStr, retruxx::allocator<CStr> > m_droneSpawningLpNames;
    }; /* size: 0x00ec */

    static_assert(sizeof(Boss04PrototypeInfo) == 0x00ec);

    class Boss04 : public ai::ComplexPhysicObj
    {
    public:
        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::Boss04, float, void>;
        using BeforeApplyModifierFloatCallback = ai::MemberFunctionTwoArgsRef<ai::Boss04, ai::Modifier, float, bool>;

    protected:
        virtual  ~Boss04() override /* 0x00 */;

    private:
        Boss04(const ai::Boss04PrototypeInfo& prototypeInfo);
        Boss04(const ai::Boss04&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBoss04;
        virtual const ai::Boss04PrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;

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
        virtual int OnEvent(const ai::Event& evn) override /* 0x00 */;

        enum Boss04State;

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x00 */;
        virtual bool RemoveChild(ai::Obj* pChild) override /* 0x00 */;
        virtual void CreateChildren() override /* 0x00 */;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        virtual void SetPosition(const CVector& pos) override /* 0x00 */;
        virtual void SetRotation(const Quaternion& rot) override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        void NextState();
        float GetMaxHealth() const;
        float GetHealth() const;

    protected:
        virtual void _InternalPostLoad() override /* 0x00 */;
        virtual void _InternalCreateVisualPart() override /* 0x100 */;

        using Boss04StationVector = retruxx::vector<ai::Boss04Station*, retruxx::allocator<ai::Boss04Station*> >;

    private:
        /* 0x014c */ retruxx::vector<ai::Boss04Station*, retruxx::allocator<ai::Boss04Station*> > m_stations;

        using Boss04DroneVector = retruxx::vector<ai::Boss04Drone*, retruxx::allocator<ai::Boss04Drone*> >;

    private:
        /* 0x015c */ retruxx::vector<ai::Boss04Drone*, retruxx::allocator<ai::Boss04Drone*> > m_drones;
        /* 0x016c */ retruxx::vector<CStr, retruxx::allocator<CStr> > m_pathNamesForDrones;
        /* 0x017c */ ai::Boss04::Boss04State m_state;
        void _CreateStations(bool bWithSuspendedPostLoad);
        void _SetState(ai::Boss04::Boss04State newState);
        void _SpawnDrone();
        static const int NUM_STATIONS;
        void _OnBoss04StationDestroyed(const ai::Event& evn);

    public:
        static void __fastcall Registration();
    }; /* size: 0x0180 */

    static_assert(sizeof(Boss04) == 0x0180);
}
