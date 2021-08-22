#pragma once
#include "gameuimanager.h"
#include <iface.h>
#include <vector>
#include <core/ref_ptr.h>
#include <game/uimisc/objectcollection.h>
#include <game/uimisc/stringparser.h>
#include <ui/msgbox.h>

class NavPointManager;
class ObjectCollection;
class QuestInfoManager;

namespace ai
{
    class Vehicle;
    class Town;
    class Workshop;
}

class MsgManager;
class RepliesManager;
class StringParser;
class LevelInfoManager;
class WeaponGroupManager;
class SavesManager;
class HelpManager;

namespace m3d
{
    namespace cmn
    {
        class XmlFile;
        class XmlNode;
    }

    namespace ui
    {
        class Wnd;
    }

    namespace rend
    {
        class TexHandle;
    }

    class AuxImpulseInfo;
    class AIParam;
    class Event;
}

enum GameState
{
    GS_ERROR = 0xFFFFFFFF,
    GS_GAME = 0x0,
    GS_CINEMATIC = 0x1,
    GS_MAINMENU = 0x2,
    GS_INITIALIZATION = 0x3,
    GS_NUM_GAMESTATES = 0x4,
};

class ITruxxUiManager : public IBase
{
public:
    virtual CStr GetPathToDialogsFileGlobal() const = 0;
    virtual QuestInfoManager* GetQuestInfoManager() const = 0;
    virtual ref_ptr<m3d::ui::Wnd> GetWindow(int) const = 0;
    virtual int GetDefaultFloatPrecision() const = 0;
    virtual MsgManager* GetMsgManager() const = 0;
    virtual ObjectCollection const& GetObjectCollection() const = 0;
    virtual bool IsHidden() const = 0;
    virtual int AddWindow(ref_ptr<m3d::ui::Wnd>, int&, bool, bool) = 0;
    virtual int AddWindowById(ref_ptr<m3d::ui::Wnd>, int, bool, bool) = 0;
    virtual CStr GetPathToQuestInfoFileGlobal() const = 0;
    virtual int HandleImpulse(m3d::AuxImpulseInfo const&, m3d::ui::Wnd*) = 0;
    virtual bool IsHiddenByUser() const = 0;
    virtual CStr GetPathToDynamicDialogsFileGlobal() const = 0;
    virtual bool IsWindowVisibleAndNotAnimating(int) const = 0;
    virtual int Load(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>) = 0;
    virtual int Init() = 0;
    virtual bool IsModalEqualWndRunning() const = 0;
    virtual LevelInfoManager* GetLevelInfoManager() const = 0;
    virtual void OnEnterTown(int) = 0;
    virtual CStr GetPathToLevelInfoFile() const = 0;
    virtual m3d::rend::TexHandle GetIcoByName(CStr const&, int) = 0;
    virtual int ShowWindow(int, bool, bool, bool, bool, int*) = 0;
    virtual void ShowSplash(int, CStr const&) const = 0;
    virtual int Show(bool, bool) = 0;
    virtual int CreateAndAddWindow(int) = 0;
    virtual StringParser const& GetStringParser() const = 0;
    virtual ai::Workshop* GetCurrentShop() const = 0;
    virtual void AddFadingMsgByStrId(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam> > const&) const = 0;
    virtual NavPointManager* GetNavPointManager() const = 0;
    virtual ai::Workshop* GetCurrentWorkshop() const = 0;
    virtual void AddFadingMsg(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam> > const&) const = 0;
    virtual bool IsGameModeValidForSmartCursor(GameState) const = 0;
    virtual int Str2WndGuiId(CStr const&) const = 0;
    virtual ai::Vehicle* GetVehicleSellingInWorkshop() const = 0;
    virtual std::vector<int, std::allocator<int> > const& GetTakenQuestIds() const = 0;
    virtual RepliesManager* GetRepliesManager() const = 0;
    virtual int Reset(bool) = 0;
    virtual WeaponGroupManager* GetWeaponGroupManager() const = 0;
    virtual void AddImportantFadingMsgByStrId(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam> > const&) const = 0;
    virtual m3d::ui::MbRetCodes RunMsgBoxDlg(CStr const&, CStr const&, unsigned int, bool) = 0;
    virtual bool IsInSaleMode() const = 0;
    virtual bool IsWindowVisible(int) const = 0;
    virtual int SetEventsForWindow(int, std::vector<int, std::allocator<int> > const&) = 0;
    virtual CStr GetPathToSplashes() const = 0;
    virtual int Update() = 0;
    virtual int LaunchEvent(int, GuiEventType, void*) = 0;
    virtual SavesManager* GetSavesManager() const = 0;
    virtual ai::Town* GetCurrentTown() const = 0;
    virtual int Save(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>) = 0;
    virtual void StartSplashing(int) const = 0;
    virtual int HandleAppEvent(m3d::Event const&) = 0;
    virtual void OnLeaveTown(bool) = 0;
    virtual void AddImportantFadingMsg(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam> > const&) const = 0;
    virtual HelpManager* GetHelpManager() const = 0;
    virtual int RemoveWindow(int) = 0;
};

