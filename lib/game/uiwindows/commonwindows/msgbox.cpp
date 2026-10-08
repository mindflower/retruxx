#include "msgbox.h"
#include "core/log.h"
#include "game/uimisc/guihelper.h"
#include "ui/button.h"
#include "ui/linewnd.h"
#include "ui/ui_srv.h"
#include "ui/wndstation.h"

RT_CLASS_EXPORTS_BEGIN(MsgBox)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MsgBox);

int MsgBox::m_ref = 0;
m3d::ui::Wnd* MsgBox::m_pattern = nullptr;

MsgBox::AuxInfo::AuxInfo()
{
}

int MsgBox::CreateMsgBox(CStr const& caption, CStr const& message, unsigned flags)
{
    if (Valid() || (m_gameDataFlags & 1) != 0 || !m_pattern || !help::CloneWndWithChildren(m_pattern, this) ||
        !GameDataSetup())
    {
        return 0;
    }
    m_msgBoxFlags = flags;
    m_msg = message;
    m_title = caption;
    RecalcLayot();
    return 1;
}

m3d::Class* MsgBox::GetClass() const
{
    return RT_CLASS_LOCAL(MsgBox);
}

m3d::Object* MsgBox::Clone()
{
    // RVA 0x516050: allocates a fresh MsgBox and runs the default ctor - the
    // source is ignored, so a clone starts empty.
    return new MsgBox();
}

MsgBox::~MsgBox()
{
    delete m_wndTitle;
    delete m_wndMsg;
    delete m_wndUpLine;
    delete m_wndFrame;
    delete m_wndBg;
    for (auto& emboss : m_idioticEmbosses)
    {
        delete emboss;
    }
    if (m_ref == 1)
    {
        delete m_pattern;
    }
    --m_ref;
}

m3d::Class* MsgBox::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

m3d::Object* MsgBox::CreateObject()
{
    return new MsgBox;
}

void MsgBox::SetIdioticEmbossesBounds(PointBase<float> const& idioticEmbossesSz)
{
    // RVA 0x5189F0. Same shape as SetButtonsBounds (and the same original bug);
    // reconstructed the same way with the embossName controls.
    if ((m_gameDataFlags & 1) != 0 && (m_msgBoxFlags & 3) != 0)
    {
        auto patE1 = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_idioticEmbossName + CStr(1)))->GetBounds();
        auto patE0 = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_idioticEmbossName + CStr(0)))->GetBounds();
        float const gap = patE1.x0 - (patE0.x0 + patE0.width);

        auto downLineBounds = m_wndDownLine->GetBounds();
        auto patDownLine = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_wndDownLineName))->GetBounds();

        float x = (m_bounds.width - idioticEmbossesSz.x) * 0.5f;
        for (int i = 0; i < 3; ++i)
        {
            if (IsDirectChild(m_idioticEmbosses[i]))
            {
                auto embossBounds = m_idioticEmbosses[i]->GetBounds();
                auto patE = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_idioticEmbosses[i]->GetName()))->GetBounds();
                float const deltaY = patE.y0 - (patDownLine.height + patDownLine.y0);

                embossBounds.x0 = x;
                embossBounds.y0 = downLineBounds.y0 + downLineBounds.height + deltaY;
                m_idioticEmbosses[i]->SetBounds(embossBounds, true);

                x += embossBounds.width + gap;
            }
        }
    }
}

MsgBox::MsgBox(MsgBox const&)
{
    // RVA 0x516480: default-constructs the ModalWnd base and members and copies
    // nothing from the source; unlike the default ctor it does NOT bump m_ref.
    // Clone() does not use this - it default-constructs.
}

MsgBox::MsgBox()
{
    ++m_ref;
}

void MsgBox::AddTitle()
{
    if ((m_gameDataFlags & 1) != 0 && !m_title.empty())
    {
        if (m_wndTitle)
        {
            if (!m_wndTitle->GetParent())
            {
                AddChild(m_wndTitle);
            }
        }
        m_wndTitle->SetText(m_title);
    }
}

