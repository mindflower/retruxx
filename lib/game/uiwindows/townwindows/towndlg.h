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
        CStr m_lblTownNameName;
        CStr m_lstBuildingsName;
        CStr m_wndClanPictureName;
        AuxInfo(TownDlg::AuxInfo const&);
        AuxInfo();
    };

    struct ConditionalClosingInfo
    {
        CStr m_townName;
        CStr m_levelName;
    };

    using ConditionalClosingInfoList =
        std::list<TownDlg::ConditionalClosingInfo*, std::allocator<TownDlg::ConditionalClosingInfo*>>;

protected:
    TownDlg();
    TownDlg(TownDlg const& rhs);

public:
    virtual ~TownDlg() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classTownDlg;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataSetup() override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    virtual int GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterAddToWndStation() override;
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
    TownDlg::AuxInfo m_aif;
    m3d::ui::Wnd* m_lblTownName;
    int m_townId;
    std::list<TownDlg::ConditionalClosingInfo*, std::allocator<TownDlg::ConditionalClosingInfo*>>
        m_conditionalClosingInfoList;
    m3d::ui::ImageWnd* m_wndClanPicture;
};
