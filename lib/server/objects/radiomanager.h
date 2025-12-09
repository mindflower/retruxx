#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"

namespace ai
{
    class RadioManagerPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
    }; /* size: 0x0040 */

    class RadioManager : public ai::Obj
    {
    protected:
        virtual  ~RadioManager() override /* 0x00 */;

    private:
        RadioManager(const ai::RadioManagerPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classRadioManager;
        virtual const ai::RadioManagerPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;
        virtual int OnEvent(const ai::Event& evn) override /* 0x40 */;

    private:
        void _OnObjectDie(const ai::Event& evn);
        void _OnSomeoneAtSight(const ai::Event& evn);
        void _OnUnderAttack(const ai::Event& evn);
        void _OnRelationChanged(const ai::Event& evn);
        void _OnPartBroken(const ai::Event& evn);
        void _OnPlayerVehicleChanged(const ai::Event& evn);

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
        void ReadyMessage(ai::RadioManager::RM_ENUM type, int belong, const CStr& mes);
        /* 0x00c0 */ bool m_bRadioEnabled;

    public:
        void EnableRadio();
        void DisableRadio();
    }; /* size: 0x00c4 */
}
