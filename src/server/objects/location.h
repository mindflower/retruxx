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


    class Location : public SimplePhysicObj
    {
    public:
        enum LocationType
        {
            LOCATION_GENERIC = 0x0,
            LOCATION_ENTER = 0x1,
            LOCATION_DEFEND = 0x2,
            LOCATION_DEPLOY = 0x3,
            LOCATION_CARAVAN_ARRIVE = 0x4,
            LOCATION_ATTACK = 0x5,
            LOCATION_PASSAGE = 0x6,
        };

    public:
        virtual LocationPrototypeInfo const * GetPrototypeInfo() const ;
        virtual CStr GetPropertyName(int) const ;
        static CStr __fastcall GetLocationNameFromPassageAddress(CStr const &);
        virtual void AddChild(Obj *);
        Location(LocationPrototypeInfo const &);
        std::vector<Npc *,std::allocator<Npc *> > const & GetNpcs() const ;
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        virtual int GetPropertyId(char const *) const ;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        static m3d::Class * __fastcall GetBaseClass();
        virtual void RenderDebugInfo() const ;
        virtual void Remove();
        static CStr __fastcall GetLevelNameFromPassageAddress(CStr const &);
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        Location * GetCorrespondingPassageLocation() const ;
        virtual void SetPositionSelf(CVector const &);
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        bool IsObjectInside(int) const ;
        virtual m3d::Class * GetClass() const ;
        virtual void Update(float,unsigned int);
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        void SetActive(bool);
        void SetRadius(float);
        static void __fastcall Registration();
        void SetLocationType(LocationType);
        virtual int OnEvent(Event const &);
        CStr const & GetPassageAddress() const ;
        bool IsPassageActive() const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        LocationType GetLocationType() const ;
        float GetRadius() const ;
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;

    protected:
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual void OnObjectIn(Obj *);
        virtual void OnObjectOut(Obj *);
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual void _InternalPostLoad();
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;

        virtual ~Location();
    private:
        void _OnGameStart(Event const &);
        virtual m3d::Object * Clone();
        static m3d::Object * __fastcall CreateObject();
        bool _MustCheckObject(Obj const *) const ;
        void _CheckIncomingOutgoingObjects(std::set<ref_ptr<Obstacle>,std::less<ref_ptr<Obstacle> >,std::allocator<ref_ptr<Obstacle> > > &);

    private:
        std::set<int> m_idsWasInside;
        NumericInRangeRegenerating<float> m_timeForNextCheck;
        std::set<m3d::Class *> m_targetClasses;
        bool m_bIsActive;
        float m_lookingTimeOut;
        LocationType m_locationType;
        std::set<eTolerance> m_toleranceSet;
        CStr m_passageAddress;
        bool m_bPassageActive;
        CStr m_correspondingPassageLocationName;
        std::vector<Npc *> m_npcs;
        int m_numFramesPassed;
    };
}
