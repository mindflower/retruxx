#include "towndlg.h"

#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>

#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>

#include <ui/image.h>

#include <server/objects/bar.h>
#include <server/objects/town.h>
#include <server/objects/base/objcontainer.h>

RT_CLASS_EXPORT_METHOD_DEFINE(TownDlg, SetConditionalClosingForTown)
{
    auto* dlg = static_cast<TownDlg*>(context->asObject(0, "TownDlg"));
    dlg->SetConditionalClosingForTown(context->asString(1), context->asString(2), context->asBool(3));
    return 0;
}

RT_CLASS_EXPORT_METHOD_DEFINE(TownDlg, IsTownWithConditionalClosing)
{
    auto* dlg = static_cast<TownDlg*>(context->asObject(0, "TownDlg"));
    context->pushBool(dlg->IsTownWithConditionalClosing(context->asString(1), context->asString(2)));
    return 1;
}

RT_CLASS_EXPORTS_BEGIN(TownDlg)
	RT_CLASS_EXPORT(TownDlg, m3d::METHOD, SetConditionalClosingForTown, "", "", "")
	RT_CLASS_EXPORT(TownDlg, m3d::METHOD, IsTownWithConditionalClosing, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TownDlg);

// ===========================================================================
//  TownDlg::AuxInfo
// ===========================================================================

TownDlg::AuxInfo::AuxInfo()
{
    // RVA 0xA3390
    m_lblTownNameName = "lblTownName";
    m_lstBuildingsName = "lstBuildingPattern";
    m_wndClanPictureName = "wndClanPicture";
}

TownDlg::AuxInfo::AuxInfo(TownDlg::AuxInfo const& o)
{
    m_lblTownNameName = o.m_lblTownNameName;
    m_lstBuildingsName = o.m_lstBuildingsName;
    m_wndClanPictureName = o.m_wndClanPictureName;
}

// ===========================================================================
//  TownDlg
// ===========================================================================

TownDlg::TownDlg()
{
    m_lblTownName = nullptr;
    m_wndClanPicture = nullptr;
    m_townId = -1;
}

TownDlg::TownDlg(TownDlg const&) : TownDlg()
{
    // Matches the original: the copy ctor default-constructs the base, the
    // AuxInfo and an empty list. Reached only via Clone().
}

TownDlg::~TownDlg()
{
    // RVA 0xA3780
    if ((m_gameDataFlags & 1) != 0)
    {
        m_townId = -1;
        ClearConditionalClosingInfo();
    }
    // The std::list frees its own nodes; the entries were released above.
}

m3d::Object* TownDlg::Clone()
{
    // RVA 0xA3330
    return new TownDlg(*this);
}

m3d::Object* TownDlg::CreateObject()
{
    return new TownDlg;
}

m3d::Class* TownDlg::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

m3d::Class* TownDlg::GetClass() const
{
    return RT_CLASS_LOCAL(TownDlg);
}

ai::Town* TownDlg::GetTown() const
{
    // RVA 0xA3B60
    if (m_townId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_townId);
    if (obj && obj->IsKindOf(&ai::Town::m_classTown))
    {
        return static_cast<ai::Town*>(obj);
    }
    return nullptr;
}

