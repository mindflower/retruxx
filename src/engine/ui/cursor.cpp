#include <m3dapp.h>
#include <stdexcept>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <core/scoped_ptr.h>
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

        bool Cursor::operator==(Cursor const&)
        {
            throw std::logic_error("Not implemented");
        }

        Cursor& Cursor::operator=(Cursor const&)
        {
            throw std::logic_error("Not implemented");
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

        Cursor::Cursor(Cursor const&)
        {
            throw std::logic_error("Not implemented");
        }
    }
}
