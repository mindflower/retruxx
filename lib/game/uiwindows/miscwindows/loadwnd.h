#pragma once
#include "lswnd.h"

class LoadWnd : public LSWnd
{
public:
    struct LAuxInfo
    {
        /* 0x0000 */ CStr m_btnLoadName;
        LAuxInfo(LoadWnd::LAuxInfo const&);
        LAuxInfo();
    }; /* size: 0x000c */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override /* 0x00 */;
    virtual void OnSaveSelectionChange() override /* 0x130 */;
    virtual void OnEnter() override /* 0x144 */;
    void OnLoad();

    /* 0x02a0 */ m3d::ui::ButtonWnd* m_btnLoad;
    /* 0x02a4 */ LoadWnd::LAuxInfo m_laif;

    LoadWnd();
    LoadWnd(LoadWnd const& rhs);

public:
    virtual ~LoadWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(LoadWnd);
}; /* size: 0x02b0 */
