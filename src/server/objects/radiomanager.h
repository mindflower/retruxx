#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"

namespace ai
{
    class RadioManagerPrototypeInfo : public PrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
    };

    class RadioManager :  public Obj
    {
    public:
        enum RM_ENUM
        {
            RM_OBJECT_DIE = 0x0,
            RM_NOTICE = 0x1,
            RM_PART_BROKEN = 0x2,
            RM_UNDER_ATTACK = 0x3,
            RM_RELATION_CHANGED = 0x4,
            RM_MAX_NUMBER = 0x5,
        };

    public:
        virtual int OnEvent(Event const &);
        void DisableRadio();
        RadioManager(RadioManagerPrototypeInfo const &);
        virtual m3d::Class * GetClass() const ;
        static m3d::Class * __fastcall GetBaseClass();
        virtual RadioManagerPrototypeInfo const * GetPrototypeInfo() const ;
        void EnableRadio();

    protected:
        virtual ~RadioManager();

    private:
        void _OnPlayerVehicleChanged(Event const &);
        void _OnObjectDie(Event const &);
        void _OnRelationChanged(Event const &);
        void _OnUnderAttack(Event const &);
        void ReadyMessage(RM_ENUM,int,CStr const &);
        void _OnSomeoneAtSight(Event const &);
        static m3d::Object * __fastcall CreateObject();
        void _OnPartBroken(Event const &);
        virtual m3d::Object * Clone();

    private:
        bool m_bRadioEnabled;
    };
}
