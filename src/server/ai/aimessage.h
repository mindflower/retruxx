#pragma once
#include <vector>

namespace m3d
{
    namespace cmn
    {
        class XmlNode;
        class XmlFile;
    }

    class AIParam;
}

namespace ai
{
    class AIMessage
    {
    public:
        AIMessage(int,m3d::AIParam const &,m3d::AIParam const &,m3d::AIParam const &);
        AIMessage(AIMessage const &);
        AIMessage & operator=(AIMessage const &);
        bool operator==(AIMessage const &);
        void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;

    private:
        int m_Num;
        int m_RemoveAfterFinishing;
        std::vector<m3d::AIParam> m_ParamList;
    };
}
