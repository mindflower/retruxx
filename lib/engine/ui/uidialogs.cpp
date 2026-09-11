#include <config.h>
#include <core/kernel.h>
#include <iface.h>
#include <m3dapp.h>
#include <ui/glyphbutton.h>
#include <ui/image.h>
#include <ui/ui_srv.h>
#include <ui/uidialogs.h>
#include <ui/wndstation.h>

namespace m3d
{
    namespace ui
    {
        namespace
        {
            // Layout constants the two dialogs share.
            float const DLG_SIDE_MARGIN = 20.0f;
            float const DLG_CAPTION_HEIGHT = 30.0f;
            float const DLG_BTN_BOTTOM_MARGIN = 10.0f;
            float const DLG_BTN_GAP = 10.0f;
            float const DLG_BTN_MIN_X = 25.0f;
            float const DLG_BTN_PADDING = 20.0f;
            float const DLG_BTN_MEASURE_WIDTH = 80.0f;

            // Style the title and the message windows are created with.
            unsigned const CAPTION_STYLE = 3904;
            unsigned const MSG_STYLE = 832;

            // Style CreateMsgBoxDlg gives the dialog itself, and the id it
            // registers under.
            unsigned const DLG_STYLE = 0xCE08;
            unsigned const DLG_ID = 3;

            // The button set a dialog shows, chosen by flags & MBX_BUTTON_FLAGS.
            struct ButtonSet
            {
                int m_count;
                CStr m_strings[3];
                GlyphButton::GlyphButtonType m_types[3];
            };

            ButtonSet GetButtonSet(unsigned flags)
            {
                ButtonSet set;
                set.m_count = 0;
                switch (flags & MBX_BUTTON_FLAGS)
                {
                case MBX_OK:
                    set.m_count = 1;
                    set.m_strings[0] = CStr("^") + "ok" + "^";
                    set.m_types[0] = GlyphButton::GBTYPE_OK;
                    break;
                case MBX_YES_NO_CANCEL:
                    set.m_count = 3;
                    set.m_strings[2] = CStr("^") + "cancel" + "^";
                    set.m_types[2] = GlyphButton::GBTYPE_CANCEL;
                    // fall through: yes/no are laid out the same either way
                case MBX_YES_NO:
                    if (!set.m_count)
                    {
                        set.m_count = 2;
                    }
                    set.m_strings[0] = CStr("^") + "yes" + "^";
                    set.m_strings[1] = CStr("^") + "no" + "^";
                    set.m_types[0] = GlyphButton::GBTYPE_YES;
                    set.m_types[1] = GlyphButton::GBTYPE_NO;
                    break;
                default:
                    break;
                }
                return set;
            }

            // Creates the button row along the bottom edge of the dialog. The
            // shipped code inlines this block in both OnInitModal overloads.
            void CreateButtonRow(Wnd* dlg, ButtonSet const& set, GlyphButton** buttons)
            {
                auto const rc = dlg->GetBounds();

                int btnWidth = Wnd::GetGfxServer()->GetBtnWidth();
                int const btnHeight = Wnd::GetGfxServer()->GetBtnHeight();
                for (auto i = 0; i < set.m_count; ++i)
                {
                    auto const w =
                        Wnd::GetGfxServer()->MeasureText(set.m_strings[i], 0, TW_NOWRAP, DLG_BTN_MEASURE_WIDTH);
                    if (w.x > static_cast<float>(btnWidth))
                    {
                        btnWidth = static_cast<int>(w.x);
                    }
                }

                float const btnW = static_cast<float>(btnWidth) + DLG_BTN_PADDING;
                float const totalW = set.m_count * (btnW + DLG_BTN_GAP) - DLG_BTN_GAP;
                float const y0 = (rc.height - DLG_BTN_BOTTOM_MARGIN) - static_cast<float>(btnHeight);
                float x0 = totalW <= rc.width ? (rc.width - totalW) * 0.5f : DLG_BTN_MIN_X;

                for (auto i = 0; i < set.m_count; ++i)
                {
                    BoundsBase<float> b;
                    b.x0 = x0;
                    b.y0 = y0;
                    b.width = btnW;
                    b.height = static_cast<float>(btnHeight);
                    auto* btn = RT_DYNCAST(g_Kernel->New("GlyphButton"), GlyphButton);
                    buttons[i] = btn;
                    btn->CreateGlyphButton(b, set.m_types[i]);
                    btn->m_activationOrder = i;
                    x0 += btnW + DLG_BTN_GAP;
                    dlg->AddChild(btn);
                }
            }

