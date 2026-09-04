#include "fadingmsg.h"

#include "core/kernel.h"
#include "core/log.h"
#include "core/timer.h"
#include "game/m3dgame.h"
#include "game/uimanager/truxxuimanager.h"
#include "game/uimisc/guihelper.h"
#include "game/uimisc/stringparser.h"
#include "ui/ui_srv.h"

RT_CLASS_EXPORTS_BEGIN(FadingMsgList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(FadingMsgList);

FadingMsgList::AuxInfo::AuxInfo()
{
    m_space = 8.0;
    m_unimportantExtraH = 40.0;
    m_importantExtraH = 46.0;
    m_unimportantMaxNumOfMsgs = 3;
    m_importantMaxNumOfMsgs = 1;
}

m3d::Class* FadingMsgList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

namespace
{
    // NOTE: FormatParam has no public constructor taking an m3d::AIParam in
    // this codebase (the shipped build's equivalent overload could not be
    // recovered from the decompile); this reconstructs the conversion via
    // the closest matching public accessors.
    FormatParam ToFormatParam(m3d::AIParam const& param)
    {
        if (param.GetType() == m3d::AIPARAM_STRING)
        {
            return FormatParam(param.GetAsStr());
        }
        return FormatParam(param.GetAsID());
    }
}  // namespace

int FadingMsgList::AddMsgT(CStr const& msg, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const& params)
{
    // RVA 0x121020
    CStr formattedMsg = msg;
    if (!params.empty())
    {
        std::vector<FormatParam> formatParams;
        formatParams.reserve(params.size());
        for (auto const& param : params)
        {
            formatParams.push_back(ToFormatParam(param));
        }
        M3D_APP->m_pInterfaceManager->GetStringParser().FormatString(formattedMsg, formatParams);
    }

    auto* msgWnd = static_cast<FadingMsgItem*>(M3D_KERNEL->New("FadingMsgItem"));
    if (!msgWnd)
    {
        return 0;
    }

    float const originY = (m_msgType == MSGTYPE_UNIMPORTANT) ? m_bounds.height : 0.0f;
    PointBase<float> const origin{0.0f, originY};
    if (!msgWnd->SetupForMsg(formattedMsg, origin, m_bounds.width, m_msgType))
    {
        msgWnd->DecRef();
        return 0;
    }

    AddChild(msgWnd);
    M3D_LOG_INFO("Fading message: '" + msg + "'");
    return InsertMsg(msgWnd);
}

int FadingMsgList::AddMsgByStrId(
    CStr const& strId,
    std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const& params)
{
    // RVA 0x121400
    CStr const msg = M3D_APP->GetStringByStringId0(strId);
    return AddMsgT(msg, params);
}

int FadingMsgList::AddMsg(CStr const& msg, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const& params)
{
    // RVA 0x1213A0
    return AddMsgT(msg, params);
}

m3d::Object* FadingMsgList::Clone()
{
    // RVA 0x120C80
    return new FadingMsgList(*this);
}

FadingMsgList::~FadingMsgList()
{
    // RVA 0x120EC0
    GameDataClear(false);
}

m3d::Class* FadingMsgList::GetClass() const
{
    return RT_CLASS_LOCAL(FadingMsgList);
}

m3d::Object* FadingMsgList::CreateObject()
{
    return new FadingMsgList;
}

void FadingMsgList::OnNewFrame()
{
    // RVA 0x121690
    for (auto* item : m_msgList)
    {
        item->Update();
    }
    while (!m_msgList.empty() && m_msgList.front()->m_state == FadingMsgItem::STATE_HIDE)
    {
        PopFront();
    }
}

int FadingMsgList::PushBack(FadingMsgItem* msgWnd)
{
    // RVA 0x121530
    if (!msgWnd)
    {
        return 0;
    }
    m_msgList.push_back(msgWnd);
    RecalcLayot();
    return 1;
}

float FadingMsgList::GetSummaryHeight()
{
    // RVA 0x1219C0
    if (m_msgList.empty())
    {
        return 0.0f;
    }
    float total = 0.0f;
    for (auto* item : m_msgList)
    {
        total += item->GetBounds().height;
    }
    return total + static_cast<float>(m_msgList.size() - 1) * m_aif.m_space;
}

int FadingMsgList::AddEnumeredMsg(int msgId, m3d::Event const* ev)
{
    // TODO(RVA 0x1221C0): looks msgId up in a shipped static table
    // (l_msgId2Name, 18 entries) mapping msgId -> a localizable string id
    // name, then formats it with per-msgId parameters (see
    // GetParamsForEnumeredMsg) and adds it via AddMsgT. Blocked on
    // recovering that static table's contents, which are not visible in the
    // decompile text (only its address, unk_A45BAC, is) - would need a
    // further targeted IDA data dump to reconstruct faithfully.
    (void)msgId;
    (void)ev;
    return 0;
}

void FadingMsgList::GetParamsForEnumeredMsg(
    int msgId,
    m3d::Event const* ev,
    std::vector<m3d::AIParam, std::allocator<m3d::AIParam>>& params) const
{
    // TODO(RVA 0x1222A0): builds the AIParam list for a specific enumerated
    // msgId (quest briefs, impulse names, event strings, ...); blocked on
    // the same missing l_msgId2Name table as AddEnumeredMsg, since the
    // msgId->case mapping is keyed off table indices not visible here.
    (void)msgId;
    (void)ev;
    params.clear();
}

void FadingMsgList::RecalcLayot()
{
    // RVA 0x121740
    // NOTE: the shipped code computes this same stacking layout via a maze
    // of aliased pointers/registers that could not be transcribed
    // byte-exact; reconstructed here as the clearly-intended behavior - the
    // newest message (deque back) anchors to the panel edge in m_direction,
    // and each older message stacks further away by the neighboring
    // message's height plus m_aif.m_space.
    if (m_msgList.empty())
    {
        return;
    }

    float anchor = (m_direction == DIR_DOWN) ? 0.0f : m_bounds.height;
    for (auto it = m_msgList.rbegin(); it != m_msgList.rend(); ++it)
    {
        FadingMsgItem* item = *it;
        BoundsBase<float> bounds = item->GetBounds();
        if (m_direction == DIR_DOWN)
        {
            bounds.y0 = anchor;
            anchor += bounds.height + m_aif.m_space;
        }
        else
        {
            anchor -= bounds.height;
            bounds.y0 = anchor;
            anchor -= m_aif.m_space;
        }
        item->SetBounds(bounds, true);
    }
}

CStr FadingMsgList::MsgId2Name(int msgId) const
{
    // TODO(RVA 0x122520): see AddEnumeredMsg - blocked on the missing
    // l_msgId2Name static table.
    (void)msgId;
    return {};
}

int FadingMsgList::InsertMsg(FadingMsgItem* newMsg)
{
    // RVA 0x121450
    if (!newMsg)
    {
        return 0;
    }
    int const maxMsgs =
        (m_msgType == MSGTYPE_IMPORTANT) ? m_aif.m_importantMaxNumOfMsgs : m_aif.m_unimportantMaxNumOfMsgs;
    float const msgHeight = newMsg->GetBounds().height;

    while (!m_msgList.empty())
    {
        float const needed = m_aif.m_space + GetSummaryHeight() + msgHeight;
        if (m_maxSummaryH >= needed && static_cast<int>(m_msgList.size()) < maxMsgs)
        {
            break;
        }
        if (m_extraH + m_maxSummaryH >= needed && static_cast<int>(m_msgList.size()) == maxMsgs)
        {
            Scroll(newMsg);
            return 1;
        }
        PopFront();
    }
    PushBack(newMsg);
    return 1;
}

void FadingMsgList::OnFadingMessage(void* data)
{
    // RVA 0x1221B0
    if (data)
    {
        auto const* ev = static_cast<m3d::Event const*>(data);
        AddEnumeredMsg(ev->m_intEv[0], ev);
    }
}

FadingMsgList::FadingMsgList()
{
    m_direction = DIR_UP;
    m_msgType = MSGTYPE_INVALID;
    m_maxSummaryH = 0.0;
    m_extraH = 0.0;
}

FadingMsgList::FadingMsgList(FadingMsgList const&) : FadingMsgList()
{
    // NOTE: the shipped copy ctor (RVA 0x120E50) default-constructs the base,
    // an empty deque, and re-inits the AuxInfo, but leaves m_msgType/
    // m_maxSummaryH/m_extraH/m_direction uninitialized; delegating to the
    // default ctor here reproduces "nothing copied from source" without
    // relying on garbage memory for those fields.
}

int FadingMsgList::PopFront()
{
    // RVA 0x1215E0
    if (m_msgList.empty())
    {
        return 0;
    }
    FadingMsgItem* front = m_msgList.front();
    if (front)
    {
        front->DecRef();
    }
    m_msgList.pop_front();
    RecalcLayot();
    return 1;
}

int FadingMsgList::Scroll(FadingMsgItem* newMsg)
{
    // RVA 0x121560
    if (m_msgList.empty())
    {
        return 0;
    }
    if (!newMsg)
    {
        return 0;
    }
    FadingMsgItem* oldest = m_msgList.front();
    oldest->StartFade();
    m_msgList.push_back(newMsg);
    RecalcLayot();
    return 1;
}

int FadingMsgList::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x120FE0
    if (dataType >= 79 && dataType <= 80)
    {
        if (data)
        {
            auto const* ev = static_cast<m3d::Event const*>(data);
            AddEnumeredMsg(ev->m_intEv[0], ev);
        }
    }
    else if (dataType == 89)
    {
        OnNewFrame();
        return 1;
    }
    return 1;
}

