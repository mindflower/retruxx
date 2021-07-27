#pragma once

namespace m3d
{
    class DataServer;

    class ServerFactories
    {
    public:
        static DataServer* CreateAnimatedModelServer();
        static DataServer* CreateDecalsServer();
        static DataServer* CreateLightsServer();
        static DataServer* CreateLinesServer();
        static DataServer* CreateMusicServer();
        static DataServer* CreateParticlesServer();
        static DataServer* CreateProjectorsServer();
        static DataServer* CreateSoundServer();
        static DataServer* CreateSpritesServer();
        static DataServer* CreateStaticModelServer();
    };
}