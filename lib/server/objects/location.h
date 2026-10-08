#pragma once
#include "base/simplephysicobj.h"
#include <server/relationship.h>
#include <server/components/numericinrangeregenerating.h>

namespace ai
{
    class Npc;

    class LocationPrototypeInfo : public SimplePhysicObjPrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
        LocationPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    };

    class Location : public ai::SimplePhysicObj
    {
        friend class LocationPrototypeInfo;

    protected:
        virtual ~Location() override /* 0x00 */;
        Location(ai::LocationPrototypeInfo const& prototypeInfo);

    private:
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classLocation;
        virtual ai::LocationPrototypeInfo const* GetPrototypeInfo() const override /* 0x00 */;
        virtual int OnEvent(ai::Event const& evn) override /* 0x00 */;

    protected:
        static void __fastcall RegisterProperty(char const* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x00 */;
        virtual void GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>& Props) const override
            /* 0x00 */;
        virtual void GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>& Props) const override
            /* 0x00 */;
        virtual CStr GetPropertyName(int id) const override /* 0x00 */;
        virtual bool SetPropertyById(int propertyId, m3d::AIParam const& newValue) override /* 0x00 */;
        virtual int GetPropertyId(char const* PropertyName) const override /* 0x00 */;

    protected:
        static inline std::map<CStr, int, ai::Obj::LessNoCaseCStr, std::allocator<std::pair<CStr const, int>>>
            m_propertiesMap;
        static inline std::map<
            int,
            enum ai::eGObjPropertySaveStatus,
            std::less<int>,
            std::allocator<std::pair<int const, enum ai::eGObjPropertySaveStatus>>>
            m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;

        using NpcVector = std::vector<ai::Npc*, std::allocator<ai::Npc*>>;

    public:
        enum LocationType
        {
            LOCATION_GENERIC = 0,
            LOCATION_ENTER = 1,
            LOCATION_DEFEND = 2,
            LOCATION_DEPLOY = 3,
            LOCATION_CARAVAN_ARRIVE = 4,
            LOCATION_ATTACK = 5,
            LOCATION_PASSAGE = 6,
        };

        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override
            /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override
            /* 0x00 */;
        virtual void Remove() override /* 0x00 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x00 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        float GetRadius() const;
        void SetRadius(float radius);
        virtual void SetPositionSelf(CVector const& pos) override /* 0x00 */;
        virtual void RenderDebugInfo() const override /* 0x00 */;
        ai::Location::LocationType GetLocationType() const;
        void SetLocationType(ai::Location::LocationType locationType);
        ai::Location* GetCorrespondingPassageLocation() const;
        bool IsObjectInside(int objId) const;
        CStr const& GetPassageAddress() const;
        void SetActive(bool activate);
        bool IsPassageActive() const;
        bool SetPassageActive(bool);
        std::vector<ai::Npc*, std::allocator<ai::Npc*>> const& GetNpcs() const;
        static CStr __fastcall GetLevelNameFromPassageAddress(CStr const& passageAddress);
        static CStr __fastcall GetLocationNameFromPassageAddress(CStr const& passageAddress);
        static void __fastcall Registration();

    protected:
        virtual void _InternalPostLoad() override /* 0x00 */;
        virtual void OnObjectIn(ai::Obj* object) /* 0x1b4 */;
        virtual void OnObjectOut(ai::Obj* object) /* 0x1b8 */;

    private:
        /* 0x0144 */ std::set<int, std::less<int>, std::allocator<int>> m_idsWasInside;
        /* 0x0150 */ ai::NumericInRangeRegenerating<float> m_timeForNextCheck;
        /* 0x0228 */ std::set<m3d::Class*, std::less<m3d::Class*>, std::allocator<m3d::Class*>> m_targetClasses;
        /* 0x0234 */ bool m_bIsActive;
        /* 0x0235 */ char Padding_173[3];
        /* 0x0238 */ float m_lookingTimeOut;
        /* 0x023c */ ai::Location::LocationType m_locationType;
        /* 0x0240 */ std::set<enum ai::eTolerance, std::less<enum ai::eTolerance>, std::allocator<enum ai::eTolerance>>
            m_toleranceSet;
        /* 0x024c */ CStr m_passageAddress;
        /* 0x0258 */ bool m_bPassageActive;
        /* 0x0259 */ char Padding_174[3];
        /* 0x025c */ CStr m_correspondingPassageLocationName;
        /* 0x0268 */ std::vector<ai::Npc*, std::allocator<ai::Npc*>> m_npcs;
        /* 0x0278 */ int m_numFramesPassed;
        bool _MustCheckObject(ai::Obj const* pObj) const;
        void _CheckIncomingOutgoingObjects(
            std::set<ref_ptr<ai::Obstacle>, std::less<ref_ptr<ai::Obstacle>>, std::allocator<ref_ptr<ai::Obstacle>>>&
                seenObstacles);
        void _OnGameStart(ai::Event const& evn);
    }; /* size: 0x027c */
}  // namespace ai
