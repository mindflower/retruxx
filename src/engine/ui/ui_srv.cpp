#include <ui/ui_srv.h>

namespace m3d
{
    int ui::GfxServer::Create()
    {
        m_created = true;
        return 1;
    }

    int ui::GfxServer::Done()
    {
        ReleaseSchema();
        if (m_fontManager)
        {
            delete m_fontManager;
        }
        m_created = false;
        return 1;
    }
}
