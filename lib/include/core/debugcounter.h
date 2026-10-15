#pragma once
#include "retruxx/common.h"

namespace m3d
{
    class Application;

    class DbgCounter
    {
        friend class DbgCounterStack;
        // Application::OneFrame draws the counters directly.
        friend class Application;
        enum eType
        {
            DBG_COUNTER_STRING = 0,
            DBG_COUNTER_INT = 1,
            DBG_COUNTER_FLOAT = 2,
            DBG_COUNTER_BOOL = 3,
        };

    public:
        void SetS(char const*);
        void SetI(int i);
        void SetF(float);
        void SetB(bool);
        void IncI();
        void DecI();
        char const* GetS() const;
        int GetI() const;
        float GetF() const;
        bool GetB() const;
        m3d::DbgCounter::eType GetType() const;
        void Reset();
        char const* GetName() const;

    private:
        DbgCounter(m3d::DbgCounter const&);
        DbgCounter();
        ~DbgCounter();
        void SetName(char const* name);

        m3d::DbgCounter::eType m_curType = DBG_COUNTER_STRING;
        union
        {
            int m_i = 0;
            float m_f;
            bool m_b;
        };
        retruxx::string m_s;
        retruxx::string m_name;
    };

    class Application;

    class DbgCounterStack
    {
        friend class Application;

    public:
        DbgCounterStack(m3d::DbgCounterStack const&);
        DbgCounterStack();
        ~DbgCounterStack();
        unsigned int AddCounter(char const* name);
        char const* GetName(unsigned int id) const;
        m3d::DbgCounter* GetCounter(unsigned int id);
        m3d::DbgCounter* GetCounterByName(char const* name);
        void Clear();
        unsigned int GetNumCounters() const;
        void DrawStringThisFrame(char const* str);
        void ClearStringStack();
        unsigned int GetNumStrings() const;
        char const* GetString(unsigned int id) const;

    private:
        retruxx::vector<m3d::DbgCounter*, retruxx::allocator<m3d::DbgCounter*>> m_stack;
        unsigned int m_numCounters = 0;
        retruxx::vector<retruxx::string> m_stringStack;
        unsigned int m_numStrings = 0;
    };
}  // namespace m3d
