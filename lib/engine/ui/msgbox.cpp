#include <m3dapp.h>
#include <core/kernel.h>
#include <ui/msgbox.h>
#include <ui/uidialogs.h>

namespace m3d
{
    namespace ui
    {
        MbRetCodes RunMsgBoxDlg(CStr const& caption, CStr const& message, unsigned int flags, bool pause)
        {
            // RVA 0x75CA40
            BoundsBase<float> rc;
            rc.x0 = (Application::g_pApp->m_bounds.width - 300.0f) * 0.5f;
            rc.y0 = (Application::g_pApp->m_bounds.height - 200.0f) * 0.5f;
            rc.width = 300.0f;
            rc.height = 200.0f;

            auto* dlg = static_cast<MsgBoxDlg*>(g_Kernel->New("MsgBoxDlg"));
            if (!dlg)
            {
                return MBX_RET_NO;
            }

            dlg->CreateMsgBoxDlg(caption, message, rc, flags | 8, 1000.0f);
            if (pause)
            {
                Application::g_pApp->Pause();
            }
            int const res = dlg->GetStation()->DoModal(dlg);
            if (pause)
            {
                Application::g_pApp->UnPause();
            }
            delete dlg;
            return static_cast<MbRetCodes>(res);
        }
    }
}
