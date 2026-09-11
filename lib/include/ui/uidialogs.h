#pragma once
#include "wnd.h"

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
        class GlyphButton;

        class MsgBoxDlg : public ModalWnd
        {
        protected:
            MsgBoxDlg();
            MsgBoxDlg(m3d::ui::MsgBoxDlg const& rhs);

        public:
            virtual ~MsgBoxDlg() override /* 0x00 */;
            virtual Object* Clone() override /* 0x00 */;
            static Object* __fastcall CreateObject();
            static Class* __fastcall GetBaseClass();
            virtual Class* GetClass() const override /* 0x00 */;
            RT_CLASS_DECLARE(MsgBoxDlg);

            int CreateMsgBoxDlg(CStr const& caption, CStr const& msg, BoundsBase<float> const& rc, unsigned int flags, float maxWidth);

        protected:
            virtual int OnInitModal() override /* 0x120 */;
            virtual int OnWndNotify(Wnd* from, unsigned int idFrom, unsigned int msg, AIParam const& data) override /* 0x00 */;
            virtual int CloseModal(int ret) override /* 0x124 */;

            /* 0x0224 */ GlyphButton* m_buttons[3];
            /* 0x0230 */ unsigned int m_flags = 0;
            /* 0x0234 */ CStr m_msg;
            // NOTE: deliberately shadows Wnd::m_caption; the dialog keeps its own
            // title separate from the window text, which OnInitModal clears.
            /* 0x0240 */ CStr m_caption;
            /* 0x024c */ int m_id0 = 0;
            /* 0x0250 */ float m_width = 0.0f;
            /* 0x0254 */ Wnd* m_captionWnd = nullptr;
            /* 0x0258 */ Wnd* m_msgWnd = nullptr;
        }; /* size: 0x025c */

        class ArtDlg : public MsgBoxDlg
        {
        protected:
            ArtDlg();
            ArtDlg(m3d::ui::ArtDlg const& rhs);

        public:
            virtual ~ArtDlg() override /* 0x00 */;
            virtual Object* Clone() override /* 0x00 */;
            static Object* __fastcall CreateObject();
            static Class* __fastcall GetBaseClass();
            virtual Class* GetClass() const override /* 0x00 */;
            RT_CLASS_DECLARE(ArtDlg);

            int CreateArtDlg(CStr const& caption, CStr const& msg, CStr const& imageFile, CStr const& soundFile, BoundsBase<float> const& rc, unsigned int flags, float maxWidth);
            virtual int OnInitModal() override /* 0x120 */;

        protected:
            /* 0x025c */ rend::TexHandle m_tex;
            /* 0x0260 */ ImageWnd* m_image = nullptr;
            /* 0x0264 */ CStr m_soundFileName;
            /* 0x0270 */ int m_soundTableId = -1;
            /* 0x0274 */ int m_soundChannelId = -1;
        }; /* size: 0x0278 */
    }
}