void MsgBox::SetMsgBounds(PointBase<float> const& msgSz)
{
    // RVA 0x518460
    if ((m_gameDataFlags & 1) != 0 && IsDirectChild(m_wndMsg))
    {
        BoundsBase<float> msgB;
        msgB.width = msgSz.x;
        msgB.height = msgSz.y;
        msgB.x0 = (m_bounds.width - msgSz.x) * 0.5f;

        float y0;
        if (IsDirectChild(m_wndTitle))
        {
            auto lineUpB = m_wndUpLine->GetBounds();
            auto patMsg = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_wndMsgName))->GetBounds();
            auto patUp = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_wndUpLineName))->GetBounds();
            y0 = (patMsg.y0 - (patUp.height + patUp.y0)) + lineUpB.height + lineUpB.y0;
        }
        else
        {
            y0 = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_wndTitleName))->GetBounds().y0;
        }
        msgB.y0 = y0;
        m_wndMsg->SetBounds(msgB, true);
    }
}

void MsgBox::AddMsg()
{
    if ((m_gameDataFlags & 1) != 0 && !m_msg.empty())
    {
        if (m_wndMsg)
        {
            if (!m_wndMsg->GetParent())
            {
                AddChild(m_wndMsg);
            }
        }
        m_wndMsg->SetText(m_msg);
    }
}

void MsgBox::AddLines()
{
    // RVA 0x517B10
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    bool const hasTitle = !m_title.empty();
    bool const hasMsg = !m_msg.empty();
    bool const hasButtons = (m_msgBoxFlags & 3) != 0;

    if (hasTitle && hasMsg && m_wndUpLine && !m_wndUpLine->GetParent())
    {
        AddChild(m_wndUpLine);
    }
    if (hasButtons && (hasMsg || hasTitle) && m_wndDownLine && !m_wndDownLine->GetParent())
    {
        AddChild(m_wndDownLine);
    }
}

void MsgBox::SetDownLineBounds()
{
    // RVA 0x5185B0. NOTE: the shipped body decompiles with corrupt locals;
    // reconstructed to mirror SetUpLineBounds/SetMsgBounds.
    if ((m_gameDataFlags & 1) != 0 && IsDirectChild(m_wndDownLine))
    {
        auto const patDown = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_wndDownLineName))->GetBounds();

        BoundsBase<float> lineB;
        lineB.x0 = patDown.x0;
        lineB.width = m_bounds.width - patDown.x0 * 2.0f;
        lineB.height = m_wndDownLine->GetBounds().height;

        if (IsDirectChild(m_wndMsg))
        {
            auto const patMsg = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_wndMsgName))->GetBounds();
            auto const msgB = m_wndMsg->GetBounds();
            lineB.y0 = (patDown.y0 - (patMsg.height + patMsg.y0)) + msgB.height + patMsg.height;
        }
        else
        {
            auto const patUp = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_wndUpLineName))->GetBounds();
            auto const patTitle = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_wndTitleName))->GetBounds();
            auto const titleB = m_wndTitle->GetBounds();
            lineB.y0 = (patUp.y0 - (patTitle.height + patTitle.y0)) + titleB.height + patTitle.height;
        }
        m_wndDownLine->SetBounds(lineB, true);
    }
}

PointBase<float> MsgBox::CalcSummaryButtonsSize() const
{
    // RVA 0x517D50
    if ((m_gameDataFlags & 1) != 0)
    {
        float buttonsWidthSum = 0.0f;
        float buttonsMaxSzY = 0.0f;
        for (int i = 0; i < 3; ++i)
        {
            if (IsDirectChild(m_buttons[i]))
            {
                auto bounds = m_buttons[i]->GetBounds();
                buttonsWidthSum += bounds.width;
                if (bounds.height > buttonsMaxSzY)
                {
                    buttonsMaxSzY = bounds.height;
                }
            }
        }
        float spacing = 0.0f;
        unsigned const n = m_msgBoxFlags & 3;
        if (n > 1)
        {
            auto b1 = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_buttonName + CStr(1)))->GetBounds();
            auto b0 = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_buttonName + CStr(0)))->GetBounds();
            float const gap = b1.x0 - (b0.width + b0.x0);
            spacing = static_cast<float>(static_cast<int>(n) - 1) * gap;
        }
        PointBase<float> res;
        res.x = spacing + buttonsWidthSum;
        res.y = buttonsMaxSzY;
        return res;
    }
    return {};
}

void MsgBox::Clear()
{
    m_msgBoxFlags = 0;
    m_title = "";
    m_msg = "";
    RecalcLayot();
}

