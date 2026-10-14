#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"

namespace ai
{
    class RadioManagerPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class RadioManager : public ai::Obj
    {
        friend class RadioManagerPrototypeInfo;

    protected:
        virtual ~RadioManager() override;

    private:
        RadioManager(ai::RadioManagerPrototypeInfo const& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classRadioManager;
        virtual ai::RadioManagerPrototypeInfo const* GetPrototypeInfo() const override;
        virtual int OnEvent(ai::Event const& evn) override;

    private:
        void _OnObjectDie(ai::Event const& evn);
        void _OnSomeoneAtSight(ai::Event const& evn);
        void _OnUnderAttack(ai::Event const& evn);
        void _OnRelationChanged(ai::Event const& evn);
        void _OnPartBroken(ai::Event const& evn);
        void _OnPlayerVehicleChanged(ai::Event const& evn);

        enum RM_ENUM
        {
            RM_OBJECT_DIE = 0,
            RM_NOTICE = 1,
            RM_PART_BROKEN = 2,
            RM_UNDER_ATTACK = 3,
            RM_RELATION_CHANGED = 4,
            RM_MAX_NUMBER = 5,
        };

    private:
        void ReadyMessage(ai::RadioManager::RM_ENUM type, int belong, CStr const& mes);
        bool m_bRadioEnabled;

    public:
        void EnableRadio();
        void DisableRadio();
    };
}  // namespace ai
