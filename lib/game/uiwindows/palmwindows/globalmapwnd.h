#pragma once
#include "screenwnd.h"
#include <map>
#include <core/ref_ptr.h>
#include <ui/image.h>

class LevelIco;

class LevelConnectionsWnd : public m3d::ui::Wnd
{
public:
    struct ConnectionDrawInfo
    {
        ConnectionDrawInfo(LevelConnectionsWnd::ConnectionDrawInfo const& __that);
        ConnectionDrawInfo(PointBase<float> const& pt0, PointBase<float> const& pt1);
        PointBase<float> m_pt0;
        PointBase<float> m_pt1;
    };

    using ConnectionDrawInfoVector =
        std::vector<LevelConnectionsWnd::ConnectionDrawInfo, std::allocator<LevelConnectionsWnd::ConnectionDrawInfo>>;

public:
    void SetDrawInfo(
        std::vector<
            LevelConnectionsWnd::ConnectionDrawInfo,
            std::allocator<LevelConnectionsWnd::ConnectionDrawInfo>> const& connectionDrawInfo);

    struct AuxInfo
    {
        CStr m_connectionTexName;
        float m_connectionH;
        AuxInfo(LevelConnectionsWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override;
    void DrawConnections(m3d::ui::DrawInfo const& di);
    std::vector<LevelConnectionsWnd::ConnectionDrawInfo, std::allocator<LevelConnectionsWnd::ConnectionDrawInfo>>
        m_connectionDrawInfo;
    LevelConnectionsWnd::AuxInfo m_aif;
    m3d::rend::TexHandle m_connectionTex;
    LevelConnectionsWnd();
    LevelConnectionsWnd(LevelConnectionsWnd const& rhs);

public:
    virtual ~LevelConnectionsWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classLevelConnectionsWnd;
};

class GlobalMapWnd : public ScreenWnd
{
public:
    struct Connection
    {
        CStr m_levelNameFrom;
        CStr m_levelNameTo;
    };

    struct AuxInfo
    {
        CStr m_levelIcoName;
        CStr m_wndChartName;
        AuxInfo(GlobalMapWnd::AuxInfo const&);
        AuxInfo();
    };

    using LevelIcoMap =
        std::map<CStr, ref_ptr<LevelIco>, std::less<CStr>, std::allocator<std::pair<CStr const, ref_ptr<LevelIco>>>>;
    using LevelIcoPair = std::pair<CStr, ref_ptr<LevelIco>>;
    using ConnectionVector = std::vector<GlobalMapWnd::Connection, std::allocator<GlobalMapWnd::Connection>>;

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override;
    virtual int OnAfterAddToWndStation() override;
    void OnStartLevel();
    void OnLocationStateChanged();
    void UpdateConnections();
    void ClearConnections();
    void UpdateConnectionsDrawInfo();
    bool ConnectionExists(CStr const& levelName0, CStr const& levelName1) const;
    int InitLevelIcons();
    void LaunchLocalMap();
    int CreateConnectionsWnd();
    std::map<CStr, ref_ptr<LevelIco>, std::less<CStr>, std::allocator<std::pair<CStr const, ref_ptr<LevelIco>>>>
        m_levelIcons;
    std::vector<GlobalMapWnd::Connection, std::allocator<GlobalMapWnd::Connection>> m_connections;
    GlobalMapWnd::AuxInfo m_aif;
    LevelConnectionsWnd* m_wndConnections;
    GlobalMapWnd();
    GlobalMapWnd(GlobalMapWnd const& rhs);

public:
    virtual ~GlobalMapWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classGlobalMapWnd;
};

class LevelIco : public m3d::ui::ImageWnd
{
public:
    int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc);
    int SetUpForLevel(CStr const& levelName);
    CStr const& GetLevelName() const;

    enum State
    {
        STATE_CURRENT = 0,
        STATE_VISITED = 1,
        STATE_KNOWN = 2,
        STATE_INVISIBLE = 3,
    };

    struct AuxInfo
    {
        CStr m_selFrameTexName;
        AuxInfo(LevelIco::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override;
    virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) override;
    void OnStartLevel();
    void UpdateState();
    void SetState(LevelIco::State state);
    bool IsValid() const;
    CStr m_levelName;
    LevelIco::State m_state;
    static LevelIco::AuxInfo m_aif;
    m3d::rend::TexHandle m_selFrameTex;
    LevelIco();
    LevelIco(LevelIco const& rhs);

public:
    virtual ~LevelIco() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classLevelIco;
};