PointBase<float> MsgBox::CalcTitleSize() const
{
    if ((m_gameDataFlags & 1) != 0)
    {
        if (!m_title.empty())
        {
            auto child = m_pattern->GetChildByName(m_aif.m_wndTitleName);
            auto wnd = dynamic_cast<Wnd*>(child);
            auto bounds = wnd->GetBounds();
            return GetGfxServer()->MeasureText(
                m_title, m_wndTitle->GetDefaultFont(), m_wndTitle->GetWrapMode(), bounds.width);
        }
    }
    return {};
}

PointBase<float> MsgBox::CalcSummaryIdioticEmbossesSize() const
{
    // RVA 0x517FC0
    if ((m_gameDataFlags & 1) != 0)
    {
        float embossesWidthSum = 0.0f;
        float embossesMaxSzY = 0.0f;
        for (int i = 0; i < 3; ++i)
        {
            if (IsDirectChild(m_idioticEmbosses[i]))
            {
                auto bounds = m_idioticEmbosses[i]->GetBounds();
                embossesWidthSum += bounds.width;
                if (bounds.height > embossesMaxSzY)
                {
                    embossesMaxSzY = bounds.height;
                }
            }
        }
        float spacing = 0.0f;
        unsigned const n = m_msgBoxFlags & 3;
        if (n > 1)
        {
            auto b1 = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_idioticEmbossName + CStr(1)))->GetBounds();
            auto b0 = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_idioticEmbossName + CStr(0)))->GetBounds();
            float const gap = b1.x0 - (b0.width + b0.x0);
            spacing = static_cast<float>(static_cast<int>(n) - 1) * gap;
        }
        PointBase<float> res;
        res.x = spacing + embossesWidthSum;
        res.y = embossesMaxSzY;
        return res;
    }
    return {};
}

void MsgBox::SetUpLineBounds()
{
    // RVA 0x518310. NOTE: the shipped body decompiles with corrupt locals;
    // reconstructed to mirror SetDownLineBounds/SetMsgBounds.
    if ((m_gameDataFlags & 1) != 0 && IsDirectChild(m_wndUpLine))
    {
        auto const patUp = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_wndUpLineName))->GetBounds();
        auto const patTitle = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_wndTitleName))->GetBounds();
        auto const titleB = m_wndTitle->GetBounds();

        BoundsBase<float> lineB;
        lineB.x0 = patUp.x0;
        lineB.width = m_bounds.width - patUp.x0 * 2.0f;
        lineB.height = 0.0f;
        lineB.y0 = (patUp.y0 - (patTitle.height + patTitle.y0)) + titleB.height + titleB.y0;
        m_wndUpLine->SetBounds(lineB, true);
    }
}

void MsgBox::AddMiscFignya()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        if (m_wndFrame)
        {
            if (!m_wndFrame->GetParent())
            {
                AddChild(m_wndFrame);
            }
        }
        if (m_wndBg)
        {
            if (!m_wndBg->GetParent())
            {
                AddChild(m_wndBg);
            }
        }
    }
}

void MsgBox::SetTitleBounds(PointBase<float> const& titleSz)
{
    // RVA 0x518270
    if ((m_gameDataFlags & 1) != 0 && IsDirectChild(m_wndTitle))
    {
        BoundsBase<float> titleB;
        titleB.width = titleSz.x;
        titleB.height = titleSz.y;
        titleB.x0 = (m_bounds.width - titleSz.x) * 0.5f;
        titleB.y0 = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_wndTitleName))->GetBounds().y0;
        m_wndTitle->SetBounds(titleB, true);
    }
}

int MsgBox::CloseModal(int ret)
{
    if (ret != 3)
        return ModalWnd::CloseModal(ret);
    if ((m_msgBoxFlags & 3) == 1)
    {
        ret = 1;
    }
    else if ((m_msgBoxFlags & 3) == 2)
    {
        return ModalWnd::CloseModal(2);
    }
    return ModalWnd::CloseModal(ret);
}

PointBase<float> MsgBox::CalcMsgSize() const
{
    // RVA 0x517CB0 (mirror of CalcTitleSize)
    if ((m_gameDataFlags & 1) != 0)
    {
        if (!m_msg.empty())
        {
            auto child = m_pattern->GetChildByName(m_aif.m_wndMsgName);
            auto wnd = dynamic_cast<Wnd*>(child);
            auto bounds = wnd->GetBounds();
            return GetGfxServer()->MeasureText(
                m_msg, m_wndMsg->GetDefaultFont(), m_wndMsg->GetWrapMode(), bounds.width);
        }
    }
    return {};
}