int TownDlg::GameDataSetup()
{
    // RVA 0xA37F0
    if ((m_gameDataFlags & 2) == 0)
    {
        bool ok = true;

        m3d::Object* lbl = GetChildByName(m_aif.m_lblTownNameName);
        if (lbl && lbl->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_lblTownName = static_cast<m3d::ui::Wnd*>(lbl);
        }
        else
        {
            M3D_LOG_INFO(
                CStr("Get control error: control ") + m_aif.m_lblTownNameName + " is not found or incorrect type");
            ok = false;
        }

        m3d::Object* clan = GetChildByName(m_aif.m_wndClanPictureName);
        if (clan && clan->IsKindOf(&m3d::ui::ImageWnd::m_classImageWnd))
        {
            m_wndClanPicture = static_cast<m3d::ui::ImageWnd*>(clan);
            if (ok)
            {
                m_gameDataFlags |= 1u;
            }
        }
        else
        {
            M3D_LOG_INFO(
                CStr("Get control error: control ") + m_aif.m_wndClanPictureName + " is not found or incorrect type");
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("TownDlg: error - fail to init because of a bad resource");
    return 0;
}

int TownDlg::GameDataClear(bool beforeContinuousLevel)
{
    // RVA 0xA3AD0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    m_townId = -1;
    if (!beforeContinuousLevel)
    {
        ClearConditionalClosingInfo();
    }
    return 1;
}

int TownDlg::GameDataUpdate(void* data, int dataType)
{
    // RVA 0xA3B00
    switch (dataType)
    {
    case '9':  // 57
        if (data)
        {
            ShowForTown(reinterpret_cast<int*>(data)[13]);
        }
        break;
    case 'h':  // 104
        OnEnterBar(reinterpret_cast<int*>(data)[13]);
        return 1;
    case 'i':  // 105
        OnLeaveBar();
        return 1;
    }
    return 1;
}

int TownDlg::OnBeforeAddToWndStation()
{
    // RVA 0xA3BC0
    FullUpdate();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int TownDlg::OnAfterAddToWndStation()
{
    // RVA 0xA4930
    int const res = m3d::ui::Wnd::OnAfterAddToWndStation();
    M3D_APP->EnqueueMessage(65691, 0, 0, 0, 0, {}, {});
    return res;
}

void TownDlg::FullUpdate()
{
    // RVA 0xA3BE0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (ai::Town* town = GetTown())
    {
        m_lblTownName->SetText(ai::theObjects->GetObjectFullName(town->GetName()));
    }
    else
    {
        m_lblTownName->SetText(CStr());
    }
    UpdateClanPicture();
}

int TownDlg::SetUpForTown(int townId)
{
    // RVA 0xA3CA0
    m_townId = townId;
    if (IsChildOf(static_cast<m3d::Object*>(M3D_APP)))
    {
        FullUpdate();
    }
    return 1;
}

void TownDlg::OnEnterBar(int barId)
{
    // RVA 0xA3CD0
    if ((m_gameDataFlags & 1) == 0 || barId < 0)
    {
        return;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(barId);
    if (obj && obj->IsKindOf(&ai::Bar::m_classBar))
    {
        m_lblTownName->SetText(ai::theObjects->GetObjectFullName(obj->GetName()));
    }
}

void TownDlg::OnLeaveBar()
{
    // RVA 0xA3D80
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (ai::Town* town = GetTown())
    {
        m_lblTownName->SetText(ai::theObjects->GetObjectFullName(town->GetName()));
    }
    else
    {
        m_lblTownName->SetText(CStr());
    }
}

void TownDlg::ShowForTown(int townId)
{
    // RVA 0xA3E30
    m_townId = townId;
    if (IsChildOf(static_cast<m3d::Object*>(M3D_APP)))
    {
        FullUpdate();
    }
    if (GetTown())
    {
        M3D_APP->EnqueueMessage(65674, GetGuiId(), 0, 0, 0, {}, {});
    }
    else
    {
        M3D_APP->EnqueueMessage(65673, GetGuiId(), 0, 0, 0, {}, {});
    }
}

void TownDlg::UpdateClanPicture()
{
    // RVA 0xA48A0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (ai::Town* town = GetTown())
    {
        // NOTE: the shipped build toggles the picture's visibility through a
        // virtual (vtable slot 0xdc == ShowWindow) and reads the clan from the
        // town before fetching its icon.
        m_wndClanPicture->ShowWindow(true);
        m3d::rend::TexHandle const tex =
            M3D_APP->m_pInterfaceManager->GetIcoByName(help::GetClanNameByBelong(town->GetBelong()), 1);
        m_wndClanPicture->SetImage(tex);
    }
    else
    {
        m_wndClanPicture->ShowWindow(false);
    }
}

// --- conditional-closing bookkeeping -------------------------------------------

void TownDlg::SetConditionalClosingForTown(CStr const& townName, CStr const& levelName, bool bConditionalClosing)
{
    // RVA 0xA3F30
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (bConditionalClosing)
    {
        AddConditionalClosingInfo(townName, levelName);
    }
    else
    {
        RemoveConditionalClosingInfo(townName, levelName);
    }
}

bool TownDlg::IsTownWithConditionalClosing(CStr const& townName, CStr const& levelName) const
{
    // RVA 0xA49D0
    return GetConditionalClosingInfoForTown(townName, levelName) != nullptr;
}

TownDlg::ConditionalClosingInfo* TownDlg::GetConditionalClosingInfoForTown(
    CStr const& townName,
    CStr const& levelName) const
{
    // RVA 0xA41B0
    for (ConditionalClosingInfo* info : m_conditionalClosingInfoList)
    {
        if (info && info->m_townName == townName &&
            CStr::my_strcmp(info->m_levelName.c_str(), levelName.c_str()) == 0)
        {
            return info;
        }
    }
    return nullptr;
}

void TownDlg::AddConditionalClosingInfo(CStr const& townName, CStr const& levelName)
{
    // RVA 0xA3F60
    if (GetConditionalClosingInfoForTown(townName, levelName))
    {
        return;
    }
    auto* info = new ConditionalClosingInfo();
    info->m_townName = townName;
    info->m_levelName = levelName;
    m_conditionalClosingInfoList.push_back(info);
}

void TownDlg::RemoveConditionalClosingInfo(CStr const& townName, CStr const& levelName)
{
    // RVA 0xA4020
    for (auto it = m_conditionalClosingInfoList.begin(); it != m_conditionalClosingInfoList.end(); ++it)
    {
        ConditionalClosingInfo* info = *it;
        if (info && info->m_townName == townName &&
            CStr::my_strcmp(info->m_levelName.c_str(), levelName.c_str()) == 0)
        {
            delete info;
            m_conditionalClosingInfoList.erase(it);
            return;
        }
    }
}

void TownDlg::ClearConditionalClosingInfo()
{
    // RVA 0xA4250
    for (ConditionalClosingInfo* info : m_conditionalClosingInfoList)
    {
        delete info;
    }
    m_conditionalClosingInfoList.clear();
}

// --- serialization -----------------------------------------------------------

int TownDlg::GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    // RVA 0xA4310
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("TownDlg::GameDataSave error - window has been not properly inited");
        return 0;
    }
    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("TownDlg::GameDataSave error - invalid params");
        return 0;
    }

    ref_ptr townDlgNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "TownDlg");
    guiNode->AddChild(townDlgNode);

    int res = 1;
    for (ConditionalClosingInfo* info : m_conditionalClosingInfoList)
    {
        if (!info)
        {
            res = 0;
            continue;
        }
        ref_ptr infoNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "ConditionalClosingInfo");
        townDlgNode->AddChild(infoNode);
        infoNode->SetAttribute("TownName", info->m_townName.c_str());
        infoNode->SetAttribute("LevelName", info->m_levelName.c_str());
    }
    if (!res)
    {
        M3D_LOG_INFO("TownDlg was saved with errors");
    }
    return res;
}

