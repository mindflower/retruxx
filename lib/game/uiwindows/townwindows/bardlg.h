#pragma once
#include <game/uiwindows/charwindows/childpanel.h>

namespace ai
{
    class Npc;
    class Bar;
}  // namespace ai

class NpcList;

class BarDlg : public ChildPanel
{
public:
    int SetUpForBar(int barId);
    ai::Bar* GetBar() const;

    struct BarDlg::AuxInfo
    {
        CStr m_lstNPCsPatternName;
        AuxInfo(BarDlg::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterAddToWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    int OnBeginConversation(ai::Npc* npc);
    void FullUpdate();
    BarDlg::AuxInfo m_aif;
    NpcList* m_lstNPCs;
    int m_barId;
    BarDlg();
    BarDlg(BarDlg const& rhs);

public:
    virtual ~BarDlg() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classBarDlg;
};
