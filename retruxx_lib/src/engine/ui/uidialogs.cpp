#include <ui/uidialogs.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(ArtDlg)
    	RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(ArtDlg);

        RT_CLASS_EXPORTS_BEGIN(MsgBoxDlg)
    	RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(MsgBoxDlg);

        MsgBoxDlg::~MsgBoxDlg()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* MsgBoxDlg::GetClass() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Object* MsgBoxDlg::Clone()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MsgBoxDlg::CreateMsgBoxDlg(CStr const&, CStr const&, BoundsBase<float> const&, unsigned, float)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Object* MsgBoxDlg::CreateObject()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* MsgBoxDlg::GetBaseClass()
        {
            return RT_CLASS_LOCAL(ModalWnd);
        }

        MsgBoxDlg::MsgBoxDlg(MsgBoxDlg const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        MsgBoxDlg::MsgBoxDlg()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MsgBoxDlg::CloseModal(int)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MsgBoxDlg::OnInitModal()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MsgBoxDlg::OnWndNotify(Wnd*, unsigned, unsigned, AIParam const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* ArtDlg::GetClass() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        ArtDlg::~ArtDlg()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Object* ArtDlg::CreateObject()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int ArtDlg::CreateArtDlg(CStr const&, CStr const&, CStr const&, CStr const&, BoundsBase<float> const&, unsigned, float)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int ArtDlg::OnInitModal()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Object* ArtDlg::Clone()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* ArtDlg::GetBaseClass()
        {
            return RT_CLASS_LOCAL(MsgBoxDlg);
        }

        ArtDlg::ArtDlg()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        ArtDlg::ArtDlg(ArtDlg const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
    }
}
