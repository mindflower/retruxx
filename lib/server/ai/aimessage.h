#pragma once
#include <vector>

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }  // namespace cmn

    class AIParam;
}  // namespace m3d

namespace ai
{
    class AIMessage
    {
    public:
        int m_Num;
        int m_RemoveAfterFinishing;
        std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> m_ParamList;
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* OwnNode);
        void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const;
        AIMessage(int Num, m3d::AIParam const& Param1, m3d::AIParam const& Param2, m3d::AIParam const& Param3);
        AIMessage();
        bool operator==(ai::AIMessage const& message);
        bool operator!=(ai::AIMessage const&);
    };
}  // namespace ai
