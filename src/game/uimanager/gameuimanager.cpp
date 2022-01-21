#include "gameuimanager.h"
#include <config.h>
#include <stdexcept>
#include <ui/wnd.h>
#include <core/ini.h>
#include <core/log.h>
#include <core/console/console.h>
#include <game/uimisc/objectsicons.h>

int GameUiManager::GUI_SetNextDynamicId(int)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_LoadResourceInfos()
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_LoadIconsResources(ResourceInfo::ResourceLoadType)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_LoadWindowsResources(ResourceInfo::ResourceLoadType)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_SetMinDynamicId(int)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_SetEventsForWindow(int, std::vector<int, std::allocator<int>> const&)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_UpdateWindowsOnEvent(int, m3d::ui::Wnd*, void*)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_LoadResourceInfosFromFile(CStr const&, std::vector<ResourceInfo*, std::allocator<ResourceInfo*>>&, CStr const&)
{
    throw std::logic_error("Not implemented");
}

void GameUiManager::GUI_GetResourceInfosByLoadType(ResourceInfo::ResourceLoadType, std::vector<ResourceInfo*, std::allocator<ResourceInfo*>> const&, std::vector<ResourceInfo*, std::allocator<ResourceInfo*>>&) const
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_Save(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_CreateWindow(int, CStr const&, bool, CStr const&)
{
    throw std::logic_error("Not implemented");
}

GameUiManager::GameUiManager()
{
}

bool GameUiManager::GUI_IsWndModalEqual(m3d::ui::Wnd*) const
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_LoadStringsFromResourceInfo(ResourceInfo const*)
{
    throw std::logic_error("Not implemented");
}

WindowResourceInfo* GameUiManager::GUI_GetResourceInfoByWndGuiId(int) const
{
    throw std::logic_error("Not implemented");
}

bool GameUiManager::GUI_IsModalEqualWndRunning() const
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_ProcessEvent(GuiEventType, int, void*, m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

void GameUiManager::GUI_GetIconsResourceInfoByLevel(CStr const&, std::vector<ResourceInfo*, std::allocator<ResourceInfo*>>&) const
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_LoadResources(ResourceInfo::ResourceLoadType)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_Init(bool reloadResources)
{
    M3D_LOG_INFO("Interface: is initializing...");
    if (!m_icons)
    {
        m_icons = new ObjectsIcons;
    }
    auto res = 0;
    if (!m_oneTimeStuffIsInited || reloadResources)
    {
        if (m_isInited)
        {
            GUI_Done();
        }
        if (!m_oneTimeStuffIsInited)
        {
            GUI_RegisterEvents();
            GUI_RegisterCVars();
            GUI_RegisterClasses();
            if (!GUI_LoadResourceInfos())
            {
                res = 0;
            }
            if (!GUI_LoadResources(ResourceInfo::LOADTYPE_AT_APP_START))
            {
                res = 0;
            }
            if (!GUI_BindWindowsToEvents())
            {
                res = 0;
            }
            m_oneTimeStuffIsInited = true;
            m_bFirstLevelResourcesLoaded = true;
        }
    }
    else
    {
        if (m_isInited)
        {
            GUI_Clear(false);
        }
        for (auto& window : m_windows)
        {
            window.second->GameDataSetup();
        }
    }
    m_isInited = true;
    if (res)
    {
        M3D_LOG_INFO("Interface: is inited successfully");
    }
    else
    {
        M3D_LOG_INFO("Interface: is inited with errors");
    }
    return res;
}

int GameUiManager::GUI_RemoveWindow(ref_ptr<m3d::ui::Wnd>)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_RemoveWindow(int)
{
    throw std::logic_error("Not implemented");
}

void GameUiManager::GUI_UnregisterCVars()
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_ReadFromXml(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>)
{
    throw std::logic_error("Not implemented");
}

void GameUiManager::GUI_RegisterEvents()
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_LoadStringsResources(ResourceInfo::ResourceLoadType)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_ShowInterface(bool, bool)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_Load(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_Clear(bool)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_AddWindow(ref_ptr<m3d::ui::Wnd>, int&, bool, bool)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_HandleEvent(int, m3d::ui::Wnd*, void*)
{
    throw std::logic_error("Not implemented");
}

GameUiManager::~GameUiManager()
{
    throw std::logic_error("Not implemented");
}

void GameUiManager::GUI_RegisterCVars()
{
    m_cvPathToUiWindows.Init("pathToUiWindows", "data\\if\\dialogs\\UiWindows.xml", m3d::CVar::CVAR_STRING, m3d::CVar::CVAR_ARCHIVE);
    m_cvPathToUiStrings.Init("pathToUiStrings", "data\\if\\strings\\UiStrings.xml", m3d::CVar::CVAR_STRING, m3d::CVar::CVAR_ARCHIVE);
    m_cvPathToUiIcons.Init("pathToUiIcons", "data\\if\\ico\\UiIcons.xml", m3d::CVar::CVAR_STRING, m3d::CVar::CVAR_ARCHIVE);

    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToUiWindows, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToUiStrings, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToUiIcons, nullptr);
}

int GameUiManager::GUI_LoadWindowFromResourceInfo(WindowResourceInfo const*)
{
    throw std::logic_error("Not implemented");
}

void GameUiManager::GUI_ClearResourceInfos(std::vector<ResourceInfo*, std::allocator<ResourceInfo*>>&)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_HideWindow(int, bool, int*, bool)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_Done()
{
    throw std::logic_error("Not implemented");
}

void GameUiManager::GUI_ClearAllResourceInfos()
{
    throw std::logic_error("Not implemented");
}

ref_ptr<m3d::ui::Wnd> GameUiManager::GUI_GetWindow(int) const
{
    throw std::logic_error("Not implemented");
}

bool GameUiManager::GUI_IsCurrentLevelMainMenuLevel() const
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_LoadIconsFromResourceInfo(IcoResourceInfo const*)
{
    throw std::logic_error("Not implemented");
}

bool GameUiManager::GUI_IsHidden() const
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_WriteToXml(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_ShowWindow(int, bool, bool, bool, int*)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_AddWindowById(ref_ptr<m3d::ui::Wnd>, int, bool, bool)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_ValidateDynamicId(int id)
{
    if (m_windows.find(id) == end(m_windows))
    {
        return id;
    }
    auto maxId = id;
    for (auto const& window : m_windows)
    {
        if (window.first > maxId)
        {
            maxId = window.first;
        }
    }
    return maxId + 1;
}
