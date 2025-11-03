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
    if (Valid()
        || (m_gameDataFlags & 1) != 0
        || !m_pattern
        || !help::CloneWndWithChildren(m_pattern, this)
        || !GameDataSetup())
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
    RETRUXX_NOT_IMPLEMENTED;
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

void MsgBox::SetIdioticEmbossesBounds(PointBase<float> const&)
{
    // TODO: check this!!!!!!
    if ((m_gameDataFlags & 1) != 0 && (m_msgBoxFlags & 3) != 0)
    {
        CStr buttonName1 = m_aif.m_buttonName + CStr(1);
        m3d::ui::Wnd* patternButton1 = static_cast<m3d::ui::Wnd*>(m_pattern->GetChildByName(buttonName1));
        auto button1Bounds = patternButton1->GetBounds();

        //CStr buttonName0 = m_aif.m_buttonName + CStr(0);
        //m3d::ui::Wnd* patternButton0 = static_cast<m3d::ui::Wnd*>(m_pattern->GetChildByName(buttonName0));
        //auto button0Bounds = patternButton0->GetBounds();

        auto downLineBounds = m_wndDownLine->GetBounds();
        for (int i = 0; i < 3; ++i)
        {
            if (IsDirectChild(m_buttons[i]))
            {
                auto btnBounds = m_buttons[i]->GetBounds();
                btnBounds.x0 = button1Bounds.x0;
                button1Bounds.x0 += downLineBounds.x0;

                m3d::ui::Wnd* buttonWnd = static_cast<m3d::ui::Wnd*>(m_pattern->GetChildByName(m_buttons[i]->GetName()));
                m3d::ui::Wnd* downLineNameWnd = static_cast<m3d::ui::Wnd*>(m_pattern->GetChildByName(m_aif.m_wndDownLineName));

                auto actualBtnBounds = buttonWnd->GetBounds();
                auto downLineNameBounds = downLineNameWnd->GetBounds();

                auto y0 = actualBtnBounds.y0;
                auto v22 = y0 - (float)(downLineNameBounds.height + downLineNameBounds.y0);

                btnBounds.y0 = (downLineBounds.height + btnBounds.y0) + v22;

                m_buttons[i]->SetBounds(btnBounds, true);

            }
        }
    }
}

