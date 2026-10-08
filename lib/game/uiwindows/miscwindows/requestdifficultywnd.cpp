#include "requestdifficultywnd.h"

#include "game/m3dgame.h"
#include "core/log.h"
#include "game/profile.h"
#include "server/objects/base/globalproperties.h"
#include "ui/comboboxwnd.h"

RT_CLASS_EXPORTS_BEGIN(RequestDifficultyWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RequestDifficultyWnd);

RequestDifficultyWnd::AuxInfo::AuxInfo()
{
    // RVA 0x4C53B0
    m_cbDifficultyLevelsName = "cbDifficultyLevels";
}

RequestDifficultyWnd::AuxInfo::AuxInfo(RequestDifficultyWnd::AuxInfo const& rhs) :
    m_cbDifficultyLevelsName(rhs.m_cbDifficultyLevelsName)
{
}

m3d::Class* RequestDifficultyWnd::GetClass() const
{
    return RT_CLASS_LOCAL(RequestDifficultyWnd);
}

m3d::Object* RequestDifficultyWnd::Clone()
{
    // RVA 0x4C5300
    return new RequestDifficultyWnd(*this);
}

RequestDifficultyWnd::~RequestDifficultyWnd() = default;

m3d::Object* RequestDifficultyWnd::CreateObject()
{
    return new RequestDifficultyWnd;
}

m3d::Class* RequestDifficultyWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

RequestDifficultyWnd::RequestDifficultyWnd() : m_cbDifficultyLevels(nullptr)
{
    // RVA 0x4C5400
}

RequestDifficultyWnd::RequestDifficultyWnd(RequestDifficultyWnd const&) : RequestDifficultyWnd()
{
    // NOTE: the shipped copy ctor (RVA 0x4C54C0) builds the base and the aux info
    // but - unlike the default ctor - never nulls m_cbDifficultyLevels, and copies
    // nothing from rhs. Delegating avoids leaving that pointer uninitialised while
    // copying just as little.
}

void RequestDifficultyWnd::FillDifficultyLevelsList()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        Clear();
        for (int i = 0; i < ai::theGlobProp.m_difficultyLevelCoeffs.size(); ++i)
        {
            auto id = m_cbDifficultyLevels->AddItem(M3D_APP->GetStringByStringId0(ai::theGlobProp.m_difficultyLevelCoeffs[i].m_name));
            if (id != -1)
            {
                m_cbDifficultyLevels->SetItemData(id, i);
            }
        }

        m_cbDifficultyLevels->SetCurSel(-1);
        const auto level = M3D_APP->GetCurDifficultyLevel();
        int const count = m_cbDifficultyLevels->GetCount();
        for (int i = 0; i < count; ++i)
        {
            if (m_cbDifficultyLevels->GetItemData(i) == level)
            {
                m_cbDifficultyLevels->SetCurSel(i);
                break;
            }
        }
        // RVA 0x4C5760: the fallback to the first row only fires when there is
        // one - an empty list is left with no selection rather than selecting 0.
        if (m_cbDifficultyLevels->GetCurSel() == -1 && count != 0)
        {
            m_cbDifficultyLevels->SetCurSel(0);
        }
    }
}

int RequestDifficultyWnd::OnKey(unsigned short key, unsigned char scanCode, unsigned state)
{
    // RVA 0x4C5990 - Escape is swallowed so the difficulty prompt cannot be
    // dismissed without answering it.
    if (key == 1)
    {
        return 1;
    }
    return m3d::ui::ModalWnd::OnKey(key, scanCode, state);
}

int RequestDifficultyWnd::OnBeforeRemoveFromWndStation()
{
    auto res = Wnd::OnBeforeRemoveFromWndStation();
    ApplyDifficultyLevel();
    return res;
}

void RequestDifficultyWnd::Clear()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_cbDifficultyLevels->RemoveAllItems();
        m_cbDifficultyLevels->SetText({});
    }
}

void RequestDifficultyWnd::ApplyDifficultyLevel()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        auto curSel = m_cbDifficultyLevels->GetCurSel();
        if (curSel != -1)
        {
            auto diffLevel = m_cbDifficultyLevels->GetItemData(curSel);
            if (auto* profile = M3D_APP->GetProfileManager()->GetCurProfile())
            {
                profile->SetParam(PP_DIFFICULTY_LEVEL, diffLevel);
            }
        }
    }
}

int RequestDifficultyWnd::OnBeforeAddToWndStation()
{
    FillDifficultyLevelsList();
    return Wnd::OnBeforeAddToWndStation();
}

int RequestDifficultyWnd::GameDataSetup()
{
    using namespace m3d::ui;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto diff = GetChildByName(m_aif.m_cbDifficultyLevelsName);
        if (diff && diff->IsKindOf(RT_CLASS_LOCAL(ComboBoxWnd)))
        {
            this->m_gameDataFlags |= 1u;
            m_cbDifficultyLevels = dynamic_cast<ComboBoxWnd*>(diff);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_cbDifficultyLevelsName + " is not found or incorrect type");
        }
    }
    if ((m_gameDataFlags & 1) != 0)
        return 1;

    M3D_LOG_INFO("RequestDifficultyWnd: error - fail to init because of a bad resource");
    return 0;
}