int TownDlg::GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    // RVA 0xA4580
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("TownDlg::GameDataLoad error - window has been not properly inited");
        return 0;
    }
    GameDataClear(false);
    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("TownDlg::GameDataLoad error - invalid params");
        return 0;
    }

    // NOTE: the shipped build first descends into an intermediate wrapper node
    // ("journal node") before reading <TownDlg>; since GameDataSave writes the
    // <TownDlg> element straight under guiNode, it is read back directly here.
    ref_ptr townDlgNode = xmlFile->CreateNode();
    guiNode->GetFirstChild(townDlgNode, "TownDlg");
    if (townDlgNode->IsEmpty())
    {
        M3D_LOG_INFO("TownDlg::GameDataLoad error - cannot find node");
        return 0;
    }

    ref_ptr infoNode = xmlFile->CreateNode();
    townDlgNode->GetFirstChild(infoNode, "ConditionalClosingInfo");
    while (!infoNode->IsEmpty())
    {
        CStr townName;
        CStr levelName;
        m3d::SafeStrAttrib(townName, infoNode, "TownName");
        m3d::SafeStrAttrib(levelName, infoNode, "LevelName");
        AddConditionalClosingInfo(townName, levelName);
        infoNode->GetNextSibling(infoNode, "ConditionalClosingInfo");
    }
    return 1;
}
