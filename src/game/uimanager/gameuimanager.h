#pragma once
#include <map>
#include <vector>
#include <core/clazz.h>
#include <core/ref_ptr.h>
#include <core/console/cvar.h>

class ObjectsIcons;

namespace m3d
{
    namespace ui
    {
        class Wnd;
    }
}

enum GuiEventType
{
    GUI_EVENT_FROM_PACKET = 0x0,
    GUI_EVENT_FROM_IMPULSE = 0x1,
    GUI_EVENT_FROM_APPEVENT = 0x2,
    GUI_EVENT_CUSTOM = 0x3,
};

class ResourceInfo : public m3d::Object
{
public:
    enum ResourceLoadType
    {
        LOADTYPE_AT_APP_START = 0x0,
        LOADTYPE_AT_FIRST_LEVEL_START = 0x1,
        LOADTYPE_AT_LEVEL_START = 0x2,
        LOADTYPE_BY_DEMAND = 0x3,
        LOADTYPE_NUM_LOAD_TYPES = 0x4,
    };

public:
    virtual m3d::Class* WindowGetClass() const;
    virtual m3d::Class* GetClass() const;
    static ResourceLoadType __fastcall Str2ResourceLoadType(CStr const&);
    virtual int IcoLoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    static m3d::Object* __fastcall IcoCreateObject();
    virtual ~ResourceInfo();
    virtual m3d::Class* IcoGetClass() const;
    static m3d::Object* __fastcall WindowCreateObject();
    static m3d::Class* __fastcall IcoGetBaseClass();
    static m3d::Object* CreateObject();
    static m3d::Class* __fastcall WindowGetBaseClass();
    virtual int WindowLoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    virtual bool WindowIsValid() const;
    virtual m3d::Object* IcoClone();
    virtual m3d::Object* WindowClone();
    virtual bool IcoIsValid() const;

protected:
    ResourceInfo();
    ResourceInfo(ResourceInfo const&);

private:
    CStr m_fileName;
    ResourceLoadType m_loadType;
};

class WindowResourceInfo :  public ResourceInfo
{
public:
    virtual m3d::Class* GetClass() const ;
    virtual ~WindowResourceInfo();
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual int LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
    virtual bool IsValid() const ;
    virtual m3d::Object* Clone();

protected:
    WindowResourceInfo(WindowResourceInfo const &);
    WindowResourceInfo();

private:
    int m_wndGuiId;
    CStr m_className;
    bool m_bShowImmediate;
};

class IcoResourceInfo :  public ResourceInfo
{
public:
    virtual int LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
    static m3d::Object * CreateObject();
    virtual m3d::Class* GetClass() const ;
    static m3d::Class* GetBaseClass();
    virtual ~IcoResourceInfo();
    virtual m3d::Object * Clone();
    virtual bool IsValid() const ;

protected:
    IcoResourceInfo(IcoResourceInfo const &);
    IcoResourceInfo();

private:
    CStr m_levelName;
};

class GameUiManager
{
public:
protected:
    int GUI_SetNextDynamicId(int);
    int GUI_LoadResourceInfos();
    int GUI_LoadIconsResources(ResourceInfo::ResourceLoadType);
    int GUI_LoadWindowsResources(ResourceInfo::ResourceLoadType);
    int GUI_SetMinDynamicId(int);
    virtual int GUI_SetEventsForWindow(int, std::vector<int, std::allocator<int> > const&);
    virtual int GUI_UpdateWindowsOnEvent(int, m3d::ui::Wnd*, void*);
    int GUI_LoadResourceInfosFromFile(CStr const&, std::vector<ResourceInfo*, std::allocator<ResourceInfo*> >&, CStr const&);
    void GUI_GetResourceInfosByLoadType(ResourceInfo::ResourceLoadType, std::vector<ResourceInfo*, std::allocator<ResourceInfo*> > const&, std::vector<ResourceInfo*, std::allocator<ResourceInfo*> >&) const;
    virtual int GUI_Save(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>);
    int GUI_CreateWindow(int, CStr const&, bool, CStr const&);
    GameUiManager();
    virtual bool GUI_IsWndModalEqual(m3d::ui::Wnd*) const;
    int GUI_LoadStringsFromResourceInfo(ResourceInfo const*);
    WindowResourceInfo* GUI_GetResourceInfoByWndGuiId(int) const;
    virtual bool GUI_IsModalEqualWndRunning() const;
    virtual int GUI_ProcessEvent(GuiEventType, int, void*, m3d::ui::Wnd*);
    void GUI_GetIconsResourceInfoByLevel(CStr const&, std::vector<ResourceInfo*, std::allocator<ResourceInfo*> >&) const;
    int GUI_LoadResources(ResourceInfo::ResourceLoadType);
    virtual int GUI_Init(bool);
    virtual int GUI_RemoveWindow(ref_ptr<m3d::ui::Wnd>);
    virtual int GUI_RemoveWindow(int);
    virtual void GUI_UnregisterCVars();
    virtual int GUI_ReadFromXml(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>);
    virtual void GUI_RegisterEvents();
    int GUI_LoadStringsResources(ResourceInfo::ResourceLoadType);
    virtual int GUI_ShowInterface(bool, bool);
    virtual int GUI_Load(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>);
    virtual int GUI_Clear(bool);
    virtual int GUI_AddWindow(ref_ptr<m3d::ui::Wnd>, int&, bool, bool);
    virtual int GUI_HandleEvent(int, m3d::ui::Wnd*, void*);
    virtual ~GameUiManager();
    virtual void GUI_RegisterCVars();
    int GUI_LoadWindowFromResourceInfo(WindowResourceInfo const*);
    void GUI_ClearResourceInfos(std::vector<ResourceInfo*, std::allocator<ResourceInfo*> >&);
    virtual int GUI_HideWindow(int, bool, int*, bool);
    virtual int GUI_Done();
    void GUI_ClearAllResourceInfos();
    virtual ref_ptr<m3d::ui::Wnd> GUI_GetWindow(int) const;
    bool GUI_IsCurrentLevelMainMenuLevel() const;
    int GUI_LoadIconsFromResourceInfo(IcoResourceInfo const*);
    bool GUI_IsHidden() const;
    virtual int GUI_WriteToXml(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>);
    virtual int GUI_ShowWindow(int, bool, bool, bool, int*);
    virtual int GUI_AddWindowById(ref_ptr<m3d::ui::Wnd>, int, bool, bool);
    int GUI_ValidateDynamicId(int);
    virtual void GUI_RegisterClasses() = 0;
    virtual int GUI_BindWindowsToEvents() = 0;

protected:
    std::map<int, int> m_packToEvent;
    std::map<int, int> m_impulseToEvent;
    std::map<int, int> m_eventToEvent;
    m3d::CVar m_cvPathToUiWindows;
    m3d::CVar m_cvPathToUiStrings;
    m3d::CVar m_cvPathToUiIcons;
    std::vector<ResourceInfo*> m_resourceInfoWindows;
    std::vector<ResourceInfo*> m_resourceInfoStrings;
    std::vector<ResourceInfo*> m_resourceInfoIcons;
    bool m_bFirstLevelResourcesLoaded;
    ObjectsIcons* m_icons = nullptr;
    bool m_isInited = false;
    bool m_oneTimeStuffIsInited = false;
    std::map<int, ref_ptr<m3d::ui::Wnd>> m_windows;
    std::set<int> m_onScreenWindows;
    std::map<int, std::set<int>> m_eventMap;
    bool m_isHidden = true;
    int m_nextDynamicId = 0;
    int m_minDynamicId = 0;
    bool m_isEventMapValide = true;
};
