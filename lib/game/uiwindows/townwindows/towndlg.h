#pragma once
#include <list>
#include <game/uiwindows/charwindows/childpanel.h>

namespace ai
{
    class Town;
}

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}  // namespace m3d

class TownDlg : public ChildPanel
{
public:
    int SetUpForTown(int townId);
    ai::Town* GetTown() const;
    void SetConditionalClosingForTown(CStr const& townName, CStr const& levelName, bool bConditionalClosing);
    bool IsTownWithConditionalClosing(CStr const& townName, CStr const& levelName) const;

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_lblTownNameName;
        /* 0x000c */ CStr m_lstBuildingsName;
        /* 0x0018 */ CStr m_wndClanPictureName;
        AuxInfo(TownDlg::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0024 */

    struct ConditionalClosingInfo
    {
        /* 0x0000 */ CStr m_townName;
        /* 0x000c */ CStr m_levelName;
    }; /* size: 0x0018 */

    using ConditionalClosingInfoList =
        std::list<TownDlg::ConditionalClosingInfo*, std::allocator<TownDlg::ConditionalClosingInfo*>>;

protected:
    TownDlg();
    TownDlg(TownDlg const& rhs);

public:
    virtual ~TownDlg() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classTownDlg;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override /* 0x00 */;
    virtual int GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int OnAfterAddToWndStation() override /* 0x00 */;
    void FullUpdate();
    void OnEnterBar(int barId);
    void OnLeaveBar();
    void ShowForTown(int townId);
    TownDlg::ConditionalClosingInfo* GetConditionalClosingInfoForTown(CStr const& townName, CStr const& levelName)
        const;
    void AddConditionalClosingInfo(CStr const& townName, CStr const& levelName);
    void RemoveConditionalClosingInfo(CStr const& townName, CStr const& levelName);
    void ClearConditionalClosingInfo();
    void UpdateClanPicture();
    /* 0x0224 */ TownDlg::AuxInfo m_aif;
    /* 0x0248 */ m3d::ui::Wnd* m_lblTownName;
    /* 0x024c */ int m_townId;
    /* 0x0250 */ std::list<TownDlg::ConditionalClosingInfo*, std::allocator<TownDlg::ConditionalClosingInfo*>>
        m_conditionalClosingInfoList;
    /* 0x025c */ m3d::ui::ImageWnd* m_wndClanPicture;
}; /* size: 0x0260 */