            // Moves the finished button row down to the given y.
            void PlaceButtonRow(GlyphButton* const* buttons, int count, float y0)
            {
                for (auto i = 0; i < count; ++i)
                {
                    auto b = buttons[i]->GetBounds();
                    b.y0 = y0;
                    buttons[i]->SetBounds(b, true);
                }
            }

            // Centres the dialog in its parent when MBX_CENTRALIZED is set.
            void CentraliseInParent(Wnd* dlg, unsigned flags)
            {
                if ((flags & MBX_CENTRALIZED) == 0)
                {
                    return;
                }
                auto* parent = RT_DYNCAST(dlg->GetParent(), Wnd);
                if (!parent)
                {
                    return;
                }
                auto const pb = parent->GetBounds();
                auto b = dlg->GetBounds();
                b.x0 = (pb.width - b.width) * 0.5f;
                b.y0 = (pb.height - b.height) * 0.5f;
                dlg->SetBounds(b, true);
            }

            bool IsSoundEnabled()
            {
                return M3D_ENGINE_CFG.m_snd_Enable.GetB();
            }
        }  // namespace

        RT_CLASS_EXPORTS_BEGIN(ArtDlg)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(ArtDlg);

        RT_CLASS_EXPORTS_BEGIN(MsgBoxDlg)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(MsgBoxDlg);

        // =====================================================================
        //  MsgBoxDlg
        // =====================================================================

        MsgBoxDlg::MsgBoxDlg()
        {
            // RVA 0x7985C0
            m_buttons[0] = nullptr;
            m_buttons[1] = nullptr;
            m_buttons[2] = nullptr;
            m_captionWnd = nullptr;
            m_msgWnd = nullptr;
            m_width = 1000.0f;
        }

        MsgBoxDlg::MsgBoxDlg(MsgBoxDlg const&)
        {
            // RVA 0x798630 - the shipped copy constructor copies nothing from rhs
            // and, unlike the default one, leaves the three button pointers, the
            // flags, the two child windows and the width uninitialised.
        }

        MsgBoxDlg::~MsgBoxDlg()
        {
            // RVA 0x798670 - the buttons and the two text windows are ordinary
            // children; only the two strings need unwinding.
        }

        Object* MsgBoxDlg::Clone()
        {
            // RVA 0x798900
            return new MsgBoxDlg(*this);
        }

        Object* MsgBoxDlg::CreateObject()
        {
            return new MsgBoxDlg;
        }

        Class* MsgBoxDlg::GetBaseClass()
        {
            return RT_CLASS_LOCAL(ModalWnd);
        }

        Class* MsgBoxDlg::GetClass() const
        {
            return RT_CLASS_LOCAL(MsgBoxDlg);
        }

        int MsgBoxDlg::CreateMsgBoxDlg(CStr const& caption, CStr const& msg, BoundsBase<float> const& rc,
                                       unsigned flags, float maxWidth)
        {
            // RVA 0x7986E0 - only records what the dialog should look like; the
            // controls are built in OnInitModal, once the size is known.
            if (!ModalWnd::Create(CStr(), DLG_STYLE, rc, DLG_ID))
            {
                return 0;
            }
            m_flags = flags;
            m_width = maxWidth;
            m_caption = caption;
            m_msg = msg;
            return 1;
        }

        int MsgBoxDlg::OnInitModal()
        {
            // RVA 0x7989B0
            auto const bounds = GetBounds();
            BoundsBase<float> rect;
            rect.x0 = DLG_SIDE_MARGIN;
            rect.y0 = 0.0f;
            rect.width = bounds.width - 2.0f * DLG_SIDE_MARGIN;
            rect.height = bounds.height;

            if (!m_caption.empty())
            {
                rect.height = DLG_CAPTION_HEIGHT;
                m_captionWnd = RT_DYNCAST(g_Kernel->New("Wnd"), Wnd);
                m_captionWnd->Create(m_caption, CAPTION_STYLE, rect, 0);
                m_captionWnd->AdjustForWndTextToFit(0, m_width);
                m_captionWnd->m_activationOrder = -1;
                AddChild(m_captionWnd);

                // The title may have grown the dialog, so re-measure before the
                // message goes in underneath it.
                auto const b = GetBounds();
                rect.x0 = DLG_SIDE_MARGIN;
                rect.y0 = 2.0f * DLG_SIDE_MARGIN;
                rect.width = b.width - 2.0f * DLG_SIDE_MARGIN;
                rect.height = b.height - 4.0f * DLG_SIDE_MARGIN;
            }

            if (!m_msg.empty())
            {
                m_msgWnd = RT_DYNCAST(g_Kernel->New("Wnd"), Wnd);
                m_msgWnd->Create(m_msg, MSG_STYLE, rect, 0);
                m_msgWnd->AdjustForWndTextToFit(1, m_width);
                m_msgWnd->SetDefaultFont(1);
                m_msgWnd->m_activationOrder = -1;
                AddChild(m_msgWnd);
            }

            AdjustToFitChildren();

            auto const set = GetButtonSet(m_flags);
            CreateButtonRow(this, set, m_buttons);

            // The row sits just under whichever text window is present; with no
            // text at all it is centred vertically instead.
            float btnY;
            auto* ref = m_msgWnd ? m_msgWnd : m_captionWnd;
            if (ref)
            {
                auto const b = ref->GetBounds();
                btnY = b.y0 + b.height + DLG_BTN_GAP;
            }
            else
            {
                btnY = (GetBounds().height - m_buttons[0]->GetBounds().height) * 0.5f;
            }
            PlaceButtonRow(m_buttons, set.m_count, btnY);

            AdjustToFitChildren();
            SetText(CStr());
            CentraliseInParent(this, m_flags);
            return 1;
        }

