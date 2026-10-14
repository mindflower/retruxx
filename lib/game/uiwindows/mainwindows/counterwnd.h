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
        CStr m_wndCounterName;
        CStr m_strHuntTime;
        AuxInfo(const CounterWnd::AuxInfo&);
        AuxInfo();
    };

    enum CounterType
    {
        COUNTERTYPE_QUEST_HUNT = 0,
        COUNTERTYPE_NUM_COUNTER_TYPES = 1,
        COUNTERTYPE_INVALID = 1,
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
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
    CounterWnd::AuxInfo m_aif;
    ElectronicDigitalWnd* m_wndCounter;
    CounterWnd::CounterType m_counterType;
    int m_dQuestId;
    CounterWnd();
    CounterWnd(const CounterWnd& rhs);

public:
    virtual ~CounterWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classCounterWnd;
};
