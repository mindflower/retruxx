#include <core/ini.h>
#include <ui/button.h>
#include <ui/comboboxwnd.h>
#include <ui/listbox.h>

#include "m3dapp.h"

namespace m3d
{
    namespace ui
    {
        RT_CLASS_DEFINE(ComboBoxWnd);

        ComboBoxWnd::AuxInfo::AuxInfo()
        {

        }

        int ComboBoxWnd::GetCurSel() const
        {
            throw std::logic_error("Not implemented");
        }

        int ComboBoxWnd::GetItemData(int) const
        {
            throw std::logic_error("Not implemented");
        }

        BoundsBase<float> ComboBoxWnd::GetListBounds() const
        {
            throw std::logic_error("Not implemented");
        }

        void ComboBoxWnd::SetCurSel(int)
        {
            throw std::logic_error("Not implemented");
        }

        void ComboBoxWnd::SetItemData(int, int)
        {
            throw std::logic_error("Not implemented");
        }

        unsigned ComboBoxWnd::GetComboStyle() const
        {
            throw std::logic_error("Not implemented");
        }

        void ComboBoxWnd::Close()
        {
            throw std::logic_error("Not implemented");
        }

        BoundsBase<float> ComboBoxWnd::GetFullBounds() const
        {
            throw std::logic_error("Not implemented");
        }

        void ComboBoxWnd::Open()
        {
            throw std::logic_error("Not implemented");
        }

        int ComboBoxWnd::GetCount() const
        {
            throw std::logic_error("Not implemented");
        }

        float ComboBoxWnd::GetSelTextFixedHeight() const
        {
            throw std::logic_error("Not implemented");
        }

        Class* ComboBoxWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(ComboBoxWnd);
        }

        void ComboBoxWnd::SetComboStyle(unsigned comboStyle)
        {
            m_comboStyle = comboStyle;
            RecalcLayot();
        }

        ComboBoxWnd::~ComboBoxWnd()
        {
            throw std::logic_error("Not implemented");
        }

        void ComboBoxWnd::SetBounds(BoundsBase<float> const& rect, bool bUpdateBaseOrigin)
        {
            m_bounds.x0 = rect.x0;
            m_bounds.y0 = rect.y0;
            m_bounds.width = rect.width;
            RecalcLayot();
            if (bUpdateBaseOrigin)
            {
                m_baseOrigin.x = m_bounds.x0;
                m_baseOrigin.y = m_bounds.y0;
            }
        }

        void ComboBoxWnd::SetSelTextFixedHeight(float height)
        {
            m_selTextFixedH = height;
            //TODO: check this
            if ((m_comboStyle & 4) != 0)
            {
                RecalcLayot();
            }
        }

        int ComboBoxWnd::Create(CStr const& caption, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            auto res = Create(style, rc, id, 0, 0.0, 0.0);
            if (res)
            {
                SetText(caption);
            }
            return res;
        }

        int ComboBoxWnd::Create(unsigned style, BoundsBase<float> const& b, int id, unsigned comboStyle, float listMaxH, float selTextFixedH)
        {
            auto rect = b;
            m_btnToggle = dynamic_cast<ButtonWnd*>(g_Kernel->New("ButtonWnd"));
            if (!m_btnToggle)
            {
                return 0;
            }
            BoundsBase<float> rc;
            rc.x0 = 0.0;
            rc.y0 = 0.0;
            rc.width = 0.0;
            rc.height = 0.0;
            //TODO: check style
            if (m_btnToggle->Create({}, 440220, rc, 5) == 0)
            {
                return 0;
            }
            m_btnToggle->SetPersistance(false);

            m_wndSelText = dynamic_cast<Wnd*>(g_Kernel->New("Wnd"));
            if (!m_wndSelText)
            {
                return 0;
            }
            //TODO: check style
            if (m_wndSelText->Create({}, 4196896, rc, 0) == 0)
            {
                return 0;
            }
            m_wndSelText->SetPersistance(false);

            m_wndStringList = dynamic_cast<StringsListBoxWnd*>(g_Kernel->New("StringsListBoxWnd"));
            if (!m_wndStringList)
            {
                return 0;
            }
            //TODO: check style
            if (m_wndStringList->Create({}, 4456960, rc, 6) == 0)
            {
                return 0;
            }
            m_wndStringList->SetPersistance(false);
            m_wndStringList->SetDrawFlags(2);

            if (!style)
            {
                style = 262720;
            }
            if (Wnd::Create({}, style, rc, id) == 0)
            {
                return 0;
            }
            m_comboStyle = comboStyle;
            m_selTextFixedH = selTextFixedH;
            SetPane(m_paneName);
            SetScrollPane(m_scrollPaneName);
            UpdateToggleButtonPane();
            SetDefaultFont(m_defFont);
            SetColor(m_curClr);
            SetTextColor(m_textColor);
            SetTextColorDisabled(m_textColorDisabled);
            SetState(m_state, true);
            AddChild(m_btnToggle);
            AddChild(m_wndStringList);
            AddChild(m_wndSelText);
            m_bounds.width = rect.width;
            m_bounds.x0 = rect.x0;
            m_bounds.y0 = rect.y0;
            SetBaseOrigin({ rect.x0, rect.y0 });
            SetListMaxHeight(listMaxH);
            return 1;
        }

