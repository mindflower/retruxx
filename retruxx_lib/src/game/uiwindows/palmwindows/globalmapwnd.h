#pragma once
#include "screenwnd.h"
#include <map>
#include <core/ref_ptr.h>
#include <ui/image.h>

class LevelIco;

class LevelConnectionsWnd :  public m3d::ui::Wnd
{
public:
    class ConnectionDrawInfo
    {
    public:
        ConnectionDrawInfo(ConnectionDrawInfo const&);
    protected:
    private:
        PointBase<float> m_pt0;
        PointBase<float> m_pt1;
    };

    class AuxInfo
    {
    public:
        AuxInfo(void);
    protected:
    private:
        CStr m_connectionTexName;
        float m_connectionH;
    };

public:
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    void SetDrawInfo(std::vector<ConnectionDrawInfo, std::allocator<ConnectionDrawInfo> > const &);
    virtual ~LevelConnectionsWnd();

protected:
    void DrawConnections(m3d::ui::DrawInfo const &);
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    LevelConnectionsWnd();
    LevelConnectionsWnd(LevelConnectionsWnd const &);

public:
    RT_CLASS_DECLARE(LevelConnectionsWnd);

private:
    std::vector<LevelConnectionsWnd::ConnectionDrawInfo> m_connectionDrawInfo;
    LevelConnectionsWnd::AuxInfo m_aif;
    m3d::rend::TexHandle m_connectionTex;
};

class GlobalMapWnd :  public ScreenWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_levelIcoName;
        CStr m_wndChartName;
    };

    class Connection
    {
    private:
        CStr m_levelNameFrom;
        CStr m_levelNameTo;
    };

public:
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual ~GlobalMapWnd();
    virtual m3d::Class * GetClass() const ;

protected:
    void UpdateConnectionsDrawInfo();
    GlobalMapWnd();
    GlobalMapWnd(GlobalMapWnd const &);
    int CreateConnectionsWnd();
    bool ConnectionExists(CStr const &,CStr const &) const ;
    virtual int GameDataSetup();
    virtual int OnAfterAddToWndStation();
    void ClearConnections();
    int InitLevelIcons();
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataClear(bool);
    void UpdateConnections();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    void LaunchLocalMap();
    void OnStartLevel();
    void OnLocationStateChanged();

public:
    RT_CLASS_DECLARE(GlobalMapWnd);

private:
    std::map<CStr,ref_ptr<LevelIco>> m_levelIcons;
    std::vector<GlobalMapWnd::Connection> m_connections;
    GlobalMapWnd::AuxInfo m_aif;
    LevelConnectionsWnd *m_wndConnections;
};

class LevelIco :  public m3d::ui::ImageWnd
{
public:
    enum State
    {
        STATE_CURRENT = 0x0,
        STATE_VISITED = 0x1,
        STATE_KNOWN = 0x2,
        STATE_INVISIBLE = 0x3,
    };

public:
    virtual m3d::Class * GetClass() const ;
    virtual ~LevelIco();
    int SetUpForLevel(CStr const &);
    static m3d::Class * GetBaseClass();
    int CreateFromPattern(m3d::ui::Wnd *,bool);
    CStr const & GetLevelName() const ;
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();

protected:
    void OnStartLevel();
    LevelIco(LevelIco const &);
    LevelIco();
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    virtual int GameDataUpdate(void *,int);
    void SetState(State);
    void UpdateState();
    virtual int OnMouseButton0(unsigned int, PointBase<float> const &);
    bool IsValid() const ;

public:
    RT_CLASS_DECLARE(LevelIco);

private:
    CStr m_levelName;
    LevelIco::State m_state;
    m3d::rend::TexHandle m_selFrameTex;
};
