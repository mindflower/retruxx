#include "counterwnd.h"

#include "core/kernel.h"
#include "core/log.h"
#include "electronicdigitalwnd.h"
#include "game/m3dgame.h"
#include "game/uimanager/uidefs.h"
#include "maingameinterfacewnd.h"

#include "server/objects/base/objcontainer.h"
#include "server/objects/dynamicquesthunt.h"

RT_CLASS_EXPORTS_BEGIN(CounterWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CounterWnd);

CounterWnd::AuxInfo::AuxInfo()
{
    m_wndCounterName = "wndCounter";
    m_strHuntTime = "HuntTime";
}

m3d::Class* CounterWnd::GetClass() const
{
    return RT_CLASS_LOCAL(CounterWnd);
}

m3d::Class* CounterWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* CounterWnd::CreateObject()
{
    return new CounterWnd;
}

bool CounterWnd::NeedShow() const
{
    return m_counterType != COUNTERTYPE_NUM_COUNTER_TYPES;
}

CounterWnd::~CounterWnd()
{
    // RVA 0x119E50 - m_aif's CStr members and the Wnd base clean up
    // automatically.
}

m3d::Object* CounterWnd::Clone()
{
    // RVA 0x119C10
    return new CounterWnd(*this);
}

void CounterWnd::ClearCounter()
{
    // RVA 0x11A240
    // NOTE: the shipped code passes a bogus "this" (literal 1) to
    // ElectronicDigitalWnd::Clear here, a decompiler register-tracking
    // artifact; m_wndCounter is clearly the intended target.
    if ((m_gameDataFlags & 1) != 0)
    {
        m_counterType = COUNTERTYPE_NUM_COUNTER_TYPES;
        m_dQuestId = -1;
        m_wndCounter->Clear();
    }
}

CounterWnd::CounterWnd(CounterWnd const&) : CounterWnd()
{
    // NOTE: the shipped copy ctor (RVA 0x119E30) default-constructs the base
    // and re-inits the AuxInfo, but leaves m_wndCounter/m_counterType/
    // m_dQuestId uninitialized; delegating to the default ctor here avoids
    // reading an uninitialized pointer while still copying nothing from the
    // source.
}

CounterWnd::CounterWnd()
{
    m_wndCounter = 0;
    m_counterType = COUNTERTYPE_NUM_COUNTER_TYPES;
    m_dQuestId = -1;
}

void CounterWnd::OnStartLevel(void* data)
{
    // RVA 0x11A690 - on a level start (*data == 2), tracks the active hunt quest taken last.
    if (!data || *static_cast<int*>(data) != 2)
    {
        return;
    }
    std::set<int> const* hunts =
        M3D_APP->m_pInterfaceManager->GetObjectCollection().GetObjectsByClass(&ai::DynamicQuestHunt::m_classDynamicQuestHunt);
    if (!hunts)
    {
        return;
    }
    ai::GameTime maxTime;
    int latestQuestId = -1;
    for (int id : *hunts)
    {
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(id);
        if (!obj || !obj->IsKindOf(&ai::DynamicQuestHunt::m_classDynamicQuestHunt))
        {
            continue;
        }
        auto* quest = static_cast<ai::DynamicQuestHunt*>(obj);
        if (quest->GetQuestStatus() != ai::DynamicQuest::STATUS_PROCESSING)
        {
            continue;
        }
        // NOTE: >=, so of equal take times the last one in id order wins.
        if (quest->GetTakeGameTime().asInt64() >= maxTime.asInt64())
        {
            maxTime = quest->GetTakeGameTime();
            latestQuestId = id;
        }
    }
    if (latestQuestId != -1)
    {
        SetupForHuntQuest(latestQuestId);
    }
}

