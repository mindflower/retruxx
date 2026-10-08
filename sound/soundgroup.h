#pragma once
// A sound group from the sound groups XML: an id, the parent group, a volume multiplier and the
// 3D distance range; the current volume is the group volume scaled by the multiplier.
#include <core/stringm3d.h>

namespace m3d
{
    namespace cmn
    {
        class XmlFile;
        struct XmlNode;
    }
}

namespace snd
{
    class SoundGroup
    {
        // The manager assigns the id when it registers the group.
        friend class CM3DSoundManager;

    public:
        SoundGroup(SoundGroup* parent);
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode);

        // orig 0x5efc90 soundgroup.h:35
        int GetId() const
        {
            // 36
            return m_id;
        }

        int GetParentId() const
        {
            return m_parentId;
        }

        // orig 0x5f14f0 soundgroup.h:45
        CStr GetName() const
        {
            // 46
            return m_name;
        }

        float GetVolumeMult() const
        {
            return m_volumeMult;
        }

        // orig 0x5efca0 soundgroup.h:56
        float GetMinDist() const
        {
            // 57
            return m_minDist;
        }

        // orig 0x5efcb0 soundgroup.h:61
        float GetMaxDist() const
        {
            // 62
            return m_maxDist;
        }

        // orig 0x5f0c90 soundgroup.h:66
        void SetVolume(int volume)
        {
            // 67
            int v = (int)((float)volume * m_volumeMult);
            m_currentVolume = v < 0 ? 0 : (v > 255 ? 255 : v);
        }

        // orig 0x5efcc0 soundgroup.h:71
        int GetVolume() const
        {
            // 72
            return m_currentVolume;
        }

    private:
        /* 0x0000 */ int m_id;
        /* 0x0004 */ int m_parentId;
        /* 0x0008 */ CStr m_name;
        /* 0x0014 */ float m_volumeMult;
        /* 0x0018 */ float m_minDist;
        /* 0x001c */ float m_maxDist;
        /* 0x0020 */ int m_currentVolume;
    }; /* size: 0x0024 */
}
