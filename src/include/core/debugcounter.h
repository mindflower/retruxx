#pragma once
#include <string>
#include <vector>

namespace m3d
{
    class DbgCounter
    {
        friend class DbgCounterStack;
    public:
        enum eType
        {
            DBG_COUNTER_STRING = 0x0,
            DBG_COUNTER_INT = 0x1,
            DBG_COUNTER_FLOAT = 0x2,
            DBG_COUNTER_BOOL = 0x3,
        };

    public:
        void SetI(int);
        eType GetType() const;
        int GetI() const;
        bool GetB() const;
        float GetF() const;
        char const* GetS() const;
        char const* GetName() const;
        void IncI();

    private:
        DbgCounter();
        ~DbgCounter();
        void SetName(char const*);

    private:
        eType m_curType = DBG_COUNTER_STRING;
        union
        {
            int m_i = 0;
            float m_f;
            bool m_b;
        };
        std::string m_s;
        std::string m_name;
    };

    class DbgCounterStack
    {
    public:
        DbgCounter* GetCounter(unsigned int);
        ~DbgCounterStack();
        unsigned int AddCounter(char const*);
        void ClearStringStack();
        DbgCounter* GetCounterByName(char const*);
        char const* GetString(unsigned int) const;
        char const* GetName(unsigned int) const;
        DbgCounterStack();
        unsigned int GetNumStrings() const;
        void Clear();
        unsigned int GetNumCounters() const;
        void DrawStringThisFrame(char const*);

    private:
        std::vector<m3d::DbgCounter*> m_stack;
        unsigned int m_numCounters = 0;
        std::vector<std::string> m_stringStack;
        unsigned int m_numStrings = 0;
    };
}
