// Sound groups: construction from a parent group and loading from the XML node.
// Ported from the original sound/SoundGroup.cpp (SoundGroup.obj).
#include "soundgroup.h"

#include <core/ini.h>

namespace snd
{
    // orig 0x7db038 SoundGroup.cpp (file static)
    static float const CM3DSOUND_MIN_DISTANCE = 10.0f;
    // orig 0x7db03c SoundGroup.cpp (file static)
    static float const CM3DSOUND_MAX_DISTANCE = 1500.0f;

    // orig 0x5f6d20 SoundGroup.cpp:40
    SoundGroup::SoundGroup(SoundGroup* parent) :
        m_id(-1),
        m_parentId(-1),
        m_volumeMult(1.0f),
        m_minDist(CM3DSOUND_MIN_DISTANCE),
        m_maxDist(CM3DSOUND_MAX_DISTANCE),
        m_currentVolume(0)
    {
        // 41
        if (parent)
        {
            // 43
            m_parentId = parent->m_id;
            // 44
            m_volumeMult = parent->m_volumeMult;
            // 45
            m_minDist = parent->m_minDist;
            // 46
            m_maxDist = parent->m_maxDist;
        }
    }

    // orig 0x5f6d90 SoundGroup.cpp:53
    void SoundGroup::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode)
    {
        // 54
        m3d::SafeStrAttrib(m_name, xmlNode, "id");

        // 56
        m_volumeMult *= 100.0f;
        // 57
        m3d::SafeFloatAttrib(m_volumeMult, xmlNode, "volume");
        // 58
        m_volumeMult *= 0.01f;

        // 60
        m3d::SafeFloatAttrib(m_minDist, xmlNode, "minDist");
        // 61
        m3d::SafeFloatAttrib(m_maxDist, xmlNode, "maxDist");

        // 63
        if (m_minDist * 2.0f > m_maxDist)
        {
            // 64
            m_maxDist = m_minDist * 2.0f;
        }
    }
}
