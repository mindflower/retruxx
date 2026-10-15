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
    class AIPassageState
    {
    public:
        int m_StateNum;
        std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> m_ParamList;
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* OwnNode);
        void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const;
    };
}  // namespace ai