int FadingMsgList::GameDataSetup()
{
    // RVA 0x120EF0
    if ((m_gameDataFlags & 2) == 0)
    {
        bool ok = true;
        if (m_guiId == 125)
        {
            m_msgType = MSGTYPE_UNIMPORTANT;
            m_direction = DIR_UP;
            m_extraH = m_aif.m_unimportantExtraH;
        }
        else if (m_guiId == 126)
        {
            m_msgType = MSGTYPE_IMPORTANT;
            m_direction = DIR_DOWN;
            m_extraH = m_aif.m_importantExtraH;
        }
        else
        {
            ok = false;
        }

        m_maxSummaryH = m_bounds.height - m_extraH;
        if (ok)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("FadingMsgList: error - fail to init because of a bad resource");
    return 0;
}

int FadingMsgList::GameDataClear(bool)
{
    // RVA 0x121920
    for (auto* item : m_msgList)
    {
        if (item)
        {
            item->DecRef();
        }
    }
    m_msgList.clear();
    return 1;
}

RT_CLASS_EXPORTS_BEGIN(FadingMsgItem)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(FadingMsgItem);

m3d::Class* FadingMsgItem::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

// NOTE: the exact offset-to-field mapping for these defaults could not be
// fully disambiguated from the decompile (the ctor operates on a raw `int`
// "this", losing field names, while call sites elsewhere resolved names via
// IDA's type info); this is a best-effort reconstruction consistent with
// every observed default value and usage. Purely cosmetic (fade timing,
// message-box sizing/colors) - no logic depends on getting this exact.
FadingMsgItem::AuxInfo::AuxInfo()
{
    m_showTime = 5000;
    m_fadeTime = 1500;
    m_unimportantTextColor = static_cast<unsigned int>(-1);
    m_importantTextColor = static_cast<unsigned int>(-1);
    m_importantFontSize = 16.0f;
    m_texIdBg = "FadingMsgBg";
    m_unimportantStyle = 2816u;
    m_importantStyle = 3840u;
    m_edge = PointBase<float>{10.0f, 5.0f};
    m_unimportantMinH = 32.0f;
    m_importantMinH = 38.0f;
    m_unimportantTextFormat = 1u;
    m_importantTextFormat = 0u;
}

m3d::Object* FadingMsgItem::Clone()
{
    // RVA 0x120CB0
    return new FadingMsgItem(*this);
}

int FadingMsgItem::SetupForMsg(
    CStr const& msg,
    PointBase<float> const& origin,
    float maxW,
    FadingMsgList::MsgType msgType)
{
    // RVA 0x121C70
    if (msgType == FadingMsgList::MSGTYPE_INVALID)
    {
        return 0;
    }
    m_msgType = msgType;

    bool const important = (msgType == FadingMsgList::MSGTYPE_IMPORTANT);
    int const fontId = important ? help::GetScaledFontId(0, m_aif.m_importantFontSize) : 1;
    SetDefaultFont(fontId);
    m_textWrap = m3d::TW_WORD_WRAP;

    PointBase<float> const textSize =
        m3d::ui::Wnd::GetGfxServer()->MeasureText(msg, m_defFont, m3d::TW_WORD_WRAP, maxW - m_aif.m_edge.x * 2.0f);

    float boxH = m_aif.m_edge.y * 2.0f + textSize.y;
    float const minH = important ? m_aif.m_importantMinH : m_aif.m_unimportantMinH;
    if (minH > boxH)
    {
        boxH = minH;
    }
    unsigned int const style = important ? m_aif.m_importantStyle : m_aif.m_unimportantStyle;

    BoundsBase<float> b;
    b.x0 = origin.x;
    b.width = maxW;
    b.height = boxH;
    b.y0 = important ? origin.y : origin.y - boxH;

    if (!Create(msg, style, b, 0))
    {
        return 0;
    }

    SetBackground(M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texIdBg, 0));
    SetClientEdges(m_aif.m_edge.x, m_aif.m_edge.y, m_aif.m_edge.x, m_aif.m_edge.y);

    SetTextColor(important ? m_aif.m_importantTextColor : m_aif.m_unimportantTextColor);
    SetFormatMode(
        static_cast<m3d::TextFormatFlags>(important ? m_aif.m_importantTextFormat : m_aif.m_unimportantTextFormat));

    m_showTime = 0;
    return 1;
}