        void ComboBoxWnd::SetToggleButtonPane(CStr const&, CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        CStr ComboBoxWnd::GetItem(int) const
        {
            throw std::logic_error("Not implemented");
        }

        void ComboBoxWnd::SetListMaxHeight(float listMaxH)
        {
            m_maxListH = listMaxH;
            RecalcLayot();
        }

        void ComboBoxWnd::SetColor(unsigned Color)
        {
            m_curClr = Color;
            if (Valid())
            {
                m_wndStringList->SetColor(Color);
                m_wndSelText->SetColor(Color);
            }
        }

        int ComboBoxWnd::SetText(CStr const& text)
        {
            if (m_wndSelText)
            {
                return m_wndSelText->SetText(text);
            }
            return 0;
        }

        Object* ComboBoxWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        int ComboBoxWnd::AddItem(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        int ComboBoxWnd::ReadFromXmlNode(cmn::XmlFile* xmlFile, cmn::XmlNode* xmlNode)
        {
            auto res = Wnd::ReadFromXmlNode(xmlFile, xmlNode);
            if (!res)
            {
                return res;
            }
            auto state = STATE_CLOSE;
            SafeEnumAttrib(state, xmlNode, "state");
            SetState(state, true);

            auto comboStyle = m_comboStyle;
            SafeUintAttrib(comboStyle, xmlNode, "comboStyle");
            SetComboStyle(comboStyle);

            auto maxListH = m_maxListH;
            SafeFloatAttrib(maxListH, xmlNode, "maxListHeight");
            SetListMaxHeight(maxListH);

            auto selTextFixedHeight = m_selTextFixedH;
            SafeFloatAttrib(selTextFixedHeight, xmlNode, "selTextFixedHeight");
            SetSelTextFixedHeight(selTextFixedHeight);
            UpdateToggleButtonPane();
            return 1;
        }

        void ComboBoxWnd::SetPane(CStr const& paneName)
        {
            if (Valid())
            {
                if (!paneName.empty())
                {
                    m_paneName = paneName;
                }
                m_wndSelText->SetPane(paneName);
                m_wndStringList->SetPane(paneName);
                RecalcLayot();
            }
        }

        CStr ComboBoxWnd::GetText() const
        {
            throw std::logic_error("Not implemented");
        }

        void ComboBoxWnd::SetItem(int, CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        float ComboBoxWnd::GetMaxListHeight() const
        {
            throw std::logic_error("Not implemented");
        }

        BoundsBase<float> ComboBoxWnd::GetSelTextBounds() const
        {
            if (Valid())
            {
                return m_wndSelText->GetBounds();
            }
            //TODO: check this
            return {};
        }

        BoundsBase<float> ComboBoxWnd::GetFullMaxBounds() const
        {
            throw std::logic_error("Not implemented");
        }

        void ComboBoxWnd::SetTextColor(unsigned color)
        {
            Wnd::SetTextColor(color);
            if (Valid())
            {
                m_wndStringList->SetTextColor(color);
                m_wndSelText->SetTextColor(color);
            }
        }

        unsigned ComboBoxWnd::GetDrawFlags() const
        {
            throw std::logic_error("Not implemented");
        }

        int ComboBoxWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        void ComboBoxWnd::SetTextColorDisabled(unsigned color)
        {
            Wnd::SetTextColorDisabled(color);
            if (Valid())
            {
                m_wndStringList->SetTextColorDisabled(color);
                m_wndSelText->SetTextColorDisabled(color);
            }
        }

        void ComboBoxWnd::SetDefaultFont(CStr const&, float, FontType, FontParams)
        {
            throw std::logic_error("Not implemented");
        }

        void ComboBoxWnd::SetDefaultFont(int uiFont)
        {
            Wnd::SetDefaultFont(uiFont);
            if (Valid())
            {
                m_wndStringList->SetDefaultFont(uiFont);
                m_wndSelText->SetDefaultFont(uiFont);
                RecalcLayot();
            }
        }

        int ComboBoxWnd::RemoveItem(int)
        {
            throw std::logic_error("Not implemented");
        }

        BoundsBase<float> ComboBoxWnd::GetListMaxBounds() const
        {
            throw std::logic_error("Not implemented");
        }

        Object* ComboBoxWnd::CreateObject()
        {
            return new ComboBoxWnd;
        }

        void ComboBoxWnd::SetDrawFlags(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int ComboBoxWnd::InsertItem(CStr const&, int)
        {
            throw std::logic_error("Not implemented");
        }

        void ComboBoxWnd::SetScrollPane(CStr const& scrollPaneName)
        {
            if (Valid())
            {
                m_scrollPaneName = scrollPaneName;
                m_wndStringList->SetScrollPane(scrollPaneName);
            }
        }

        Class* ComboBoxWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        int ComboBoxWnd::ItemFromPoint(PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        int ComboBoxWnd::RemoveAllItems()
        {
            throw std::logic_error("Not implemented");
        }

        bool ComboBoxWnd::IsOpen() const
        {
            throw std::logic_error("Not implemented");
        }

        ComboBoxWnd::ComboBoxWnd(ComboBoxWnd const&)
        {
            throw std::logic_error("Not implemented");
        }

        ComboBoxWnd::ComboBoxWnd()
        {
            m_defFont = 1;
        }

        void ComboBoxWnd::UpdateToggleButtonPane()
        {
            if (m_btnToggle)
            {
                //TODO: check this
                m_btnToggle->SetPane(m_state != STATE_OPEN ? m_toggleButtonOpenPaneName : m_toggleButtonClosePaneName);
            }
        }

        void ComboBoxWnd::SetState(State state, bool bForceUpdate)
        {
            m_state = state;
            if (m_state != state || bForceUpdate)
            {
                RecalcListBounds();
                RecalcSelfBounds();
                UpdateToggleButtonPane();
                //TODO: check this!!!!
                Application::g_pApp->EnqueueMessage((m_state != STATE_OPEN) + 47, reinterpret_cast<int>(this), 0, 0, 0, {}, {});
            }
        }

        void ComboBoxWnd::RecalcLayot()
        {
            //TODO: check this
            if (Valid())
            {
                int paneW = 0;
                auto pane = GetGfxServer()->GetPane(m_paneName);
                if (pane && pane->m_frame[0])
                {
                    paneW = pane->m_frame[0]->m_barUsedWidth;
                }
                auto selTextH = this->m_selTextFixedH;
                if ((m_comboStyle & 4) == 0)
                {
                    selTextH = m_aif.m_space * 2.0 + GetGfxServer()->MeasureText("Ap", m_defFont, TW_NOWRAP, 100.0).y + (paneW * 2.0);
                }

                auto btnH = selTextH;
                if ((m_comboStyle & 2) == 0)
                {
                    btnH -= paneW * 2.0;
                }
                auto x = m_bounds.width - selTextH;
                if ((m_comboStyle & 2) != 0)
                {
                    paneW = 0.0;
                }
                else
                {
                    x -= paneW;
                }
                BoundsBase<float> btnB;
                btnB.x0 = x;
                btnB.y0 = paneW;
                btnB.width = (x + btnH) - x;
                btnB.height = (paneW + btnH) - paneW;
                m_btnToggle->SetBounds(btnB, true);

                BoundsBase<float> textB;
                textB.x0 = 0.0;
                textB.y0 = 0.0;
                textB.width = m_bounds.width;
                textB.height = selTextH;
                m_wndSelText->SetBounds(textB, true);

                m_wndSelText->SetClientEdges(m_aif.m_space, m_aif.m_space, m_bounds.width - btnB.x0, m_aif.m_space);
                RecalcListBounds();
                RecalcSelfBounds();
            }
        }

        void ComboBoxWnd::SelectItem()
        {
            throw std::logic_error("Not implemented");
        }

        void ComboBoxWnd::ToggleState()
        {
            throw std::logic_error("Not implemented");
        }

        void ComboBoxWnd::RecalcSelfBounds()
        {
            if (Valid())
            {
                auto const bounds = m_wndStringList->GetBounds();
                auto const selTextBounds = GetSelTextBounds();
                m_bounds.height = selTextBounds.height + bounds.height;
            }
        }

        int ComboBoxWnd::OnWndNotify(Wnd*, unsigned, unsigned, AIParam const&)
        {
            throw std::logic_error("Not implemented");
        }

        void ComboBoxWnd::RecalcListBounds()
        {
            //TODO: check this
            if (Valid())
            {
                if (m_state)
                {
                    if (m_state == STATE_OPEN)
                    {
                        m_wndStringList->SetBounds(GetListBounds(), true);
                    }
                }
                else
                {
                    //TODO; check this
                    m_wndStringList->SetBounds(BoundsBase<float>{0.0, 0.0, 0.0, 0.0}, true);
                }
            }
        }

        int ComboBoxWnd::OnMouseButton0(unsigned, PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        int ComboBoxWnd::OnAfterRemoveFromWndStation()
        {
            throw std::logic_error("Not implemented");
        }

        int ComboBoxWnd::OnAfterAddToWndStation()
        {
            throw std::logic_error("Not implemented");
        }
    }    
}