void MsgBox::ClearPattern()
{
    // RVA 0x518E70: the last MsgBox alive releases the shared pattern window.
    if (m_ref == 1)
    {
        delete m_pattern;
        m_pattern = nullptr;
    }
}

void MsgBox::RecalcLayot()
{
    // RVA 0x517350
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    for (auto child = GetFirstChild(); child;)
    {
        auto next = child->GetNextSibling();
        RemoveChild(child);
        child = next;
    }
    AddTitle();
    AddMsg();
    AddButtonsAndIdioticEmbosses();
    AddLines();
    AddMiscFignya();

    auto const titleSz = CalcTitleSize();
    auto const msgSz = CalcMsgSize();
    auto const buttonsSz = CalcSummaryButtonsSize();
    auto const idioticEmbossesSz = CalcSummaryIdioticEmbossesSize();

    float maxChildWidth = msgSz.x;
    if (msgSz.x <= titleSz.x)
    {
        maxChildWidth = titleSz.x;
    }
    if (buttonsSz.x > maxChildWidth)
    {
        maxChildWidth = buttonsSz.x;
    }
    if (idioticEmbossesSz.x > maxChildWidth)
    {
        maxChildWidth = idioticEmbossesSz.x;
    }
    CalcSelfWidthByChildrensMaxWidth(maxChildWidth);

    SetTitleBounds(titleSz);
    SetUpLineBounds();
    SetMsgBounds(msgSz);
    SetDownLineBounds();
    SetButtonsBounds(buttonsSz);
    SetIdioticEmbossesBounds(idioticEmbossesSz);
    CalcSelfHeight();

    m_bounds.x0 = (1024.0f - m_bounds.width) * 0.5f;
    m_bounds.y0 = (768.0f - m_bounds.height) * 0.5f;
    HackedExpandToScreen();
}

void MsgBox::CenterOnScreen()
{
    // RVA 0x518DE0
    m_bounds.x0 = (1024.0f - m_bounds.width) * 0.5f;
    m_bounds.y0 = (768.0f - m_bounds.height) * 0.5f;
}

void MsgBox::HackedExpandToScreen()
{
    // RVA 0x518F20: stretch the frame over the full 1024x768 screen and re-anchor
    // every other child at its screen-space position.
    if ((m_gameDataFlags & 1) != 0)
    {
        BoundsBase<float> frameB;
        frameB.x0 = 0.0f;
        frameB.y0 = 0.0f;
        frameB.width = m_bounds.width;
        frameB.height = m_bounds.height;
        m_wndFrame->SetBounds(frameB, true);

        for (auto child = GetFirstChild(); child; child = child->GetNextSibling())
        {
            if (child->IsKindOf(RT_CLASS_LOCAL(Wnd)) && child != m_wndBg)
            {
                auto wnd = dynamic_cast<Wnd*>(child);
                auto bounds = wnd->GetBounds();
                auto screenPt = ToScreen(PointBase<float>{bounds.x0, bounds.y0});
                BoundsBase<float> newB;
                newB.x0 = screenPt.x;
                newB.y0 = screenPt.y;
                newB.width = bounds.width;
                newB.height = bounds.height;
                wnd->SetBounds(newB, true);
            }
        }

        m_bounds.x0 = 0.0f;
        m_bounds.y0 = 0.0f;
        m_bounds.width = 1024.0f;
        m_bounds.height = 768.0f;
    }
}

void MsgBox::CalcSelfWidthByChildrensMaxWidth(float maxChildControlsWidth)
{
    // RVA 0x518230
    if ((m_gameDataFlags & 1) != 0)
    {
        float const frameWidth = m_wndFrame->GetFrameWidth();
        m_bounds.width = frameWidth + frameWidth + m_wndFrame->GetClientEdges()[2] + maxChildControlsWidth +
            m_wndFrame->GetClientEdges()[0];
    }
}

