#pragma once

namespace ai
{
    class AIParamRef
    {
    public:
        AIParamRef(int);
        AIParamRef & operator=(AIParamRef const &);

    private:
        int m_Num;
    };
}
