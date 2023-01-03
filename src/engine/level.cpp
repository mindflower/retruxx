#include <level.h>
#include <stdexcept>

#include "m3dapp.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/ref_ptr.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Level, GetLandSize)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Level, GetLevelName)
{
    throw std::logic_error("Not implemented");
}

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(Level)
        RT_CLASS_EXPORT(Level, m3d::METHOD, GetLandSize, "", "", "")
        RT_CLASS_EXPORT(Level, m3d::METHOD, GetLevelName, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Level);

    Class* Level::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    char const* Level::GetLevelName() const
    {
        return m_levelName.c_str();
    }

    int Level::GetLandSize() const
    {
        throw std::logic_error("Not implemented");
    }

    Object* Level::CreateObject()
    {
        return new Level;
    }

    int Level::Save(CStr const&, CCamera const&)
    {
        throw std::logic_error("Not implemented");
    }

    Level::~Level()
    {
        throw std::logic_error("Not implemented");
    }

    Object* Level::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    CStr Level::GetFullPathNameA(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    int Level::New(CCamera&, int)
    {
        throw std::logic_error("Not implemented");
    }

    Class* Level::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    int Level::Load(CStr const& name, CCamera& cam, bool bQuiet)
    {
        ref_ptr file = g_Kernel->CreateIniFile();
        scoped_ptr stream = g_Kernel->GetFileServer().CreateFileStream();
        if (stream->Open(name.c_str(), fs::IStream::OPEN_READ))
        {
            file->Read(*stream);
            stream->Close();
            auto err = file->GetError();
            if (err)
            {
                M3D_LOG_INFO("Error: Parse error file " + name + " err " + CStr(err));
                return 0;
            }
            m_levelName = "TEST";
            m_levelPath = file->GetString("LEVEL", "PATH");
            m_levelName = file->GetString("LEVEL", "LEVELNAME");
            if (!bQuiet)
            {
                m3d::Application::g_pApp->PutSplash(0, Application::g_pApp->GetStringByStringId0("LoadingDots").c_str());
            }
            m_hfName = file->GetString("LEVEL", "HIGHMAP");
            m_waterName = file->GetString("LEVEL", "DETMAP");
            m_cameraMapName = file->GetString("LEVEL", "CAMERAMAP");
        }
        throw std::logic_error("Not implemented");
    }

    Level::Level(Level const&)
    {
        throw std::logic_error("Not implemented");
    }

    Level::Level()
    {
        this->land_size = 0;
        this->m_skyType = 0;
        this->m_skyCloudsEnable = 0;
        this->m_lsFarColor = 0;
        this->m_olsFarColor = 0;
        this->m_lsSkyColor = 0;
        this->m_olsSkyColor = 0;
        this->m_skyScrollSpeed = 0.0;
        this->m_skyRotateSpeed = 0.0;
        this->m_sunAzimuthSpeed = 0.0;
        this->m_sunAzimuth = 0.0;
        this->m_sunDayAscention = 0.0;
        this->m_sunRiseAscention = 0.0;
        this->m_sunSetAscention = 0.0;
        this->waterlevel = 0.0;
        this->m_baseWaterLevel = 0.0;
    }
}
