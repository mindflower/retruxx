#include <m3dapp.h>
#include <stdexcept>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <core/scoped_ptr.h>
#include "core/ini.h"
#include <file/fileserver.h>
#include <file/filestream.h>
#include <ui/cursor.h>

namespace m3d
{
    namespace ui
    {
        int Cursor::Create(CStr const& name, PointBase<float> const& size, PointBase<float> const& spot)
        {
            Application::g_pApp->m_renderer->ReleaseTexture(m_tex);
            m_tex = Application::g_pApp->m_renderer->AddTexture(name, 12);
            m_sz = size;
            m_spot = spot;
            Application::g_pApp->m_renderer->SetTextureParameter(m_tex, rend::TM_WRAP_S, 3);
            Application::g_pApp->m_renderer->SetTextureParameter(m_tex, rend::TM_WRAP_T, 3);
            return 1;
        }

        int Cursor::Create(CStr const& scriptName)
        {
            ref_ptr iniFile = g_Kernel->CreateIniFile();
            scoped_ptr stream = g_Kernel->GetFileServer().CreateFileStream();
            if (stream->Open(scriptName.c_str(), fs::IStream::OPEN_READ))
            {
                iniFile->Read(*stream);
                stream->Close();
                auto const name = iniFile->GetString("CURSOR", "FACE");

                PointBase<float> size;
                size.x = iniFile->GetFloat("CURSOR", "SIZE_X");
                size.y = iniFile->GetFloat("CURSOR", "SIZE_Y");

                PointBase<float> spot;
                spot.x = iniFile->GetFloat("CURSOR", "SPOT_X");
                spot.y = iniFile->GetFloat("CURSOR", "SPOT_Y");
                return Create(name, size, spot);
            }
            M3D_LOG_INFO("Cannot load cursor script " + scriptName);
            return 0;
        }

        bool Cursor::operator==(Cursor const& c)
        {
            return this->m_tex == c.m_tex
                && this->m_spot.x == c.m_spot.x
                && this->m_spot.y == c.m_spot.y
                && this->m_sz.x == c.m_sz.x
                && this->m_sz.y == c.m_sz.y;
        }

        Cursor& Cursor::operator=(Cursor const& c)
        {
            if (this == &c)
            {
                return *this;
            }
            m_sz = c.m_sz;
            m_spot = c.m_spot;
            if (m_tex != c.m_tex)
            {
                Application::g_pApp->m_renderer->ReleaseTexture(m_tex);
                m_tex = c.m_tex;
                Application::g_pApp->m_renderer->ReferenceTexture(m_tex);
            }
            return *this;
        }

        Cursor::~Cursor()
        {
            if (Application::g_pApp->m_renderer)
            {
                Application::g_pApp->m_renderer->ReleaseTexture(m_tex);
            }
        }

        Cursor::Cursor() :
            m_sz(0.0, 0.0),
            m_spot(0.0, 0.0)
        {
        }

        Cursor::Cursor(Cursor const& c)
        {
            operator=(c);
        }
    }
}
