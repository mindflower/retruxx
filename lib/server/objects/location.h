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
        virtual ~Location() override;
        Location(ai::LocationPrototypeInfo const& prototypeInfo);

    private:
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classLocation;
        virtual ai::LocationPrototypeInfo const* GetPrototypeInfo() const override;
        virtual int OnEvent(ai::Event const& evn) override;

    protected:
        static void __fastcall RegisterProperty(char const* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>& Props) const override;
        virtual void GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, m3d::AIParam const& newValue) override;
        virtual int GetPropertyId(char const* PropertyName) const override;

    protected:
        static inline std::map<CStr, int, ai::Obj::LessNoCaseCStr, std::allocator<std::pair<CStr const, int>>>
            m_propertiesMap;
        static inline std::map<
            int,
            enum ai::eGObjPropertySaveStatus,
            std::less<int>,
            std::allocator<std::pair<int const, enum ai::eGObjPropertySaveStatus>>>
            m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

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

        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void Remove() override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        float GetRadius() const;
        void SetRadius(float radius);
        virtual void SetPositionSelf(CVector const& pos) override;
        virtual void RenderDebugInfo() const override;
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
        virtual void _InternalPostLoad() override;
        virtual void OnObjectIn(ai::Obj* object);
        virtual void OnObjectOut(ai::Obj* object);

    private:
        std::set<int, std::less<int>, std::allocator<int>> m_idsWasInside;
        ai::NumericInRangeRegenerating<float> m_timeForNextCheck;
        std::set<m3d::Class*, std::less<m3d::Class*>, std::allocator<m3d::Class*>> m_targetClasses;
        bool m_bIsActive;
        float m_lookingTimeOut;
        ai::Location::LocationType m_locationType;
        std::set<enum ai::eTolerance, std::less<enum ai::eTolerance>, std::allocator<enum ai::eTolerance>>
            m_toleranceSet;
        CStr m_passageAddress;
        bool m_bPassageActive;
        CStr m_correspondingPassageLocationName;
        std::vector<ai::Npc*, std::allocator<ai::Npc*>> m_npcs;
        int m_numFramesPassed;
        bool _MustCheckObject(ai::Obj const* pObj) const;
        void _CheckIncomingOutgoingObjects(
            std::set<ref_ptr<ai::Obstacle>, std::less<ref_ptr<ai::Obstacle>>, std::allocator<ref_ptr<ai::Obstacle>>>&
                seenObstacles);
        void _OnGameStart(ai::Event const& evn);
    };
}  // namespace ai
