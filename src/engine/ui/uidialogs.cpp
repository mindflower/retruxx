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
            throw std::logic_error("Not implemented");
        }

        Class* MsgBoxDlg::GetClass() const
        {
            throw std::logic_error("Not implemented");
        }

        Object* MsgBoxDlg::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        int MsgBoxDlg::CreateMsgBoxDlg(CStr const&, CStr const&, BoundsBase<float> const&, unsigned, float)
        {
            throw std::logic_error("Not implemented");
        }

        Object* MsgBoxDlg::CreateObject()
        {
            throw std::logic_error("Not implemented");
        }

        Class* MsgBoxDlg::GetBaseClass()
        {
            return RT_CLASS_LOCAL(ModalWnd);
        }

        MsgBoxDlg::MsgBoxDlg(MsgBoxDlg const&)
        {
            throw std::logic_error("Not implemented");
        }

        MsgBoxDlg::MsgBoxDlg()
        {
            throw std::logic_error("Not implemented");
        }

        int MsgBoxDlg::CloseModal(int)
        {
            throw std::logic_error("Not implemented");
        }

        int MsgBoxDlg::OnInitModal()
        {
            throw std::logic_error("Not implemented");
        }

        int MsgBoxDlg::OnWndNotify(Wnd*, unsigned, unsigned, AIParam const&)
        {
            throw std::logic_error("Not implemented");
        }

        Class* ArtDlg::GetClass() const
        {
            throw std::logic_error("Not implemented");
        }

        ArtDlg::~ArtDlg()
        {
            throw std::logic_error("Not implemented");
        }

        Object* ArtDlg::CreateObject()
        {
            throw std::logic_error("Not implemented");
        }

        int ArtDlg::CreateArtDlg(CStr const&, CStr const&, CStr const&, CStr const&, BoundsBase<float> const&, unsigned, float)
        {
            throw std::logic_error("Not implemented");
        }

        int ArtDlg::OnInitModal()
        {
            throw std::logic_error("Not implemented");
        }

        Object* ArtDlg::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        Class* ArtDlg::GetBaseClass()
        {
            return RT_CLASS_LOCAL(MsgBoxDlg);
        }

        ArtDlg::ArtDlg()
        {
            throw std::logic_error("Not implemented");
        }

        ArtDlg::ArtDlg(ArtDlg const&)
        {
            throw std::logic_error("Not implemented");
        }
    }
}
