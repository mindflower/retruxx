#pragma once
#include <vector>

namespace ai
{
    class AIParamRef;

    class AIPassageCommand
    {
    public:
        void operator=(AIPassageCommand const &);
        void Dump();

    private:
        int m_StateNum;
        std::vector<AIParamRef> m_ParamRefList;
    };
}