void MsgBox::AddButtonsAndIdioticEmbosses()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        switch (m_msgBoxFlags & 3)
        {
        case 1:
        {
            if (m_buttons[0] && !m_buttons[0]->GetParent())
            {
                AddChild(m_buttons[0]);
            }
            if (m_idioticEmbosses[0] && !m_idioticEmbosses[0]->GetParent())
            {
                AddChild(m_idioticEmbosses[0]);
            }
            m_buttons[0]->SetText(GetStation()->InitializeStringUsingIds("^ok^"));
            break;
        }
        case 2:
        {
            if (m_buttons[0] && !m_buttons[0]->GetParent())
            {
                AddChild(m_buttons[0]);
            }
            if (m_buttons[1] && !m_buttons[1]->GetParent())
            {
                AddChild(m_buttons[1]);
            }
            if (m_idioticEmbosses[0] && !m_idioticEmbosses[0]->GetParent())
            {
                AddChild(m_idioticEmbosses[0]);
            }
            if (m_idioticEmbosses[1] && !m_idioticEmbosses[1]->GetParent())
            {
                AddChild(m_idioticEmbosses[1]);
            }
            m_buttons[0]->SetText(GetStation()->InitializeStringUsingIds("^yes^"));
            break;
        }
        case 3:
        {
            for (int i = 0; i < 3; ++i)
            {
                if (m_buttons[i] && !m_buttons[i]->GetParent())
                {
                    AddChild(m_buttons[i]);
                }
            }
            for (int i = 0; i < 3; ++i)
            {
                if (m_idioticEmbosses[i] && !m_idioticEmbosses[i]->GetParent())
                {
                    AddChild(m_idioticEmbosses[i]);
                }
            }
            m_buttons[0]->SetText(GetStation()->InitializeStringUsingIds("^yes^"));
            break;
        }
        default:
            return;
        }
    }
}

void MsgBox::CalcSelfHeight()
{
    // RVA 0x518C80: height = bottom of the lowest non-frame/bg child plus the
    // margin the pattern leaves below that same child.
    if ((m_gameDataFlags & 1) != 0)
    {
        float maxBottom = 0.0f;
        CStr childName;
        for (auto child = GetFirstChild(); child; child = child->GetNextSibling())
        {
            if (child->IsKindOf(RT_CLASS_LOCAL(Wnd)) && child != m_wndFrame && child != m_wndBg)
            {
                auto bounds = dynamic_cast<Wnd*>(child)->GetBounds();
                if (bounds.height + bounds.y0 > maxBottom)
                {
                    maxBottom = bounds.height + bounds.y0;
                    childName = child->GetName();
                }
            }
        }
        float add = 0.0f;
        if (!childName.empty())
        {
            auto wnd = dynamic_cast<Wnd*>(m_pattern->GetChildByName(childName));
            auto patternBounds = m_pattern->GetBounds();
            auto bounds = wnd->GetBounds();
            add = patternBounds.height - (bounds.height + bounds.y0);
        }
        m_bounds.height = add + maxBottom;
    }
}

int MsgBox::LoadPattern()
{
    // RVA 0x518E30
    if (m_pattern)
    {
        return 1;
    }
    m_pattern = dynamic_cast<m3d::ui::Wnd*>(m3d::g_Kernel->New("Wnd"));
    if (m_pattern)
    {
        return help::CloneWndWithChildren(this, m_pattern);
    }
    return 0;
}

void MsgBox::SetButtonsBounds(PointBase<float> const& buttonsSz)
{
    // RVA 0x518760. NOTE: the shipped body reads several uninitialized stack slots
    // for the button pitch (a genuine bug in the original). Reconstructed from the
    // parts it *does* compute cleanly: a centered start X
    // ((m_bounds.width - buttonsSz.x) * 0.5), the pattern gap between button_0 and
    // button_1, and the per-button vertical offset from the down line - stepping
    // by (buttonWidth + gap), consistent with CalcSummaryButtonsSize.
    if ((m_gameDataFlags & 1) != 0 && (m_msgBoxFlags & 3) != 0)
    {
        auto patBtn1 = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_buttonName + CStr(1)))->GetBounds();
        auto patBtn0 = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_buttonName + CStr(0)))->GetBounds();
        float const gap = patBtn1.x0 - (patBtn0.x0 + patBtn0.width);

        auto downLineBounds = m_wndDownLine->GetBounds();
        auto patDownLine = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_aif.m_wndDownLineName))->GetBounds();

        float x = (m_bounds.width - buttonsSz.x) * 0.5f;
        for (int i = 0; i < 3; ++i)
        {
            if (IsDirectChild(m_buttons[i]))
            {
                auto btnBounds = m_buttons[i]->GetBounds();
                auto patBtn = dynamic_cast<Wnd*>(m_pattern->GetChildByName(m_buttons[i]->GetName()))->GetBounds();
                float const deltaY = patBtn.y0 - (patDownLine.height + patDownLine.y0);

                btnBounds.x0 = x;
                btnBounds.y0 = downLineBounds.y0 + downLineBounds.height + deltaY;
                m_buttons[i]->SetBounds(btnBounds, true);

                x += btnBounds.width + gap;
            }
        }
    }
}