int CounterWnd::SetupForHuntQuest(int dQuestId)
{
    // RVA 0x11A400
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    m_counterType = COUNTERTYPE_NUM_COUNTER_TYPES;
    m_dQuestId = -1;
    m_wndCounter->Clear();

    if (dQuestId < 0)
    {
        return 0;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(dQuestId);
    if (!obj || !obj->IsKindOf(&ai::DynamicQuestHunt::m_classDynamicQuestHunt))
    {
        return 0;
    }

    m_counterType = COUNTERTYPE_QUEST_HUNT;
    m_dQuestId = dQuestId;
    UpdateCounter();
    CheckAndShow();
    return 1;
}

void CounterWnd::GetMinAndSecFromTime(float time, int& minutes, int& secundes) const
{
    // RVA 0x11A600
    int const t = static_cast<int>(time);
    int const min = (t % 3600) / 60;
    minutes = min;
    secundes = t - 60 * (min + 60 * (t / 3600));
}

int CounterWnd::GameDataClear(bool beforeContinuousLevel)
{
    // RVA 0x11A200
    if (!beforeContinuousLevel)
    {
        if ((m_gameDataFlags & 1) != 0)
        {
            m_counterType = COUNTERTYPE_NUM_COUNTER_TYPES;
            m_dQuestId = -1;
            m_wndCounter->Clear();
        }
        CheckAndShow();
    }
    return 1;
}

ai::DynamicQuestHunt const* CounterWnd::GetQuestHunt() const
{
    // RVA 0x11A570
    if (m_counterType != COUNTERTYPE_QUEST_HUNT)
    {
        return nullptr;
    }
    if (m_dQuestId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_dQuestId);
    return (obj && obj->IsKindOf(&ai::DynamicQuestHunt::m_classDynamicQuestHunt)) ?
        static_cast<ai::DynamicQuestHunt const*>(obj) :
        nullptr;
}

int CounterWnd::GameDataSetup()
{
    // RVA 0x119EC0
    using namespace m3d::ui;

    if ((m_gameDataFlags & 2) == 0)
    {
        if (auto* child = RT_DYNCAST(GetChildByName(m_aif.m_wndCounterName), Wnd))
        {
            m_wndCounter = static_cast<ElectronicDigitalWnd*>(M3D_KERNEL->New("ElectronicDigitalWnd"));
            if (m_wndCounter)
            {
                if (m_wndCounter->CreateFromPattern(child, true))
                {
                    m_wndCounter->SetDigitalSize(ElectronicDigitalWnd::DIGITAL_SIZE_SMALL);
                    m_gameDataFlags |= 1u;
                }
                else
                {
                    M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndCounterName + " from pattern class");
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_aif.m_wndCounterName +
                    " - cannot find rtti class ElectronicDigitalWnd");
            }
        }
        else
        {
            M3D_LOG_INFO("Make control error: control " + m_aif.m_wndCounterName + " is not found or incorrect type");
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("CounterWnd: error - fail to init because of a bad resource");
    return 0;
}

void CounterWnd::OnDynamicQuestStateChanged(void* data)
{
    // RVA 0x11A2D0 - a hunt quest turning active (STATUS_PROCESSING) gets tracked; the tracked one leaving it (STATUS_COMPLETE to STATUS_FORGOTTEN) is
    // dropped. The quest id is at +0x34 of the event data.
    if ((m_gameDataFlags & 1) == 0 || !data)
    {
        return;
    }
    int const dQuestId = static_cast<int*>(data)[13];
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(dQuestId);
    if (!obj || !obj->IsKindOf(&ai::DynamicQuest::m_classDynamicQuest))
    {
        return;
    }
    auto* quest = static_cast<ai::DynamicQuest*>(obj);
    if (quest->GetQuestType() != ai::DynamicQuestManager::TYPE_HUNT)
    {
        return;
    }
    ai::DynamicQuest::QuestStatus const status = quest->GetQuestStatus();
    if (status == ai::DynamicQuest::STATUS_PROCESSING)
    {
        SetupForHuntQuest(dQuestId);
    }
    else if (status > ai::DynamicQuest::STATUS_PROCESSING && status <= ai::DynamicQuest::STATUS_FORGOTTEN && m_counterType == COUNTERTYPE_QUEST_HUNT && m_dQuestId == dQuestId)
    {
        GameDataClear(false);
    }
}

void CounterWnd::CheckAndShow()
{
    // RVA 0x11A380
    if (auto wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_MAIN_GAME_INTERFACE))
    {
        if (IS_KIND_OF(wnd.get(), MainGameInterfaceWnd))
        {
            RT_DYNCAST(wnd.get(), MainGameInterfaceWnd)->CheckAndShowCounterWnd();
        }
    }
}

float CounterWnd::GetHuntTimeLeft() const
{
    // RVA 0x11A5E0
    if (auto const* questHunt = GetQuestHunt())
    {
        return questHunt->GetTimeBeforeHuntEnd();
    }
    return -1.0f;
}

int CounterWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x11A270
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    switch (dataType)
    {
    case 'C':
        OnDynamicQuestStateChanged(data);
        break;
    case 'U':
        OnStartLevel(data);
        return 1;
    case 'Y':
        UpdateCounter();
        return 1;
    default:
        break;
    }
    return 1;
}

void CounterWnd::UpdateCounter()
{
    // RVA 0x11A4B0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (m_counterType != COUNTERTYPE_QUEST_HUNT)
    {
        if (m_counterType == COUNTERTYPE_NUM_COUNTER_TYPES)
        {
            // NOTE: matches the shipped build, which calls this and discards
            // the result here - apparently dead code left over in the
            // original source.
            IsAnimatingNow();
        }
        return;
    }

    int minutes = 0;
    int secundes = 0;
    if (auto const* questHunt = GetQuestHunt())
    {
        GetMinAndSecFromTime(questHunt->GetTimeBeforeHuntEnd(), minutes, secundes);
    }
    else
    {
        GetMinAndSecFromTime(-1.0f, minutes, secundes);
    }
    m_wndCounter->ShowNumber(minutes, false, 2u, true);
    m_wndCounter->ShowSymbol(ElectronicDigitalWnd::SYMBOL_COLON, true, 1u, false);
    m_wndCounter->ShowNumber(secundes, true, 2u, true);
}

void CounterWnd::OnNewFrame()
{
    // RVA 0x11A4A0
    UpdateCounter();
}
