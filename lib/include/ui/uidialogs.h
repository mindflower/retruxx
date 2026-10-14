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
            virtual ~MsgBoxDlg() override;
            virtual Object* Clone() override;
            static Object* __fastcall CreateObject();
            static Class* __fastcall GetBaseClass();
            virtual Class* GetClass() const override;
            RT_CLASS_DECLARE(MsgBoxDlg);

            int CreateMsgBoxDlg(CStr const& caption, CStr const& msg, BoundsBase<float> const& rc, unsigned int flags, float maxWidth);

        protected:
            virtual int OnInitModal() override;
            virtual int OnWndNotify(Wnd* from, unsigned int idFrom, unsigned int msg, AIParam const& data) override;
            virtual int CloseModal(int ret) override;

            GlyphButton* m_buttons[3];
            unsigned int m_flags = 0;
            CStr m_msg;
            // NOTE: deliberately shadows Wnd::m_caption; the dialog keeps its own
            // title separate from the window text, which OnInitModal clears.
            CStr m_caption;
            int m_id0 = 0;
            float m_width = 0.0f;
            Wnd* m_captionWnd = nullptr;
            Wnd* m_msgWnd = nullptr;
        };

        class ArtDlg : public MsgBoxDlg
        {
        protected:
            ArtDlg();
            ArtDlg(m3d::ui::ArtDlg const& rhs);

        public:
            virtual ~ArtDlg() override;
            virtual Object* Clone() override;
            static Object* __fastcall CreateObject();
            static Class* __fastcall GetBaseClass();
            virtual Class* GetClass() const override;
            RT_CLASS_DECLARE(ArtDlg);

            int CreateArtDlg(CStr const& caption, CStr const& msg, CStr const& imageFile, CStr const& soundFile, BoundsBase<float> const& rc, unsigned int flags, float maxWidth);
            virtual int OnInitModal() override;

        protected:
            rend::TexHandle m_tex;
            ImageWnd* m_image = nullptr;
            CStr m_soundFileName;
            int m_soundTableId = -1;
            int m_soundChannelId = -1;
        };
    }
}