int MsgBox::GameDataSetup()
{
    using namespace m3d::ui;
    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto title = GetChildByName(m_aif.m_wndTitleName);
        if (title && title->IsKindOf(RT_CLASS_LOCAL(Wnd)))
        {
            m_wndTitle = dynamic_cast<Wnd*>(title);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndTitleName + " is not found or incorrect type");
            res = 0;
        }

        auto msg = GetChildByName(m_aif.m_wndMsgName);
        if (msg && msg->IsKindOf(RT_CLASS_LOCAL(Wnd)))
        {
            m_wndMsg = dynamic_cast<Wnd*>(msg);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndMsgName + " is not found or incorrect type");
            res = 0;
        }

        auto lineUp = GetChildByName(m_aif.m_wndUpLineName);
        if (lineUp && lineUp->IsKindOf(RT_CLASS_LOCAL(LineWnd)))
        {
            m_wndUpLine = dynamic_cast<LineWnd*>(lineUp);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndUpLineName + " is not found or incorrect type");
            res = 0;
        }

        auto lineDown = GetChildByName(m_aif.m_wndDownLineName);
        if (lineDown && lineDown->IsKindOf(RT_CLASS_LOCAL(LineWnd)))
        {
            m_wndDownLine = dynamic_cast<LineWnd*>(lineDown);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndDownLineName + " is not found or incorrect type");
            res = 0;
        }

        auto frame = GetChildByName(m_aif.m_wndFrameName);
        if (frame && frame->IsKindOf(RT_CLASS_LOCAL(Wnd)))
        {
            m_wndFrame = dynamic_cast<Wnd*>(frame);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndFrameName + " is not found or incorrect type");
            res = 0;
        }

        auto bg = GetChildByName(m_aif.m_wndBgName);
        if (bg && bg->IsKindOf(RT_CLASS_LOCAL(Wnd)))
        {
            m_wndBg = dynamic_cast<Wnd*>(bg);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndBgName + " is not found or incorrect type");
            res = 0;
        }

        for (int i = 0; i < 3; ++i)
        {
            auto name = m_aif.m_buttonName + CStr(i);
            auto button = GetChildByName(name);
            if (button && button->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
            {
                m_buttons[i] = dynamic_cast<ButtonWnd*>(button);
            }
            else
            {
                M3D_LOG_INFO("Get control error: control " + name + " is not found or incorrect type");
                res = 0;
            }

            auto embossName = m_aif.m_idioticEmbossName + CStr(i);
            auto emboss = GetChildByName(embossName);
            if (emboss && emboss->IsKindOf(RT_CLASS_LOCAL(Wnd)))
            {
                m_idioticEmbosses[i] = dynamic_cast<Wnd*>(emboss);
            }
            else
            {
                M3D_LOG_INFO("Get control error: control " + embossName + " is not found or incorrect type");
                res = 0;
            }
        }
        auto res2 = 0;
        if (m_pattern)
        {
            res2 = 1;
        }
        else
        {
            auto wnd = m3d::g_Kernel->New("Wnd");
            m_pattern = dynamic_cast<Wnd*>(wnd);
            if (wnd)
            {
                res2 = help::CloneWndWithChildren(this, m_pattern);
            }
        }
        if ((res & res2) != 0)
        {
            m_gameDataFlags |= 1u;
            Clear();
        }
    }
    // RVA 0x516630: the success/failure check runs unconditionally, not only when
    // the (m_gameDataFlags & 2) == 0 branch was taken.
    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("MsgBox: error - fail to init because of a bad resource");
    return 0;
}

int MsgBox::CreateFromPattern()
{
    // RVA 0x518EE0: the first half of CreateMsgBox - clone the pattern tree and
    // resolve the child controls.
    if (Valid() || (m_gameDataFlags & 1) != 0 || !m_pattern || !help::CloneWndWithChildren(m_pattern, this))
    {
        return 0;
    }
    return GameDataSetup();
}
