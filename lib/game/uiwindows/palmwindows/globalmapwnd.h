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
        /* 0x0000 */ PointBase<float> m_pt0;
        /* 0x0008 */ PointBase<float> m_pt1;
    }; /* size: 0x0010 */

    using ConnectionDrawInfoVector =
        std::vector<LevelConnectionsWnd::ConnectionDrawInfo, std::allocator<LevelConnectionsWnd::ConnectionDrawInfo>>;

public:
    void SetDrawInfo(
        std::vector<
            LevelConnectionsWnd::ConnectionDrawInfo,
            std::allocator<LevelConnectionsWnd::ConnectionDrawInfo>> const& connectionDrawInfo);

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_connectionTexName;
        /* 0x000c */ float m_connectionH;
        AuxInfo(LevelConnectionsWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0010 */

protected:
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override /* 0x88 */;
    void DrawConnections(m3d::ui::DrawInfo const& di);
    /* 0x0220 */ std::
        vector<LevelConnectionsWnd::ConnectionDrawInfo, std::allocator<LevelConnectionsWnd::ConnectionDrawInfo>>
            m_connectionDrawInfo;
    /* 0x0230 */ LevelConnectionsWnd::AuxInfo m_aif;
    /* 0x0240 */ m3d::rend::TexHandle m_connectionTex;
    LevelConnectionsWnd();
    LevelConnectionsWnd(LevelConnectionsWnd const& rhs);

public:
    virtual ~LevelConnectionsWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classLevelConnectionsWnd;
}; /* size: 0x0244 */

class GlobalMapWnd : public ScreenWnd
{
public:
    struct Connection
    {
        /* 0x0000 */ CStr m_levelNameFrom;
        /* 0x000c */ CStr m_levelNameTo;
    }; /* size: 0x0018 */

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_levelIcoName;
        /* 0x000c */ CStr m_wndChartName;
        AuxInfo(GlobalMapWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0018 */

    using LevelIcoMap =
        std::map<CStr, ref_ptr<LevelIco>, std::less<CStr>, std::allocator<std::pair<CStr const, ref_ptr<LevelIco>>>>;
    using LevelIcoPair = std::pair<CStr, ref_ptr<LevelIco>>;
    using ConnectionVector = std::vector<GlobalMapWnd::Connection, std::allocator<GlobalMapWnd::Connection>>;

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x00 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0x00 */;
    virtual int OnAfterAddToWndStation() override /* 0x00 */;
    void OnStartLevel();
    void OnLocationStateChanged();
    void UpdateConnections();
    void ClearConnections();
    void UpdateConnectionsDrawInfo();
    bool ConnectionExists(CStr const& levelName0, CStr const& levelName1) const;
    int InitLevelIcons();
    void LaunchLocalMap();
    int CreateConnectionsWnd();
    /* 0x0224 */ std::
        map<CStr, ref_ptr<LevelIco>, std::less<CStr>, std::allocator<std::pair<CStr const, ref_ptr<LevelIco>>>>
            m_levelIcons;
    /* 0x0230 */ std::vector<GlobalMapWnd::Connection, std::allocator<GlobalMapWnd::Connection>> m_connections;
    /* 0x0240 */ GlobalMapWnd::AuxInfo m_aif;
    /* 0x0258 */ LevelConnectionsWnd* m_wndConnections;
    GlobalMapWnd();
    GlobalMapWnd(GlobalMapWnd const& rhs);

public:
    virtual ~GlobalMapWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classGlobalMapWnd;
}; /* size: 0x025c */

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
        /* 0x0000 */ CStr m_selFrameTexName;
        AuxInfo(LevelIco::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x000c */

protected:
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override /* 0x88 */;
    virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) override /* 0x00 */;
    void OnStartLevel();
    void UpdateState();
    void SetState(LevelIco::State state);
    bool IsValid() const;
    /* 0x0234 */ CStr m_levelName;
    /* 0x0240 */ LevelIco::State m_state;
    static LevelIco::AuxInfo m_aif;
    /* 0x0244 */ m3d::rend::TexHandle m_selFrameTex;
    LevelIco();
    LevelIco(LevelIco const& rhs);

public:
    virtual ~LevelIco() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classLevelIco;
}; /* size: 0x0248 */
