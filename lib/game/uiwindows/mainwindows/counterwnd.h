#pragma once
#include <ui/ui.h>

namespace ai
{
    class DynamicQuestHunt;
}

class ElectronicDigitalWnd;

class CounterWnd : public m3d::ui::Wnd
{
public:
    bool NeedShow() const;

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndCounterName;
        /* 0x000c */ CStr m_strHuntTime;
        AuxInfo(const CounterWnd::AuxInfo&);
        AuxInfo();
    }; /* size: 0x0018 */

    enum CounterType
    {
        COUNTERTYPE_QUEST_HUNT = 0,
        COUNTERTYPE_NUM_COUNTER_TYPES = 1,
        COUNTERTYPE_INVALID = 1,
    };

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    void OnDynamicQuestStateChanged(void* data);
    int SetupForHuntQuest(int dQuestId);
    void CheckAndShow();
    void ClearCounter();
    void OnNewFrame();
    void UpdateCounter();
    float GetHuntTimeLeft() const;
    void GetMinAndSecFromTime(float time, int& minutes, int& secundes) const;
    const ai::DynamicQuestHunt* GetQuestHunt() const;
    void OnStartLevel(void* data);
    /* 0x0220 */ CounterWnd::AuxInfo m_aif;
    /* 0x0238 */ ElectronicDigitalWnd* m_wndCounter;
    /* 0x023c */ CounterWnd::CounterType m_counterType;
    /* 0x0240 */ int m_dQuestId;
    CounterWnd();
    CounterWnd(const CounterWnd& rhs);

public:
    virtual ~CounterWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classCounterWnd;
}; /* size: 0x0244 */
