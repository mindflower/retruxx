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
        /* 0x0000 */ CStr m_lstNPCsPatternName;
        AuxInfo(BarDlg::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x000c */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0xc8 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int OnAfterAddToWndStation() override /* 0x00 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x00 */;
    int OnBeginConversation(ai::Npc* npc);
    void FullUpdate();
    /* 0x0224 */ BarDlg::AuxInfo m_aif;
    /* 0x0230 */ NpcList* m_lstNPCs;
    /* 0x0234 */ int m_barId;
    BarDlg();
    BarDlg(BarDlg const& rhs);

public:
    virtual ~BarDlg() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classBarDlg;
}; /* size: 0x0238 */
