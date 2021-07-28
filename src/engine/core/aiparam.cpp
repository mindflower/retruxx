#include <core/aiparam.h>

namespace m3d
{
    AIParam::AIParam(AIParam const& param)
    {
        if (this != &param)
        {
            Copy(param);
        }
    }

    AIParam::~AIParam()
    {
        Detach();
    }
}
