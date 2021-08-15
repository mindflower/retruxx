#pragma once
#include "gameuimanager.h"
#include <iface.h>
#include <vector>
#include <core/ref_ptr.h>
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
    int Init();
    int Reset(bool);
    int Update();
    int Show(bool, bool);
    bool IsHidden();
    bool IsHiddenByUser();
    int LaunchEvent(int, GuiEventType, void*);
    int HandleImpulse(const m3d::AuxImpulseInfo*, m3d::ui::Wnd*);
    int HandleAppEvent(const m3d::Event*);
    int Save(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>);
    int Load(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>);
    ref_ptr<m3d::ui::Wnd>  GetWindow(int);
    int AddWindow(ref_ptr<m3d::ui::Wnd>, int*, bool, bool);
    int AddWindowById(ref_ptr<m3d::ui::Wnd>, int, bool, bool);
    int CreateAndAddWindow(int);
    int RemoveWindow(int);
    int ShowWindow(int, bool, bool, bool, bool, int*);
    int SetEventsForWindow(int, const std::vector<int>*);
    m3d::rend::TexHandle  GetIcoByName(const CStr*, int);
    CStr GetPathToQuestInfoFileGlobal();
    CStr GetPathToDialogsFileGlobal();
    CStr GetPathToDynamicDialogsFileGlobal();
    CStr GetPathToLevelInfoFile();
    CStr GetPathToSplashes();
    QuestInfoManager*  GetQuestInfoManager();
    RepliesManager*  GetRepliesManager();
    const ObjectCollection*  GetObjectCollection();
    const StringParser*  GetStringParser();
    LevelInfoManager*  GetLevelInfoManager();
    NavPointManager*  GetNavPointManager();
    WeaponGroupManager*  GetWeaponGroupManager();
    SavesManager*  GetSavesManager();
    MsgManager*  GetMsgManager();
    HelpManager*  GetHelpManager();
    bool IsModalEqualWndRunning();
    int GetDefaultFloatPrecision();
    bool IsGameModeValidForSmartCursor(GameState);
    void AddFadingMsg(CStr const&, std::vector<m3d::AIParam>&);
    void AddFadingMsgByStrId(CStr const&, std::vector<m3d::AIParam>&);
    void AddImportantFadingMsg(CStr const&, std::vector<m3d::AIParam> const&);
    void AddImportantFadingMsgByStrId(CStr const&, std::vector<m3d::AIParam>&);
    void StartSplashing(int);
    void ShowSplash(int, const CStr*);
    bool IsWindowVisible(int);
    bool IsWindowVisibleAndNotAnimating(int);
    int Str2WndGuiId(const CStr*);
    m3d::ui::MbRetCodes RunMsgBoxDlg(const CStr*, const CStr*, unsigned int, bool);
    const std::vector<int>*  GetTakenQuestIds();
    bool IsInSaleMode();
    ai::Workshop*  GetCurrentWorkshop();
    ai::Workshop*  GetCurrentShop();
    ai::Town*  GetCurrentTown();
    ai::Vehicle*  GetVehicleSellingInWorkshop();
    void OnLeaveTown(bool);
    void OnEnterTown(int);
};
