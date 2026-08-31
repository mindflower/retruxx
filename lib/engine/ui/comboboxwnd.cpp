#include <core/ini.h>
#include <ui/button.h>
#include <ui/comboboxwnd.h>
#include <ui/listbox.h>

#include "m3dapp.h"

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(ComboBoxWnd)
    	RT_CLASS_EXPORTS_END;

        RT_CLASS_DEFINE(ComboBoxWnd);

        ComboBoxWnd::AuxInfo::AuxInfo()
        {

        }

        int ComboBoxWnd::GetCurSel() const
        {
            if (Valid())
            {
                return m_wndStringList->GetCurSel();
            }
            return -1;
        }

        int ComboBoxWnd::GetItemData(int idx) const
        {
            if (Valid())
            {
                return m_wndStringList->GetItemData(idx);
            }
            return -1;
        }

        BoundsBase<float> ComboBoxWnd::GetListBounds() const
        {
            if (Valid())
            {
                float h = 0.0;
                float w = 0.0;
                for (int i = 0; i < m_wndStringList->GetCount(); ++i)
                {
                    const auto& bounds = m_wndStringList->GetItemBounds(i);
                    if (h + bounds.height > m_maxListH)
                    {
                        break;
                    }
                    h += bounds.height;
                }

                auto* pane = GetStation()->GetGfxServer()->GetPane(m_wndStringList->GetPaneName());
                if (pane)
                {
                    auto& frame = pane->m_frame[0];
                    if (frame)
                    {
                        w = frame->m_barUsedWidth;
                    }
                }

                const auto selBounds = GetSelTextBounds();
                BoundsBase<float> result;
                result.x0 = selBounds.x0;
                result.y0 = selBounds.height + selBounds.y0;
                result.width = selBounds.width + selBounds.x0 - selBounds.x0;
                result.height = ((((w * 2.0) + h) + selBounds.height) + selBounds.y0) - (selBounds.height + selBounds.y0);
                return result;

            }
            return { 0.0, 0.0, 0.0, 0.0 };
        }

        void ComboBoxWnd::SetCurSel(int idx)
        {
            if (Valid())
            {
                m_wndStringList->SetCurSel(idx);
            }
        }

        void ComboBoxWnd::SetItemData(int idx, int data)
        {
            if (Valid())
            {
                m_wndStringList->SetItemData(idx, data);
            }
        }

        unsigned ComboBoxWnd::GetComboStyle() const
        {
            return m_comboStyle;
        }

        void ComboBoxWnd::Close()
        {
            SetState(STATE_CLOSE, false);
        }

        BoundsBase<float> ComboBoxWnd::GetFullBounds() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void ComboBoxWnd::Open()
        {
            SetState(STATE_OPEN, false);
        }

        int ComboBoxWnd::GetCount() const
        {
            if (Valid())
            {
                return m_wndStringList->GetCount();
            }
            return 0;
        }

        float ComboBoxWnd::GetSelTextFixedHeight() const
        {
            return m_selTextFixedH;
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
            // The original deletes m_btnToggle / m_wndStringList / m_wndSelText explicitly here.
            // In retruxx the sub-windows are owned through the child list (released by
            // ~Wnd -> DestroyWnd -> RemoveAllChildren -> DecRef), so the body stays empty.
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

        void ComboBoxWnd::SetToggleButtonPane(CStr const& openPaneName, CStr const& closePaneName)
        {
            if (Valid())
            {
                m_toggleButtonOpenPaneName = openPaneName;
                m_toggleButtonClosePaneName = closePaneName;
                if (m_btnToggle)
                {
                    m_btnToggle->SetPane(
                        m_state == STATE_OPEN ? m_toggleButtonClosePaneName : m_toggleButtonOpenPaneName);
                }
            }
        }

        CStr ComboBoxWnd::GetItem(int idx) const
        {
            if (Valid())
            {
                return m_wndStringList->GetItem(idx);
            }
            return {};
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
            return new ComboBoxWnd(*this);
        }

        int ComboBoxWnd::AddItem(CStr const& item)
        {
            if (Valid())
            {
                return m_wndStringList->AddItem(item);
            }
            return -1;
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
            if (m_wndSelText)
            {
                return m_wndSelText->GetText();
            }
            return {};
        }

        void ComboBoxWnd::SetItem(int idx, CStr const& text)
        {
            if (Valid())
            {
                m_wndStringList->SetItem(idx, text);
            }
        }

        float ComboBoxWnd::GetMaxListHeight() const
        {
            return m_maxListH;
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
            RETRUXX_NOT_IMPLEMENTED;
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
            if (Valid())
            {
                return m_wndStringList->GetDrawFlags();
            }
            return 0;
        }

        int ComboBoxWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            RETRUXX_NOT_IMPLEMENTED;
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

        void ComboBoxWnd::SetDefaultFont(CStr const& name, float height, FontType type, FontParams params)
        {
            Wnd::SetDefaultFont(name, height, type, params);
            if (Valid())
            {
                m_wndStringList->SetDefaultFont(name, height, type, params);
                m_wndSelText->SetDefaultFont(name, height, type, params);
                RecalcLayot();
            }
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

        int ComboBoxWnd::RemoveItem(int idx)
        {
            if (Valid())
            {
                return m_wndStringList->RemoveItem(idx);
            }
            return 0;
        }

        BoundsBase<float> ComboBoxWnd::GetListMaxBounds() const
        {
            auto const listBounds = GetListBounds();
            BoundsBase<float> result;
            result.x0 = listBounds.x0;
            result.y0 = listBounds.y0 + listBounds.height;
            result.width = listBounds.width;
            result.height = m_maxListH;
            return result;
        }

        Object* ComboBoxWnd::CreateObject()
        {
            return new ComboBoxWnd;
        }

        void ComboBoxWnd::SetDrawFlags(unsigned flags)
        {
            if (Valid())
            {
                m_wndStringList->SetDrawFlags(flags);
            }
        }

        int ComboBoxWnd::InsertItem(CStr const& item, int idx)
        {
            if (Valid())
            {
                return m_wndStringList->InsertItem(item, idx);
            }
            return 0;
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

        int ComboBoxWnd::ItemFromPoint(PointBase<float> const& at)
        {
            if (!Valid())
            {
                return -1;
            }
            auto const screen = ToScreen(at);
            auto const listLocal = m_wndStringList->ToWindow(screen);
            return m_wndStringList->ItemFromPoint(listLocal);
        }

        int ComboBoxWnd::RemoveAllItems()
        {
            if (Valid())
            {
                return m_wndStringList->RemoveAllItems();
            }
            return 0;
        }

        bool ComboBoxWnd::IsOpen() const
        {
            return m_state == STATE_OPEN;
        }

        ComboBoxWnd::ComboBoxWnd(ComboBoxWnd const&)
        {
            // The original copy constructor only runs the default Wnd base setup and leaves
            // every ComboBoxWnd member at its default; it does not copy from the source.
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
            auto const oldState = m_state;
            m_state = state;
            if (oldState != state || bForceUpdate)
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
            if (Valid())
            {
                auto const curSel = m_wndStringList->GetCurSel();
                m_wndSelText->SetText(curSel != -1 ? m_wndStringList->GetItem(curSel) : CStr{});
                CallParentNotify(5, {}, false);
                Close();
            }
        }

        void ComboBoxWnd::ToggleState()
        {
            if (this->m_state == STATE_OPEN)
                this->Close();
            else
                this->Open();
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

        int ComboBoxWnd::OnWndNotify(Wnd* from, unsigned idFrom, unsigned message, AIParam const& data)
        {
            if (!Valid())
            {
                return 0;
            }
            if ((m_style & 0x100000) != 0)
            {
                ReflectChildNotifyToParent(from, idFrom, message, data);
            }
            if (idFrom == 5)
            {
                if (message == 1 && (m_comboStyle & 1) == 0)
                {
                    ToggleState();
                }
            }
            else if (idFrom == 6 && message == 5)
            {
                SelectItem();
                return 1;
            }
            return 1;;
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

        int ComboBoxWnd::OnMouseButton0(unsigned state, PointBase<float> const& at)
        {
            auto result = Wnd::OnMouseButton0(state, at);
            if (!result)
                return result;
            if ((this->m_comboStyle & 1) != 0)
            {
                if (state)
                    this->ToggleState();
            }
            return 1;
        }

        int ComboBoxWnd::OnAfterRemoveFromWndStation()
        {
            auto res = Wnd::OnAfterAddToWndStation();
            Close();
            return res;
        }

        int ComboBoxWnd::OnAfterAddToWndStation()
        {
            auto res = Wnd::OnAfterAddToWndStation();
            if (m_state != STATE_OPEN)
            {
                return res;
            }
            M3D_APP->EnqueueMessage(47, reinterpret_cast<int>(this), 0, 0, 0, {}, {});
            return res;
        }
    }    
}
