#pragma once
#include <ui/image.h>

class LevelConnectionsWnd :  public m3d::ui::Wnd
{
public:
    class ConnectionDrawInfo
    {
    public:
        ConnectionDrawInfo(struct ConnectionDrawInfo const&);
    protected:
    private:
        PointBase<float> m_pt0;
        PointBase<float> m_pt1;
    };
public:
    virtual class m3d::Object * Clone();
    static struct m3d::Class * GetBaseClass();
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * CreateObject();
    void SetDrawInfo(class std::vector<struct ConnectionDrawInfo,class std::allocator<struct ConnectionDrawInfo> > const &);
    virtual ~LevelConnectionsWnd();
protected:
    void DrawConnections(struct m3d::ui::DrawInfo const &);
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    LevelConnectionsWnd();
    LevelConnectionsWnd(class LevelConnectionsWnd const &);
private:
    std::vector<LevelConnectionsWnd::ConnectionDrawInfo> m_connectionDrawInfo;
    LevelConnectionsWnd::AuxInfo m_aif;
    m3d::rend::TexHandle m_connectionTex;
};

class GlobalMapWnd :  public ScreenWnd
{
public:
    virtual class m3d::Object * Clone();
    static struct m3d::Class * GetBaseClass();
    static class m3d::Object * CreateObject();
    virtual ~GlobalMapWnd();
    virtual struct m3d::Class * GetClass() const ;
protected:
    void UpdateConnectionsDrawInfo();
    GlobalMapWnd();
    GlobalMapWnd(class GlobalMapWnd const &);
    int CreateConnectionsWnd();
    bool ConnectionExists(CStr const &,CStr const &) const ;
    virtual int GameDataSetup();
    virtual int OnAfterAddToWndStation();
    void ClearConnections();
    int InitLevelIcons();
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataClear(bool);
    void UpdateConnections();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    void LaunchLocalMap();
    void OnStartLevel();
    void OnLocationStateChanged();
private:
    std::map<CStr,ref_ptr<LevelIco>> m_levelIcons;
    std::vector<GlobalMapWnd::Connection> m_connections;
    GlobalMapWnd::AuxInfo m_aif;
    LevelConnectionsWnd *m_wndConnections;
};

class LevelIco :  public m3d::ui::ImageWnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    virtual ~LevelIco();
    int SetUpForLevel(CStr const &);
    static struct m3d::Class * GetBaseClass();
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    CStr const & GetLevelName() const ;
    static class m3d::Object * CreateObject();
    virtual class m3d::Object * Clone();
protected:
    void OnStartLevel();
    LevelIco(class LevelIco const &);
    LevelIco();
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    virtual int GameDataUpdate(void *,int);
    void SetState(enum State);
    void UpdateState();
    virtual int OnMouseButton0(unsigned int,class PointBase<float> const &);
    bool IsValid() const ;
private:
    CStr m_levelName;
    LevelIco::State m_state;
    m3d::rend::TexHandle m_selFrameTex;
};
