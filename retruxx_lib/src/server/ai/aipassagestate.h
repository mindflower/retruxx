#pragma once
#include <vector>

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }

    class AIParam;
}

namespace ai
{
    class AIPassageState
    {
    public:
        /* 0x0000 */ int m_StateNum;
        /* 0x0004 */ std::vector<m3d::AIParam, std::allocator<m3d::AIParam> > m_ParamList;
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* OwnNode);
        void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const;
    }; /* size: 0x0014 */
}
