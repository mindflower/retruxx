#include "video.h"

namespace m3d
{
    mVideoPlayer::~mVideoPlayer()
    {
        if (IsVideoPlaing())
        {
            Stop();
        }
    }

    int mVideoPlayer::Init()
    {
        return 1;
    }
}
