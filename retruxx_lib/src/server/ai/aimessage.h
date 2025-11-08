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
    class AIMessage
    {
    public:
        /* 0x0000 */ int m_Num;
        /* 0x0004 */ int m_RemoveAfterFinishing;
        /* 0x0008 */ std::vector<m3d::AIParam, std::allocator<m3d::AIParam> > m_ParamList;
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* OwnNode);
        void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const;
        AIMessage(int Num, const m3d::AIParam& Param1, const m3d::AIParam& Param2, const m3d::AIParam& Param3);
        AIMessage();
        bool operator==(const ai::AIMessage& message);
        bool operator!=(const ai::AIMessage&);
    }; /* size: 0x0018 */
}
