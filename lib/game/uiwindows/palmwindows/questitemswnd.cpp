#include "questitemswnd.h"

#include "checklist.h"

#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uiwindows/commonwindows/contextmodelwnd.h>
#include <m3dapp.h>
#include <server/objects/base/prototypemanager.h>
#include <server/objects/player.h>
#include <ui/textbox.h>

RT_CLASS_EXPORTS_BEGIN(QuestItemsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestItemsWnd);

QuestItemsWnd::AuxInfo::AuxInfo()
{
    // RVA 0x4FF510
    m_wndPictureName = "wndPicture";
    m_wndDizName = "wndDiz";
    m_wndItemsListName = "wndItemsList";
}

QuestItemsWnd::AuxInfo::AuxInfo(QuestItemsWnd::AuxInfo const& rhs) :
    m_wndPictureName(rhs.m_wndPictureName),
    m_wndDizName(rhs.m_wndDizName),
    m_wndItemsListName(rhs.m_wndItemsListName)
{
}

QuestItemsWnd::QuestItemsWnd() : m_wndPicture(nullptr), m_wndDiz(nullptr), m_wndItemsList(nullptr)
{
}

QuestItemsWnd::QuestItemsWnd(QuestItemsWnd const&) : QuestItemsWnd()
{
}

QuestItemsWnd::~QuestItemsWnd() = default;

m3d::Object* QuestItemsWnd::Clone()
{
    return new QuestItemsWnd(*this);
}

m3d::Object* QuestItemsWnd::CreateObject()
{
    return new QuestItemsWnd;
}

m3d::Class* QuestItemsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* QuestItemsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(QuestItemsWnd);
}

int QuestItemsWnd::GameDataSetup()
{
    // RVA 0x4FF720 - the picture and the list are built from XML pattern windows;
    // the description box is used as it comes.
    if ((m_gameDataFlags & 2) != 0)
    {
        return 1;
    }

    int res = 1;

    m3d::Object* dizChild = GetChildByName(m_aif.m_wndDizName);
    if (dizChild && dizChild->IsKindOf(&m3d::ui::TextBoxWnd::m_classTextBoxWnd))
    {
        m_wndDiz = static_cast<m3d::ui::TextBoxWnd*>(dizChild);
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndDizName + " is not found or incorrect type");
        res = 0;
    }

    m3d::Object* pictureChild = GetChildByName(m_aif.m_wndPictureName);
    if (pictureChild && pictureChild->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        m_wndPicture = static_cast<ContextModelWnd*>(m3d::g_Kernel->New("ContextModelWnd"));
        if (!m_wndPicture ||
            !m_wndPicture->CreateFromPattern(static_cast<m3d::ui::Wnd*>(pictureChild), true))
        {
            M3D_LOG_INFO(
                "Make control error: cannot create " + m_aif.m_wndPictureName + " from pattern class");
            res = 0;
        }
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndPictureName + " is not found or incorrect type");
        res = 0;
    }

    m3d::Object* listChild = GetChildByName(m_aif.m_wndItemsListName);
    if (listChild && listChild->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        m_wndItemsList = static_cast<CheckList*>(m3d::g_Kernel->New("CheckList"));
        if (!m_wndItemsList ||
            !m_wndItemsList->CreateFromPattern(static_cast<m3d::ui::Wnd*>(listChild), true))
        {
            M3D_LOG_INFO(
                "Make control error: cannot create " + m_aif.m_wndItemsListName + " from pattern class");
            res = 0;
        }
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndItemsListName + " is not found or incorrect type");
        res = 0;
    }

    if (res)
    {
        m_gameDataFlags |= 1u;
    }
    return res;
}

int QuestItemsWnd::GameDataClear(bool beforeContinuousLevel)
{
    // RVA 0x4FFE20
    if (!beforeContinuousLevel && (m_gameDataFlags & 1) != 0)
    {
        m_wndItemsList->RemoveAllItems();
    }
    return 1;
}

int QuestItemsWnd::GameDataUpdate(void*, int dataType)
{
    // RVA 0x4FFE50
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 77 && IsChildOf(M3D_APP))
    {
        FullUpdate();
    }
    return 1;
}

int QuestItemsWnd::OnBeforeAddToWndStation()
{
    // RVA 0x4FFEA0
    FullUpdate();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int QuestItemsWnd::OnAfterAddToWndStation()
{
    // RVA 0x5002E0
    int const res = m3d::ui::Wnd::OnAfterAddToWndStation();
    M3D_APP->EnqueueMessage(65691, 0, 0, 0, 0, {}, {});
    return res;
}

void QuestItemsWnd::FullUpdate()
{
    // RVA 0x4FFEB0 - rebuild the list, then put the selection back where it was
    // if that row still exists, otherwise fall back to the first row (or none).
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    int const prevSel = m_wndItemsList->GetCurSel();
    CreateItems();

    if (prevSel >= 0 && prevSel < m_wndItemsList->GetCount())
    {
        m_wndItemsList->SetCurSel(prevSel);
    }
    else
    {
        m_wndItemsList->SetCurSel(m_wndItemsList->GetCount() ? 0 : -1);
    }
}

void QuestItemsWnd::OnQuestItemsChanged()
{
    // RVA 0x4FFF80
    if (IsChildOf(M3D_APP))
    {
        FullUpdate();
    }
}

void QuestItemsWnd::CreateItems()
{
    // RVA 0x4FFFB0 - one row per quest item the player is carrying.
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndItemsList->RemoveAllItems();
    }
    if (!ai::thePlayer)
    {
        return;
    }
    auto const& itemPrototypeNames = ai::thePlayer->GetQuestItemPrototypeNames();
    for (unsigned i = 0; i < itemPrototypeNames.size(); ++i)
    {
        CStr const& prototypeName = itemPrototypeNames[i];
        m_wndItemsList->AddButtonByName(prototypeName, ai::thePrototypeManager->GetPrototypeFullName(prototypeName));
    }
}

void QuestItemsWnd::ClearItems()
{
    // RVA 0x500090
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndItemsList->RemoveAllItems();
    }
}

int QuestItemsWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned idFrom, unsigned message, m3d::AIParam const& data)
{
    // RVA 0x5000B0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, idFrom, message, data);
    }
    if (idFrom != 350 || message != 5)
    {
        return 0;
    }
    OnItemsListSelChanged(data);
    return 1;
}

void QuestItemsWnd::OnItemsListSelChanged(m3d::AIParam const& data)
{
    // RVA 0x500120 - the selection arrives as a range whose y is the new row.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    CVector2 const sel = data.GetAsRange();
    int const idx = static_cast<int>(sel.y);

    CheckButton* btn = (idx == -1) ? nullptr : m_wndItemsList->GetItem(idx);
    if (!btn)
    {
        m_wndPicture->DestroyObjToShow();
        m_wndPicture->ClearCurrentModelWnd();
        m_wndDiz->SetText({});
        return;
    }

    CStr const prototypeName = btn->GetName();
    int const prototypeId = ai::thePrototypeManager->GetPrototypeId(prototypeName);
    if (prototypeId == -1)
    {
        m_wndPicture->DestroyObjToShow();
        m_wndPicture->ClearCurrentModelWnd();
        m_wndDiz->SetText({});
        return;
    }

    m_wndPicture->ShowImageByPrototypeId0(prototypeId, true);
    m_wndDiz->SetText(M3D_APP->GetStringByStringId0(prototypeName + "_diz"));
}