MsgBox::MsgBox(MsgBox const&)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    //TODO: check this and refactor!!!!!!!!!
    m3d::ui::Wnd* v3; // ecx
    m3d::ui::Wnd* v4; // ebx
    m3d::Object* v5; // edi
    m3d::Object* v6; // ebx
    BoundsBase<float> v7; // eax
    float v8; // xmm0_4
    m3d::Object* v9; // eax
    m3d::ui::Wnd* v10; // ecx
    float v11; // [esp+Ch] [ebp-44h]
    BoundsBase<float> msgB; // [esp+10h] [ebp-40h] BYREF
    BoundsBase<float> lineUpB; // [esp+20h] [ebp-30h] BYREF
    char v14[16]; // [esp+30h] [ebp-20h] BYREF
    char v15[16]; // [esp+40h] [ebp-10h] BYREF

    if ((this->m_gameDataFlags & 1) != 0 && IsDirectChild(this->m_wndMsg))
    {
        v3 = this->m_wndTitle;
        msgB.y0 = 0.0;
        msgB.width = msgSz.x;
        msgB.height = msgSz.y;
        msgB.x0 = (this->m_bounds.width - msgSz.x) * 0.5;
        if (IsDirectChild(v3))
        {
            IsDirectChild(this->m_wndUpLine);
            lineUpB = this->m_wndUpLine->GetBounds();
            v4 = m_pattern;
            v5 = m_pattern->GetChildByName(this->m_aif.m_wndMsgName);
            v6 = v4->GetChildByName(this->m_aif.m_wndUpLineName);

            v11 = dynamic_cast<Wnd*>(v5)->GetBounds().y0;
            v7 = dynamic_cast<Wnd*>(v6)->GetBounds();
            v8 = ((v11 - (v7.height + v7.y0)) + lineUpB.height) + lineUpB.y0;
        }
        else
        {
            IsDirectChild(this->m_wndUpLine);
            v9 = m_pattern->GetChildByName(this->m_aif.m_wndTitleName);
            v8 = dynamic_cast<Wnd*>(v9)->GetBounds().y0;
        }
        v10 = this->m_wndMsg;
        msgB.y0 = v8;
        v10->SetBounds(msgB, true);
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
    //TODO: check this and refactor!!!
    int v2; // eax
    bool v3; // zf
    int v4; // eax
    bool v5; // dl
    bool v6; // bl
    bool v7; // cl
    m3d::ui::LineWnd* v8; // eax
    m3d::ui::LineWnd* v9; // eax
    bool bHasTitle; // [esp+4h] [ebp-2h]
    bool bHasButtons; // [esp+5h] [ebp-1h]

    if ((m_gameDataFlags & 1) == 0)
        return;
    v2 = m_title.length();
    v3 = v2 == 0;
    v5 = !v3;
    bHasTitle = m_title.length() != 0;
    v4 = m_msg.length();
    v6 = v4 != 0;
    v7 = (m_msgBoxFlags & 3) != 0;
    bHasButtons = v7;
    if (!v3)
    {
        if (!v4)
            goto LABEL_13;
        v8 = m_wndUpLine;
        if (v8 && !v8->GetParent())
        {
            AddChild(m_wndUpLine);
            v5 = bHasTitle;
            v7 = bHasButtons;
        }
    }
    if (!v6 || !v7)
    {
    LABEL_13:
        if (!v5 || !v7)
            return;
    }
    v9 = m_wndDownLine;
    if (v9)
    {
        if (!v9->GetParent())
            AddChild(m_wndDownLine);
    }
}

void MsgBox::SetDownLineBounds()
{
    // TODO: generated code
    // Check if we need to update the down line bounds and if the down line is a direct child
    if ((m_gameDataFlags & 1) != 0 && m3d::Object::IsDirectChild(m_wndDownLine))
    {
        // Get the pattern reference for the down line
        m3d::ui::Wnd* patternDownLine = static_cast<m3d::ui::Wnd*>(
            m_pattern->GetChildByName(m_aif.m_wndDownLineName));

        // Get the pattern's down line bounds to use as reference
        BoundsBase<float> patternBounds = patternDownLine->GetBounds();

        // Calculate the width for the down line (full width minus margins)
        float downLineWidth = m_bounds.width - (patternBounds.x0 * 2.0f);

        // Get the current down line height from the pattern
        float downLineHeight = m_wndDownLine->GetBounds().height;

        // Initialize line bounds structure
        BoundsBase<float> lineBounds;
        lineBounds.x0 = patternBounds.x0;  // Left margin
        lineBounds.y0 = 0.0f;              // Will be calculated based on content
        lineBounds.width = downLineWidth;   // Calculated width
        lineBounds.height = downLineHeight; // Height from pattern

        // Calculate vertical position based on whether message window is present
        if (m3d::Object::IsDirectChild(m_wndMsg))
        {
            // Message window is present - position down line below it
            BoundsBase<float> msgBounds = m_wndMsg->GetBounds();

            // Get pattern references for positioning
            m3d::ui::Wnd* patternMsg = static_cast<m3d::ui::Wnd*>(
                m_pattern->GetChildByName(m_aif.m_wndMsgName));

            // Get pattern positions to calculate proper spacing
            BoundsBase<float> patternMsgBounds = patternMsg->GetBounds();

            BoundsBase<float> patternDownLinePos = patternDownLine->GetBounds();

            // Calculate Y position: below message with proper spacing from pattern
            lineBounds.y0 = patternDownLinePos.y0 - (patternMsgBounds.height + patternMsgBounds.y0)
                + msgBounds.height + patternMsgBounds.height;
        }
        else
        {
            // No message window - position down line below title and up line
            //m3d::Object::IsDirectChild(m_wndTitle);
            //m3d::Object::IsDirectChild(m_wndUpLine);

            // Get title bounds
            BoundsBase<float> titleBounds = m_wndTitle->GetBounds();

            // Get pattern references for positioning
            m3d::ui::Wnd* patternUpLine = static_cast<m3d::ui::Wnd*>(
                m_pattern->GetChildByName(m_aif.m_wndUpLineName));

            m3d::ui::Wnd* patternTitle = static_cast<m3d::ui::Wnd*>(
                m_pattern->GetChildByName(m_aif.m_wndTitleName));

            // Get pattern positions to calculate proper spacing
            BoundsBase<float> patternUpLinePos = patternUpLine->GetBounds();

            BoundsBase<float> patternTitleBounds = patternTitle->GetBounds();

            // Calculate Y position: below title/up-line with proper spacing from pattern
            lineBounds.y0 = patternUpLinePos.y0 - (patternTitleBounds.height + patternTitleBounds.y0)
                + titleBounds.height + patternTitleBounds.height;
        }

        // Apply the calculated bounds to the down line
        m_wndDownLine->SetBounds(lineBounds, true);
    }
}

