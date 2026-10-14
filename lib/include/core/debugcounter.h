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
        void SetS(const char*);
        void SetI(int i);
        void SetF(float);
        void SetB(bool);
        void IncI();
        void DecI();
        const char* GetS() const;
        int GetI() const;
        float GetF() const;
        bool GetB() const;
        m3d::DbgCounter::eType GetType() const;
        void Reset();
        const char* GetName() const;

    private:
        DbgCounter(const m3d::DbgCounter&);
        DbgCounter();
        ~DbgCounter();
        void SetName(const char* name);

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
        DbgCounterStack(const m3d::DbgCounterStack&);
        DbgCounterStack();
        ~DbgCounterStack();
        unsigned int AddCounter(const char* name);
        const char* GetName(unsigned int id) const;
        m3d::DbgCounter* GetCounter(unsigned int id);
        m3d::DbgCounter* GetCounterByName(const char* name);
        void Clear();
        unsigned int GetNumCounters() const;
        void DrawStringThisFrame(const char* str);
        void ClearStringStack();
        unsigned int GetNumStrings() const;
        const char* GetString(unsigned int id) const;

    private:
        retruxx::vector<m3d::DbgCounter*, retruxx::allocator<m3d::DbgCounter*> > m_stack;
        unsigned int m_numCounters = 0;
        retruxx::vector<retruxx::string> m_stringStack;
        unsigned int m_numStrings = 0;
    };
}
