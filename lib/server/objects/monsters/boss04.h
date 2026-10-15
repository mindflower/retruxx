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
        using StringVectorVector = retruxx::vector<
            retruxx::vector<CStr, retruxx::allocator<CStr>>,
            retruxx::allocator<retruxx::vector<CStr, retruxx::allocator<CStr>>>>;

    public:
        int m_stationPrototypeId;
        int m_dronePrototypeId;
        CVector2 m_timeBetweenDrones;
        unsigned int m_maxDrones;
        retruxx::vector<
            retruxx::vector<CStr, retruxx::allocator<CStr>>,
            retruxx::allocator<retruxx::vector<CStr, retruxx::allocator<CStr>>>>
            m_stationToPartBindings;
        retruxx::vector<int, retruxx::allocator<int>> m_droneSpawningLpIds;
        Boss04PrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;

    private:
        CStr m_stationPrototypeName;
        CStr m_dronePrototypeName;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_droneSpawningLpNames;
    };

    class Boss04 : public ai::ComplexPhysicObj
    {
        friend class Boss04PrototypeInfo;

    public:
        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::Boss04, float, void>;
        using BeforeApplyModifierFloatCallback = ai::MemberFunctionTwoArgsRef<ai::Boss04, ai::Modifier, float, bool>;

    protected:
        virtual ~Boss04() override;

    private:
        Boss04(ai::Boss04PrototypeInfo const& prototypeInfo);
        Boss04(ai::Boss04 const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBoss04;
        virtual ai::Boss04PrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        static void RegisterProperty(char const*, int, ai::eGObjPropertySaveStatus);

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
        static inline retruxx::
            map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int>>>
                m_propertiesMap;
        static inline retruxx::map<
            int,
            enum ai::eGObjPropertySaveStatus,
            retruxx::less<int>,
            retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus>>>
            m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        virtual int OnEvent(ai::Event const& evn) override;

        enum Boss04State
        {
            STATE_WAITING = 0,
            STATE_ACTION = 1,
            STATE_DEAD = 2,
        };

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pChild) override;
        virtual void CreateChildren() override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SetPosition(CVector const& pos) override;
        virtual void SetRotation(Quaternion const& rot) override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        void NextState();
        float GetMaxHealth() const;
        float GetHealth() const;

    protected:
        virtual void _InternalPostLoad() override;
        virtual void _InternalCreateVisualPart() override;

        using Boss04StationVector = retruxx::vector<ai::Boss04Station*, retruxx::allocator<ai::Boss04Station*>>;

    private:
        retruxx::vector<ai::Boss04Station*, retruxx::allocator<ai::Boss04Station*>> m_stations;

        using Boss04DroneVector = retruxx::vector<ai::Boss04Drone*, retruxx::allocator<ai::Boss04Drone*>>;

    private:
        retruxx::vector<ai::Boss04Drone*, retruxx::allocator<ai::Boss04Drone*>> m_drones;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_pathNamesForDrones;
        ai::Boss04::Boss04State m_state;
        void _CreateStations(bool bWithSuspendedPostLoad);
        void _SetState(ai::Boss04::Boss04State newState);
        void _SpawnDrone();
        static int const NUM_STATIONS;
        void _OnBoss04StationDestroyed(ai::Event const& evn);

    public:
        static void Registration();
    };
}  // namespace ai