        int MsgBoxDlg::OnWndNotify(Wnd* from, unsigned idFrom, unsigned msg, AIParam const& data)
        {
            // RVA 0x798450 - any button click closes the dialog with that button's
            // id, which GlyphButton set to the matching MBX_RET_* code.
            if (msg == 1)
            {
                if ((m_style & WS_REFLECT_NOTIFY_MESSAGES_TO_PARENT) != 0)
                {
                    ReflectChildNotifyToParent(from, idFrom, 1u, data);
                }
                if (GetStation()->IsModal(this))
                {
                    CloseModal(idFrom);
                }
            }
            return 1;
        }

        int MsgBoxDlg::CloseModal(int ret)
        {
            // RVA 0x7984B0 - a cancel that has no Cancel button to come from (the
            // Escape key) is folded onto whatever the dialog's rightmost button is.
            if (ret == MBX_RET_CANCEL)
            {
                if ((m_flags & MBX_BUTTON_FLAGS) == MBX_OK)
                {
                    ret = MBX_RET_OK;
                }
                else if ((m_flags & MBX_BUTTON_FLAGS) == MBX_YES_NO)
                {
                    return ModalWnd::CloseModal(MBX_RET_NO);
                }
            }
            return ModalWnd::CloseModal(ret);
        }

        // =====================================================================
        //  ArtDlg
        // =====================================================================

        ArtDlg::ArtDlg()
        {
            // RVA 0x79ABD0
            m_tex.SetInvalid();
            m_image = nullptr;
            m_soundTableId = -1;
            m_soundChannelId = -1;
        }

        ArtDlg::ArtDlg(ArtDlg const&)
        {
            // RVA 0x79AC20 - the shipped copy constructor copies nothing from rhs
            // and, unlike the default one, leaves the image pointer and both sound
            // ids uninitialised; only the texture handle is made safe to release.
            m_tex.SetInvalid();
        }

        ArtDlg::~ArtDlg()
        {
            // RVA 0x798780
            if (m_soundTableId != -1)
            {
                if (IsSoundEnabled())
                {
                    M3D_APP->m_sound->DeleteIdTableSound(m_soundTableId);
                }
                m_soundTableId = -1;
                m_soundChannelId = -1;
            }
            if (m_tex.IsValid())
            {
                M3D_RENDERER->ReleaseTexture(m_tex);
            }
        }

        Object* ArtDlg::Clone()
        {
            // RVA 0x79AC60
            return new ArtDlg(*this);
        }

        Object* ArtDlg::CreateObject()
        {
            return new ArtDlg;
        }

        Class* ArtDlg::GetBaseClass()
        {
            return RT_CLASS_LOCAL(MsgBoxDlg);
        }

        Class* ArtDlg::GetClass() const
        {
            return RT_CLASS_LOCAL(ArtDlg);
        }