FadingMsgItem::~FadingMsgItem()
{
    // RVA 0x121C60 - the Wnd base cleans up automatically.
}

m3d::Class* FadingMsgItem::GetClass() const
{
    return RT_CLASS_LOCAL(FadingMsgItem);
}

m3d::Object* FadingMsgItem::CreateObject()
{
    // RVA 0x121AD0
    return new FadingMsgItem;
}

void FadingMsgItem::Update()
{
    // RVA 0x121EB0
    m_showTime += M3D_KERNEL->GetTimer().GetLastFrameTime();
    if (m_state != STATE_SHOW)
    {
        if (m_state == STATE_FADE)
        {
            if (m_showTime < m_aif.m_fadeTime)
            {
                Fade();
            }
            else
            {
                m_state = STATE_HIDE;
            }
        }
    }
    else if (m_showTime >= m_aif.m_showTime)
    {
        m_state = STATE_FADE;
        m_showTime = 0;
    }
}

void FadingMsgItem::StartFade()
{
    // RVA 0x121E90
    if (m_state != STATE_FADE)
    {
        m_state = STATE_FADE;
        m_showTime = 0;
    }
}

void FadingMsgItem::Fade()
{
    // RVA 0x121F20
    float t = 1.0f - static_cast<float>(m_showTime) / static_cast<float>(m_aif.m_fadeTime);
    int alphaInt = static_cast<int>(t * 255.0f);
    if (alphaInt < 1)
    {
        alphaInt = 1;
    }
    else if (alphaInt > 255)
    {
        alphaInt = 255;
    }
    unsigned int const alpha = static_cast<unsigned int>(alphaInt);

    // NOTE: the shipped code also rewrites any embedded "@rrggbb"-style color
    // markers within the message text with the current fade alpha; that
    // per-character text-markup rewrite could not be reconstructed
    // confidently from the decompile and is omitted here - the window's
    // background and text alpha are still faded.
    unsigned int const bgColor = m3d::ui::Wnd::GetGfxServer()->GetColor(m_curClr);
    SetColor((alpha << 24) | (bgColor & 0xFFFFFFu));

    unsigned int const textColor = m3d::ui::Wnd::GetGfxServer()->GetColor(m_textColor);
    SetTextColor((alpha << 24) | (textColor & 0xFFFFFFu));
}

FadingMsgItem::FadingMsgItem(FadingMsgItem const&) : FadingMsgItem()
{
    // Matches the original (RVA 0x121C50): the copy ctor default-constructs
    // only the base; nothing is copied from the source.
}

FadingMsgItem::FadingMsgItem()
{
    // RVA 0x121BF0
    m_state = STATE_SHOW;
    m_showTime = 0;
    m_msgType = FadingMsgList::MSGTYPE_INVALID;
}