class TruxxUiManager : public ITruxxUiManager, public GameUiManager
{
public:
    virtual CStr GetPathToDialogsFileGlobal() const;
    virtual QuestInfoManager* GetQuestInfoManager() const;
    virtual ref_ptr<m3d::ui::Wnd> GetWindow(int) const;
    virtual int GetDefaultFloatPrecision() const;
    virtual MsgManager* GetMsgManager() const;
    virtual ObjectCollection const& GetObjectCollection() const;
    virtual bool IsHidden() const;
    virtual int AddWindow(ref_ptr<m3d::ui::Wnd>, int&, bool, bool);
    virtual int AddWindowById(ref_ptr<m3d::ui::Wnd>, int, bool, bool);
    virtual CStr GetPathToQuestInfoFileGlobal() const;
    virtual int HandleImpulse(m3d::AuxImpulseInfo const&, m3d::ui::Wnd*);
    TruxxUiManager();
    virtual bool IsHiddenByUser() const;
    virtual CStr GetPathToDynamicDialogsFileGlobal() const;
    virtual bool IsWindowVisibleAndNotAnimating(int) const;
    virtual int Load(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>);
    virtual int Init();
    virtual bool IsModalEqualWndRunning() const;
    virtual LevelInfoManager* GetLevelInfoManager() const;
    virtual void OnEnterTown(int);
    virtual CStr GetPathToLevelInfoFile() const;
    virtual m3d::rend::TexHandle GetIcoByName(CStr const&, int);
    virtual int ShowWindow(int, bool, bool, bool, bool, int*);
    virtual void ShowSplash(int, CStr const&) const;
    virtual int Show(bool, bool);
    virtual int CreateAndAddWindow(int);
    virtual StringParser const& GetStringParser() const;
    virtual ai::Workshop* GetCurrentShop() const;
    virtual void AddFadingMsgByStrId(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam> > const&) const;
    virtual NavPointManager* GetNavPointManager() const;
    virtual ai::Workshop* GetCurrentWorkshop() const;
    virtual void AddFadingMsg(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam> > const&) const;
    virtual bool IsGameModeValidForSmartCursor(GameState) const;
    virtual int Str2WndGuiId(CStr const&) const;
    virtual ai::Vehicle* GetVehicleSellingInWorkshop() const;
    virtual std::vector<int, std::allocator<int> > const& GetTakenQuestIds() const;
    virtual RepliesManager* GetRepliesManager() const;
    virtual int Reset(bool);
    virtual WeaponGroupManager* GetWeaponGroupManager() const;
    virtual void AddImportantFadingMsgByStrId(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam> > const&) const;
    virtual m3d::ui::MbRetCodes RunMsgBoxDlg(CStr const&, CStr const&, unsigned int, bool);
    virtual bool IsInSaleMode() const;
    virtual bool IsWindowVisible(int) const;
    virtual int SetEventsForWindow(int, std::vector<int, std::allocator<int> > const&);
    virtual ~TruxxUiManager();
    virtual CStr GetPathToSplashes() const;
    virtual int Update();
    virtual int LaunchEvent(int, GuiEventType, void*);
    virtual SavesManager* GetSavesManager() const;
    virtual ai::Town* GetCurrentTown() const;
    virtual int Save(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>);
    virtual void StartSplashing(int) const;
    virtual int HandleAppEvent(m3d::Event const&);
    virtual void OnLeaveTown(bool);
    virtual void AddImportantFadingMsg(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam> > const&) const;
    virtual HelpManager* GetHelpManager() const;
    virtual int RemoveWindow(int);

private:
    virtual bool GUI_IsWndModalEqual(m3d::ui::Wnd*) const;
    virtual int GUI_ReadFromXml(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>);
    virtual int IncRef();
    virtual void GUI_UnRegisterScriptGlobals();
    void ShowGameMenu(CStr const&);
    void OnGameModeChanged(void*);
    void OnBeforeStartLevel();
    virtual void GUI_EndModalDlg();
    virtual int GUI_BindWindowsToEvents();
    virtual void GUI_RegisterCVars();
    virtual int GUI_WriteToXml(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>);
    void OnStartLevel(void*);
    void OnEndLevel(bool);
    int LoadCommonDiz();
    bool CanLaunchModalEqualWindow();
    void PrepareMenuForShow(CStr const&);
    virtual int DecRef();
    virtual bool GUI_NeedUpdateWndOnEvent(ref_ptr<m3d::ui::Wnd>, int, void*);
    virtual int GUI_HandleEvent(int, m3d::ui::Wnd*, void*);
    virtual void* QueryIface(char const*);
    virtual void GUI_RegisterEvents();
    virtual void GUI_RegisterClasses();
    void OnChangeGameMenuMode();
    virtual void GUI_UnregisterCVars();
    void SetGameMenuMode(bool);
    virtual int GUI_BeginModalDlg(bool, bool);
    virtual int GUI_RegisterScriptGlobals();

private:
    int m_refCount;
    IBase* m_parent;
    m3d::CVar m_cvPathToQuestInfo;
    m3d::CVar m_cvPathToDialogs;
    m3d::CVar m_cvPathToDynamicDialogs;
    m3d::CVar m_cvPathToLevelInfo;
    m3d::CVar m_cvDefaultFloatPrecision;
    m3d::CVar m_cvPathToSplashes;
    QuestInfoManager* m_questInfoManager;
    RepliesManager* m_repliesManager;
    ObjectCollection m_objectCollection;
    StringParser m_stringParser;
    LevelInfoManager* m_levelInfoManager;
    NavPointManager* m_navPointManager;
    WeaponGroupManager* m_weaponGroupManager;
    SavesManager* m_savesManager;
    MsgManager* m_msgManager;
    HelpManager* m_helpManager;
    bool m_bInGameMenuMode;
    std::vector<int> m_takenQuestIds;
    int m_currentTownId;
    bool m_bIsHiddenByUser;
    bool m_bIsPlayerDead;
};