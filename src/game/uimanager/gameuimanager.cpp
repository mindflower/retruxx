#include "gameuimanager.h"
#include <stdexcept>
#include <ui/wnd.h>
#include <core/ini.h>

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

int GameUiManager::GUI_Init(bool)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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
