#pragma once

class VideoWnd :  public ChildPanel
{
public:
    static class m3d::Object * CreateObject();
    int ShowModelByPrototypeId(int,class ref_ptr<class m3d::ui::Wnd>);
    int GetPrototypeId() const ;
    static struct m3d::Class * GetBaseClass();
    virtual class m3d::Object * Clone();
    int HideModel(class ref_ptr<class m3d::ui::Wnd>);
    virtual struct m3d::Class * GetClass() const ;
    int GetObjId() const ;
    int ShowModelByObjId(int,class ref_ptr<class m3d::ui::Wnd>);
    class ref_ptr<class m3d::ui::Wnd> GetSrcWindow() const ;
    virtual ~VideoWnd();
protected:
    virtual int GameDataSetup();
    VideoWnd();
    VideoWnd(class VideoWnd const &);
    bool IsDisabled() const ;
    void ShowBgModel(enum BgMode);
    void OnNewFrame();
    virtual int GameDataClear(bool);
    void UpdateBgMode();
    void UpdateBgModel(enum BgMode);
    virtual int GameDataUpdate(void *,int);
    enum BgMode DetectBgMode() const ;
private:
    ContextModelWnd *m_wndModel;
    VideoWnd::AuxInfo m_aif;
    ref_ptr<m3d::ui::Wnd> m_srcWindow;
    VideoWnd::BgMode m_bgMode;
};
