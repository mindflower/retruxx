#include <scene/servers/serveranimatedmodel.h>
#include <scene/servers/serverdecals.h>
#include <scene/servers/serverfactories.h>
#include <scene/servers/serverlight.h>
#include <scene/servers/serverlines.h>
#include <scene/servers/servermusic.h>
#include <scene/servers/serverparticles.h>
#include <scene/servers/serverprojectors.h>
#include <scene/servers/serversprites.h>
#include <scene/servers/serversound.h>
#include <scene/servers/serverstaticmodels.h>

namespace m3d
{
    DataServer* ServerFactories::CreateAnimatedModelServer()
    {
        return new AnimatedModelsServer;
    }

    DataServer* ServerFactories::CreateDecalsServer()
    {
        return new DecalsServer;
    }

    DataServer* ServerFactories::CreateLightsServer()
    {
        return new LightsServer;
    }

    DataServer* ServerFactories::CreateLinesServer()
    {
        return new LinesServer;
    }

    DataServer* ServerFactories::CreateMusicServer()
    {
        return new MusicServer;
    }

    DataServer* ServerFactories::CreateParticlesServer()
    {
        return new ParticlesServer;
    }

    DataServer* ServerFactories::CreateProjectorsServer()
    {
        return new ProjectorsServer;
    }

    DataServer* ServerFactories::CreateSoundServer()
    {
        return new Sound3DServer;
    }

    DataServer* ServerFactories::CreateSpritesServer()
    {
        return new SpritesServer;
    }

    DataServer* ServerFactories::CreateStaticModelServer()
    {
        return new StaticModelsServer;
    }
}
