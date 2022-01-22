#include "gameuimanager.h"
#include <config.h>
#include <m3dapp.h>
#include <stdexcept>
#include <ui/wnd.h>
#include <core/ini.h>
#include <core/log.h>
#include <core/console/console.h>
#include <game/m3dgame.h>
#include <game/uimisc/objectsicons.h>

namespace
{
    struct _Str2ResourceLoadType
    {
        ResourceInfo::ResourceLoadType m_loadType;
        CStr m_str;
    };

    _Str2ResourceLoadType l_str2ResourceLoadType[] = {
        {ResourceInfo::LOADTYPE_AT_APP_START, "AT_APP_START"},
        {ResourceInfo::LOADTYPE_AT_FIRST_LEVEL_START, "AT_FIRST_LEVEL_START"},
        {ResourceInfo::LOADTYPE_AT_LEVEL_START, "AT_LEVEL_START"},
        {ResourceInfo::LOADTYPE_BY_DEMAND, "BY_DEMAND"},
    };
}

RT_CLASS_DEFINE(ResourceInfo);

m3d::Class* ResourceInfo::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ResourceInfo::CreateObject()
{
    return new ResourceInfo;
}

m3d::Class* ResourceInfo::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

ResourceInfo::ResourceLoadType ResourceInfo::Str2ResourceLoadType(CStr const& str)
{
    for (auto const& str2ResourceLoadType : l_str2ResourceLoadType)
    {
        if (str == str2ResourceLoadType.m_str)
        {
            return str2ResourceLoadType.m_loadType;
        }
    }
    return LOADTYPE_NUM_LOAD_TYPES;
}

int ResourceInfo::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    if (!xmlFile || !xmlNode)
    {
        return 0;
    }
    m3d::SafeStrAttrib(m_fileName, xmlNode, "file");
    CStr strLoadType;
    m3d::SafeStrAttrib(strLoadType, xmlNode, "loadType");
    if (!strLoadType.empty())
    {
        m_loadType = Str2ResourceLoadType(strLoadType);
    }
    return IsValid();
}

bool ResourceInfo::IsValid() const
{
    return m_loadType != LOADTYPE_NUM_LOAD_TYPES && !m_fileName.empty();
}

ResourceInfo::~ResourceInfo()
{
    throw std::logic_error("Not implemented");
}

ResourceInfo::ResourceInfo()
{
}

ResourceInfo::ResourceInfo(ResourceInfo const&)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_DEFINE(WindowResourceInfo);

m3d::Class* WindowResourceInfo::GetClass() const
{
    throw std::logic_error("Not implemented");
}

WindowResourceInfo::~WindowResourceInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WindowResourceInfo::CreateObject()
{
    return new WindowResourceInfo;
}

m3d::Class* WindowResourceInfo::GetBaseClass()
{
    return RT_CLASS_LOCAL(ResourceInfo);
}

int WindowResourceInfo::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    m3d::SafeStrAttrib(m_className, xmlNode, "class");
    m3d::SafeBoolAttrib(m_bShowImmediate, xmlNode, "showImmediate");
    CStr strWndGuiId;
    m3d::SafeStrAttrib(strWndGuiId, xmlNode, "id");
    m_wndGuiId = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp)->m_pInterfaceManager->Str2WndGuiId(strWndGuiId);
    return ResourceInfo::LoadFromXml(xmlFile, xmlNode);
}

bool WindowResourceInfo::IsValid() const
{
    return m_loadType != LOADTYPE_NUM_LOAD_TYPES
        && m3d::g_Kernel->FindClass(m_className.c_str())
        && m_wndGuiId != -1;
}

m3d::Object* WindowResourceInfo::Clone()
{
    throw std::logic_error("Not implemented");
}

WindowResourceInfo::WindowResourceInfo(WindowResourceInfo const&)
{
    throw std::logic_error("Not implemented");
}

WindowResourceInfo::WindowResourceInfo()
{
}

RT_CLASS_DEFINE(IcoResourceInfo);

int IcoResourceInfo::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    m3d::SafeStrAttrib(m_levelName, xmlNode, "level");
    return ResourceInfo::LoadFromXml(xmlFile, xmlNode);
}

m3d::Object* IcoResourceInfo::CreateObject()
{
    return new IcoResourceInfo;
}

m3d::Class* IcoResourceInfo::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* IcoResourceInfo::GetBaseClass()
{
    return RT_CLASS_LOCAL(ResourceInfo);
}

IcoResourceInfo::~IcoResourceInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* IcoResourceInfo::Clone()
{
    throw std::logic_error("Not implemented");
}

bool IcoResourceInfo::IsValid() const
{
    if (m_loadType == LOADTYPE_NUM_LOAD_TYPES)
    {
        return false;
    }
    if (m_fileName.empty())
    {
        return false;
    }
    if (m_levelName.empty() && m_loadType != LOADTYPE_AT_LEVEL_START)
    {
        return true;
    }
    if (m_levelName.empty() || m_loadType != LOADTYPE_AT_LEVEL_START)
    {
        return false;
    }
    return true;
}