PointBase<float> MsgBox::CalcSummaryButtonsSize() const
{
    if ((m_gameDataFlags & 1) != 0)
    {
        float buttonsMaxSzX = 0.0;
        float buttonsMaxSzY = 0.0;
        for (int i = 0; i<3;++i)
        {
	        if (IsDirectChild(m_buttons[i]))
	        {
                auto bounds = m_buttons[i]->GetBounds();
                if (bounds.height > buttonsMaxSzY)
                {
                    buttonsMaxSzY = bounds.height;
                }
	        }
        }
        if (m_msgBoxFlags & 3 > 1)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
        PointBase<float> res;
        res.x = buttonsMaxSzX + buttonsMaxSzY;
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
            return GetGfxServer()->MeasureText(m_title, m_wndTitle->GetDefaultFont(), m_wndTitle->GetWrapMode(), bounds.width);
        }
    }
    return {};
}

PointBase<float> MsgBox::CalcSummaryIdioticEmbossesSize() const
{
    if ((m_gameDataFlags & 1) != 0)
    {
        float embossesMaxSzX = 0.0;
        float embossesMaxSzY = 0.0;
        for (int i = 0; i < 3; ++i)
        {
            if (IsDirectChild(m_idioticEmbosses[i]))
            {
                auto bounds = m_idioticEmbosses[i]->GetBounds();
                if (bounds.height > embossesMaxSzY)
                {
                    embossesMaxSzY = bounds.height;
                }
            }
        }
        if (m_msgBoxFlags & 3 > 1)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
        PointBase<float> res;
        res.x = embossesMaxSzX + embossesMaxSzY;
        res.y = embossesMaxSzY;
        return res;
    }
    return {};
}