        int ArtDlg::CreateArtDlg(CStr const& caption, CStr const& msg, CStr const& imageFile, CStr const& soundFile,
                                 BoundsBase<float> const& rc, unsigned flags, float maxWidth)
        {
            // RVA 0x799650
            if (!MsgBoxDlg::CreateMsgBoxDlg(caption, msg, rc, flags, maxWidth))
            {
                return 0;
            }

            m_soundFileName = soundFile;
            if (!m_soundFileName.empty() && IsSoundEnabled())
            {
                m_soundTableId = M3D_APP->m_sound->AddSound(
                    soundFile.c_str(), snd::SND_TYPE_2DSOUND, 1, 1, snd::SND_PRIORITY_HIGH);
            }

            if (!imageFile.empty())
            {
                m_tex = M3D_RENDERER->AddTexture(imageFile, 4);
                if (m_tex.IsValid())
                {
                    // The picture is inset ten pixels from the top left and shown
                    // at its native size.
                    int texW = 0;
                    int texH = 0;
                    M3D_RENDERER->GetDims(m_tex, texW, texH);
                    m_image = RT_DYNCAST(g_Kernel->New("ImageWnd"), ImageWnd);
                    if (m_image)
                    {
                        BoundsBase<float> const imgB{DLG_BTN_GAP,
                                                     DLG_BTN_GAP,
                                                     static_cast<float>(texW) - DLG_BTN_GAP,
                                                     static_cast<float>(texH) - DLG_BTN_GAP};
                        m_image->CreateImageWnd(imgB, m_tex);
                        AddChild(m_image);
                    }
                }
            }
            return 1;
        }

        int ArtDlg::OnInitModal()
        {
            // RVA 0x799820 - the same layout as MsgBoxDlg, except the text column
            // is pushed clear of the picture and the dialog's sound starts once
            // everything is in place.
            auto const bounds = GetBounds();
            BoundsBase<float> rect;
            rect.x0 = DLG_SIDE_MARGIN;
            rect.y0 = 0.0f;
            rect.width = bounds.width - 2.0f * DLG_SIDE_MARGIN;
            rect.height = bounds.height;
            float maxTextWidth = m_width;

            if (m_image)
            {
                auto const imgW = m_image->GetBounds().width;
                rect.width -= imgW;
                rect.x0 += imgW;
                maxTextWidth -= imgW;
            }

            if (!m_caption.empty())
            {
                rect.height = DLG_CAPTION_HEIGHT;
                m_captionWnd = RT_DYNCAST(g_Kernel->New("Wnd"), Wnd);
                m_captionWnd->Create(m_caption, CAPTION_STYLE, rect, 0);
                m_captionWnd->AdjustForWndTextToFit(0, maxTextWidth);
                m_captionWnd->m_activationOrder = -1;
                AddChild(m_captionWnd);

                // NOTE: unlike MsgBoxDlg, this shifts the rect it already has
                // rather than re-measuring the dialog after the title went in.
                rect.y0 += 2.0f * DLG_SIDE_MARGIN;
                rect.height -= 4.0f * DLG_SIDE_MARGIN;
            }

            if (!m_msg.empty())
            {
                m_msgWnd = RT_DYNCAST(g_Kernel->New("Wnd"), Wnd);
                m_msgWnd->Create(m_msg, MSG_STYLE, rect, 0);
                m_msgWnd->AdjustForWndTextToFit(1, maxTextWidth);
                m_msgWnd->SetDefaultFont(1);
                m_msgWnd->m_activationOrder = -1;
                AddChild(m_msgWnd);
            }

            AdjustToFitChildren();

            auto const set = GetButtonSet(m_flags);
            CreateButtonRow(this, set, m_buttons);

            // The row clears the picture as well as the text, so it goes under
            // whichever of the two reaches lower.
            float btnY;
            auto* text = m_msgWnd ? m_msgWnd : m_captionWnd;
            if (m_image && text)
            {
                auto const ib = m_image->GetBounds();
                auto const tb = text->GetBounds();
                float const imgBottom = ib.y0 + ib.height;
                float const textBottom = tb.y0 + tb.height;
                btnY = (imgBottom > textBottom ? imgBottom : textBottom) + DLG_BTN_GAP;
            }
            else if (m_image || text)
            {
                auto const b = m_image ? m_image->GetBounds() : text->GetBounds();
                btnY = b.y0 + b.height + DLG_BTN_GAP;
            }
            else
            {
                btnY = (GetBounds().height - m_buttons[0]->GetBounds().height) * 0.5f;
            }
            PlaceButtonRow(m_buttons, set.m_count, btnY);

            AdjustToFitChildren();
            SetText(CStr());
            CentraliseInParent(this, m_flags);

            if (m_soundTableId != -1 && m_soundChannelId == -1 && IsSoundEnabled())
            {
                m_soundChannelId =
                    M3D_APP->m_sound->PlaySound2D(m_soundTableId, (m_flags & MBX_SOUNDLOOP) != 0);
            }
            return 1;
        }
    }  // namespace ui
}  // namespace m3d