IcoResourceInfo::IcoResourceInfo(IcoResourceInfo const&)
{
    throw std::logic_error("Not implemented");
}

IcoResourceInfo::IcoResourceInfo()
{
}

int GameUiManager::GUI_SetNextDynamicId(int)
{
    throw std::logic_error("Not implemented");
}

int GameUiManager::GUI_LoadResourceInfos()
{
    GUI_ClearAllResourceInfos();
    M3D_LOG_INFO("Interface: loading resource infos ...");
    auto res = GUI_LoadResourceInfosFromFile(m_cvPathToUiStrings.GetS(), m_resourceInfoStrings, "ResourceInfo") & 1;
    res = GUI_LoadResourceInfosFromFile(m_cvPathToUiIcons.GetS(), m_resourceInfoIcons, "IcoResourceInfo") & res;
    res = GUI_LoadResourceInfosFromFile(m_cvPathToUiWindows.GetS(), m_resourceInfoWindows, "WindowResourceInfo") & res;
    return res;
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

int GameUiManager::GUI_LoadResourceInfosFromFile(CStr const& fileName, std::vector<ResourceInfo*>& resourceInfos, CStr const& className)
{
    if (m_isInited)
    {
        return 0;
    }
    GUI_ClearResourceInfos(resourceInfos);
    if (fileName.empty())
    {
        M3D_LOG_INFO("Interface: error - resource file is not specified");
        return 0;
    }
    auto cls = m3d::g_Kernel->FindClass(className.c_str());
    if (!cls || !cls->IsKindOf(RT_CLASS_LOCAL(ResourceInfo)))
    {
        return 0;
    }
    CStr err;
    ref_ptr xmlFile = m3d::ReadXmlFile(fileName.c_str(), &err);
    if (xmlFile)
    {
        ref_ptr rootNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlFile->GetFirstChild_(rootNode, "GuiResourceInfo");
        if (rootNode->IsEmpty())
        {
            M3D_LOG_INFO("Interface: error - cannot find root node GuiResourceInfo in file " + fileName);
            return 0;
        }
        ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        auto res = 1;
        for(rootNode->GetFirstChild_(node, "Item");!node->IsEmpty(); node->GetNextSibling_(node, "Item"))
        {
            auto info = dynamic_cast<ResourceInfo*>(m3d::g_Kernel->New(className.c_str()));
            if (!info)
            {
                return 0;
            }
            if (info->LoadFromXml(xmlFile, node))
            {
                resourceInfos.push_back(info);
            }
            else
            {
                res = 0;
                delete info;
            }
        }
        if (!res)
        {
            M3D_LOG_INFO("Interface: resource infos loaded with errors from file " + fileName);
        }
        return res;
    }
    M3D_LOG_INFO("Interface: error - cannot load " + fileName + " err: " + err);
    return 0;
}

void GameUiManager::GUI_GetResourceInfosByLoadType(ResourceInfo::ResourceLoadType loadType, std::vector<ResourceInfo*> const& srcInfos, std::vector<ResourceInfo*>& dstInfos) const
{
    //TODO: check this
    dstInfos.clear();
    for (auto* info : srcInfos)
    {
        if (info && info->m_loadType == loadType)
        {
            dstInfos.push_back(info);
        }
    }
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

int GameUiManager::GUI_LoadResources(ResourceInfo::ResourceLoadType loadType)
{
    M3D_LOG_INFO("Interface: loading resources ...");
    auto res = GUI_LoadStringsResources(loadType) & 1;
    res = GUI_LoadIconsResources(loadType) & res;
    res = GUI_LoadWindowsResources(loadType) & res;
    return res;
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

int GameUiManager::GUI_LoadStringsResources(ResourceInfo::ResourceLoadType loadType)
{
    if (loadType == ResourceInfo::LOADTYPE_NUM_LOAD_TYPES)
    {
        return 0;
    }
    std::vector<ResourceInfo*> resourceInfosByLoadType;
    GUI_GetResourceInfosByLoadType(loadType, m_resourceInfoStrings, resourceInfosByLoadType);
    auto res = 1;
    for (auto* info : resourceInfosByLoadType)
    {
        if (info)
        {
            res &= GUI_LoadStringsFromResourceInfo(info);
        }
        else
        {
            res = 0;
        }
    }
    if (!res)
    {
        M3D_LOG_ERR("Interface: strings were loaded from resources with errors");
    }
    return res;
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

void GameUiManager::GUI_ClearResourceInfos(std::vector<ResourceInfo*>& resourceInfos)
{
    for (auto* info: resourceInfos)
    {
        delete info;
    }
    resourceInfos.clear();
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
    M3D_LOG_INFO("Interface: clearing resource infos ...");
    GUI_ClearResourceInfos(m_resourceInfoWindows);
    GUI_ClearResourceInfos(m_resourceInfoStrings);
    GUI_ClearResourceInfos(m_resourceInfoIcons);
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