void MsgBox::SetUpLineBounds()
{
    if ((m_gameDataFlags & 1) != 0 && IsDirectChild(m_wndUpLine))
    {
        //TODO: check this!!!!!!!!!!!!!!!
        auto childUpLine = m_pattern->GetChildByName(m_aif.m_wndUpLineName);
        auto wndUpLine = dynamic_cast<Wnd*>(childUpLine);
        auto boundsUpLine = wndUpLine->GetBounds();
        BoundsBase<float> lineB;
        lineB.y0 = boundsUpLine.x0;
        lineB.height = 0.0;
        lineB.width = lineB.y0;

        auto childTitle = m_pattern->GetChildByName(m_aif.m_wndTitleName);
        auto wndUpTitle = dynamic_cast<Wnd*>(childUpLine);
        auto boundsTitle= wndUpLine->GetBounds();
        auto spaceX = boundsUpLine.y0;
        lineB.y0 = ((spaceX - (boundsTitle.height+ boundsTitle.y0)) + boundsTitle.height) + boundsTitle.y0;
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
    if ((m_gameDataFlags & 1) != 0 &&IsDirectChild(this->m_wndTitle))
    {
        BoundsBase<float> titleB;
        titleB.height = 0.0;
        titleB.width = titleSz.x;
        titleB.x0 = (m_bounds.width - titleSz.x) * 0.5;
        auto child = m_pattern->GetChildByName(m_aif.m_wndTitleName);
        auto wnd = dynamic_cast<Wnd*>(child);
        auto bounds = wnd->GetBounds();
        titleB.y0 = bounds.y0;
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
    if ((m_gameDataFlags & 1) != 0)
    {
        if (!m_msg.empty())
        {
            auto child = m_pattern->GetChildByName(m_aif.m_wndMsgName);
            auto wnd = dynamic_cast<Wnd*>(child);
            auto bounds = wnd->GetBounds();
            return GetGfxServer()->MeasureText(m_msg, m_wndMsg->GetDefaultFont(), m_wndMsg->GetWrapMode(), bounds.width);
        }
    }
    return {};
}

void MsgBox::ClearPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MsgBox::RecalcLayot()
{
    if ((m_gameDataFlags & 1) != 0)
    {
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
        auto titleSz = CalcTitleSize();
        auto msgSz = CalcMsgSize();
        auto buttonsSz = CalcSummaryButtonsSize();
        auto idioticEmbossesSz = CalcSummaryIdioticEmbossesSize();
        auto maxSizeX = msgSz.x;
        if (msgSz.x <= titleSz.x)
            maxSizeX = titleSz.x;
        if (buttonsSz.x > maxSizeX)
            maxSizeX = buttonsSz.x;
        if (idioticEmbossesSz.x > maxSizeX)
            maxSizeX = idioticEmbossesSz.x;
        CalcSelfWidthByChildrensMaxWidth(maxSizeX);
        SetTitleBounds(titleSz);
        SetUpLineBounds();
        SetMsgBounds(msgSz);
        SetDownLineBounds();
        SetButtonsBounds(buttonsSz);
        SetIdioticEmbossesBounds(idioticEmbossesSz);
        CalcSelfHeight();
        m_bounds.x0 = (1024.0 - m_bounds.width) * 0.5;
        m_bounds.y0 = (768.0 - m_bounds.height) * 0.5;
        HackedExpandToScreen();
    }
}

void MsgBox::CenterOnScreen()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MsgBox::HackedExpandToScreen()
{
    //TODO: check this!!
    if ((m_gameDataFlags & 1) != 0)
    {
        BoundsBase<float> result;
        result.x0 = 0;
        result.y0 = 0;
        result.width = m_bounds.width;
        result.height = m_bounds.height;
        m_wndFrame->SetBounds(result, true);
        for (auto child = GetFirstChild(); child; child = child->GetNextSibling())
        {
	        if (child->IsKindOf(RT_CLASS_LOCAL(Wnd)))
	        {
		        if (child != m_wndBg)
		        {
                    auto wnd = dynamic_cast<Wnd*>(child);
                    auto bounds = wnd->GetBounds();
                    auto res = ToScreen(result);
                    res.width = (bounds.width + res.x0) - res.x0;
                    res.height = (bounds.height + res.y0) - res.y0;
                    wnd->SetBounds(res, true);
		        }
	        }
        }
        m_bounds.x0 = 0.0;
        m_bounds.y0 = 0.0;
        m_bounds.width = 1024.0;
        m_bounds.height = 768.0;
    }
}

void MsgBox::CalcSelfWidthByChildrensMaxWidth(float maxChildControlsWidth)
{
    //TODO: check this
    if ((m_gameDataFlags & 1) != 0)
    {
        auto frameWidth = m_wndFrame->GetFrameWidth();
        m_bounds.width = frameWidth
            + frameWidth
            + m_wndFrame->GetClientEdges()[2]
            + maxChildControlsWidth
            + m_wndFrame->GetClientEdges()[0];
    }
}

void MsgBox::AddButtonsAndIdioticEmbosses()
{
    if ((m_gameDataFlags & 1) != 0)
    {
	    switch (m_msgBoxFlags & 3)
	    {
	    case 1: RETRUXX_NOT_IMPLEMENTED;
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
	    case 3: RETRUXX_NOT_IMPLEMENTED;
        default: return;
	    }
    }
}

void MsgBox::CalcSelfHeight()
{
    //TODO: check this!!!!
    if ((m_gameDataFlags & 1) != 0)
    {
        float max = 0.0;
        float add = 0.0;
        CStr childName;
	    for (auto child= GetFirstChild(); child; child = child->GetNextSibling())
	    {
		    if (child->IsKindOf(RT_CLASS_LOCAL(Wnd)))
		    {
			    if (child != m_wndFrame && child!= m_wndBg)
			    {
                    auto wnd = dynamic_cast<Wnd*>(child);
                    auto bounds = wnd->GetBounds();
                    if ((bounds.height + bounds.y0)> max)
                    {
                        max = bounds.height + bounds.y0;
                        childName = wnd->GetName();
                    }
			    }
		    }
	    }
        if (!childName.empty())
        {
            auto wnd = dynamic_cast<Wnd*>(m_pattern->GetChildByName(childName));
            auto patterBounds = m_pattern->GetBounds();
            auto bounds = wnd->GetBounds();
            add = patterBounds.height - (bounds.height + bounds.y0);
        }
        m_bounds.height = max + add;
    }
}

int MsgBox::LoadPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MsgBox::SetButtonsBounds(PointBase<float> const& buttonsSz)
{
    // TODO: check this!!!!!!
    if ((m_gameDataFlags & 1) != 0 && (m_msgBoxFlags & 3) != 0)
    {
        CStr buttonName1 = m_aif.m_buttonName + CStr(1);
        m3d::ui::Wnd* patternButton1 = static_cast<m3d::ui::Wnd*>(m_pattern->GetChildByName(buttonName1));
        auto button1Bounds = patternButton1->GetBounds();

        CStr buttonName0 = m_aif.m_buttonName + CStr(0);
        m3d::ui::Wnd* patternButton0 = static_cast<m3d::ui::Wnd*>(m_pattern->GetChildByName(buttonName0));
        auto button0Bounds = patternButton0->GetBounds();

        auto downLineBounds = m_wndDownLine->GetBounds();
        for (int i = 0; i < 3; ++i)
        {
            if (IsDirectChild(m_buttons[i]))
            {
                auto btnBounds = m_buttons[i]->GetBounds();
                btnBounds.x0 = button1Bounds.x0;
                button1Bounds.x0 += downLineBounds.x0;

                m3d::ui::Wnd* buttonWnd = static_cast<m3d::ui::Wnd*>(m_pattern->GetChildByName(m_buttons[i]->GetName()));
                m3d::ui::Wnd* downLineNameWnd = static_cast<m3d::ui::Wnd*>(m_pattern->GetChildByName(m_aif.m_wndDownLineName));

                auto actualBtnBounds = buttonWnd->GetBounds();
                auto downLineNameBounds = downLineNameWnd->GetBounds();

                auto y0 = actualBtnBounds.y0;
                auto v22 = y0 - (float)(downLineNameBounds.height + downLineNameBounds.y0);

                btnBounds.y0 = (downLineBounds.height + btnBounds.y0) + v22;

                m_buttons[i]->SetBounds(btnBounds, true);

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
        if ((m_gameDataFlags & 1) != 0)
            return 1;
        M3D_LOG_INFO("MsgBox: error - fail to init because of a bad resource");
        return 0;
    }
}

int MsgBox::CreateFromPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}
