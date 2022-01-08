#pragma once
#include "uicontrols.h"

namespace m3d
{
    namespace ui
    {
        class MsgBoxDlg : public ModalWnd
        {
        public:
            RT_CLASS_DECLARE(MsgBoxDlg);

        public:
            virtual ~MsgBoxDlg();
            virtual Class* GetClass() const;
            virtual Object* Clone();
            int CreateMsgBoxDlg(CStr const&, CStr const&, BoundsBase<float> const&, unsigned int, float);
            static Object* __fastcall CreateObject();
            static Class* __fastcall GetBaseClass();

        protected:
            MsgBoxDlg(MsgBoxDlg const&);
            MsgBoxDlg();
            virtual int CloseModal(int);
            virtual int OnInitModal();
            virtual int OnWndNotify(Wnd*, unsigned int, unsigned int, AIParam const&);

        private:
            GlyphButton* m_buttons[3];
            unsigned int m_flags;
            CStr m_msg;
            CStr m_caption;
            int m_id0;
            float m_width;
            Wnd* m_captionWnd;
            Wnd* m_msgWnd;
        };

        class ArtDlg :  public MsgBoxDlg
        {
        public:
            RT_CLASS_DECLARE(ArtDlg);

        public:
            virtual Class * GetClass() const ;
            virtual ~ArtDlg();
            static Object * __fastcall CreateObject();
            int CreateArtDlg(CStr const &,CStr const &,CStr const &,CStr const &,BoundsBase<float> const &,unsigned int,float);
            virtual int OnInitModal();
            virtual Object * Clone();
            static Class * __fastcall GetBaseClass();

        protected:
            ArtDlg();
            ArtDlg(ArtDlg const &);

        private:
            rend::TexHandle m_tex;
            ImageWnd *m_image;
            CStr m_soundFileName;
            int m_soundTableId;
            int m_soundChannelId;
        };
    }
}
