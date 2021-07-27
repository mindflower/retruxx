#pragma once
#include "DataServer.h"
#include <memory>

namespace m3d
{
    class ServerFactories
    {
    public:
        static std::unique_ptr<DataServer> CreateAnimatedModelServer();
        static std::unique_ptr<DataServer> CreateDecalsServer();
        static std::unique_ptr<DataServer> CreateLightsServer();
        static std::unique_ptr<DataServer> CreateLinesServer();
        static std::unique_ptr<DataServer> CreateMusicServer();
        static std::unique_ptr<DataServer> CreateParticlesServer();
        static std::unique_ptr<DataServer> CreateProjectorsServer();
        static std::unique_ptr<DataServer> CreateSoundServer();
        static std::unique_ptr<DataServer> CreateSpritesServer();
        static std::unique_ptr<DataServer> CreateStaticModelServer();
    };
}