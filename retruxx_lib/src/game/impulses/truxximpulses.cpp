#include "truxximpulses.h"
#include <stdexcept>

RT_CLASS_EXPORTS_BEGIN(TruxxImpulse)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TruxxImpulse);

namespace
{
    struct AuxIdToName
    {
        CStr m_name;
        int m_id;
    };

    AuxIdToName l_gameModeNames[] = {
        {"GS_GAME", 0},
        {"GS_CINEMATIC", 1},
        {"GS_MAINMENU" , 2},
        {"GS_INITIALIZATION" , 3},
    };

    AuxIdToName l_impulseNames[] = {
        {"IM_MODE_GAME_MENU", 0},
        {"IM_FWD", 4},
        {"IM_BK", 5},
        {"IM_LEFT", 6},
        {"IM_RIGHT", 7},
        {"IM_GAME_MS_MOVE", 10},
        {"IM_GAME_MS_MOVE_ON_UI", 11},
        {"IM_GAME_ZOOM_CAMERA", 8},
        {"IM_GAME_DRAG_CAMERA", 9},
        {"IM_DEBUG_0", 12},
        {"IM_DEBUG_1", 13},
        {"IM_DEBUG_2", 14},
        {"IM_DEBUG_3", 15},
        {"IM_DEBUG_4", 16},
        {"IM_DEBUG_5", 17},
        {"IM_DEBUG_6", 18},
        {"IM_DEBUG_7", 19},
        {"IM_DEBUG_8", 20},
        {"IM_DEBUG_9", 21},
        {"IM_DEBUG_WIREFRAME", 22},
        {"IM_CAR_ACC", 23},
        {"IM_CAR_BREAK", 24},
        {"IM_CAR_HAND_BREAK", 25},
        {"IM_CAR_LEFT", 26},
        {"IM_CAR_RIGHT", 27},
        {"IM_CAR_HORN", 28},
        {"IM_CAR_TURNTOWHEELS", 29},
        {"IM_CAR_SWITCHCAMERA", 30},
        {"IM_CAR_LIGHTS", 31},
        {"IM_CAR_GET_OUT_OF_DIFFICULT_PLACE", 32},
        {"IM_CINEMATIC_SKIP", 40},
        {"IM_CINEMATIC_SKIP_MSG", 41},
        {"IM_UI_INVENTORY", 42},
        {"IM_UI_MENUBOOK", 43},
        {"IM_UI_QUESTLOG", 44},
        {"IM_UI_JOURNAL", 45},
        {"IM_UI_MAP", 46},
        {"IM_UI_VEHICLE_INFO", 47},
        {"IM_PAUSE", 51},
        {"IM_CAR_FIRE_0", 33},
        {"IM_CAR_FIRE_1", 34},
        {"IM_CAR_FIRE_2", 35},
        {"IM_CAR_FIRE_3", 36},
        {"IM_CAR_FIRE_4", 37},
        {"IM_CAR_FIRE_ALL", 38},
        {"IM_UI_TOGGLE_INTERFACE", 52},
        {"IM_QUICK_SAVE", 53},
        {"IM_QUICK_LOAD", 54},
        {"IM_RELOAD_WEAPON", 39},
        {"IM_UI_BAR", 48},
        {"IM_UI_ADDITIONAL_BUILDING", 49},
        {"IM_UI_PICKUP_ALL", 55},
        {"IM_UI_HELP", 50},
    };
}

int TruxxImpulse::HandleImpulse(m3d::AuxImpulseInfo const&, m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TruxxImpulse::GetImpulseIdByName(CStr const& impName)
{
    if (!impName.empty())
    {
	    for (auto const& name : l_impulseNames)
	    {
		    if (name.m_name == impName)
		    {
                return name.m_id;
		    }
	    }
    }
    return -1;
}

int TruxxImpulse::GetGameModeIdByName(CStr const& modeName)
{
    for (auto const& mode : l_gameModeNames)
    {
	    if (mode.m_name == modeName)
	    {
            return mode.m_id;
	    }
    }
    return -1;
}

CStr TruxxImpulse::GetImpulseNameById(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr TruxxImpulse::GetGameModeNameById(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* TruxxImpulse::GetClass() const
{
    return RT_CLASS_LOCAL(TruxxImpulse);
}

m3d::Object* TruxxImpulse::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

TruxxImpulse::~TruxxImpulse()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* TruxxImpulse::CreateObject()
{
    return new TruxxImpulse;
}

m3d::Class* TruxxImpulse::GetBaseClass()
{
    return RT_CLASS_LOCAL(GameImpulse);
}

CStr TruxxImpulse::GetProfileFolder()
{
    RETRUXX_NOT_IMPLEMENTED;
}

TruxxImpulse::TruxxImpulse()
{
}

TruxxImpulse::TruxxImpulse(TruxxImpulse const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
