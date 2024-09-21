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
        void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);

    private:
        int m_StateNum;
        std::vector<m3d::AIParam> m_ParamList;
    };
}
