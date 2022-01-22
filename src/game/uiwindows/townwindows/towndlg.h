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
}

class TownDlg : public ChildPanel
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_lblTownNameName;
        CStr m_lstBuildingsName;
        CStr m_wndClanPictureName;
    };

    class ConditionalClosingInfo
    {
    private:
        CStr m_townName;
        CStr m_levelName;
    };

public:
    virtual int GameDataLoad(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *);
    ConditionalClosingInfo * GetConditionalClosingInfoForTown(CStr const &,CStr const &) const ;
    static m3d::Object * CreateObject();
    void FullUpdate();
    void ClearConditionalClosingInfo();
    virtual m3d::Class * GetClass() const ;
    virtual int OnAfterAddToWndStation();
    void OnEnterBar(int);
    bool IsTownWithConditionalClosing(CStr const &,CStr const &) const ;
    virtual int OnBeforeAddToWndStation();
    virtual int GameDataClear(bool);
    virtual int GameDataSave(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *);
    void UpdateClanPicture();
    void AddConditionalClosingInfo(CStr const &,CStr const &);
    virtual int GameDataSetup();
    void OnLeaveBar();
    virtual int GameDataUpdate(void *,int);
    static m3d::Class * GetBaseClass();
    void SetConditionalClosingForTown(CStr const &,CStr const &,bool);
    int SetUpForTown(int);
    void ShowForTown(int);
    virtual m3d::Object * Clone();
    ai::Town * GetTown() const ;
    void RemoveConditionalClosingInfo(CStr const &,CStr const &);
    virtual ~TownDlg();

protected:
    TownDlg();
    TownDlg(TownDlg const &);

public:
    RT_CLASS_DECLARE(TownDlg);

private:
    TownDlg::AuxInfo m_aif;
    m3d::ui::Wnd *m_lblTownName;
    int m_townId;
    std::list<TownDlg::ConditionalClosingInfo *> m_conditionalClosingInfoList;
    m3d::ui::ImageWnd *m_wndClanPicture;
};
