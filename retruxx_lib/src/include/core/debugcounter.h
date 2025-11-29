#pragma once
#include "retruxx/common.h"

namespace m3d
{
    class DbgCounter
    {
        friend class DbgCounterStack;
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

        /* 0x0000 */ m3d::DbgCounter::eType m_curType = DBG_COUNTER_STRING;
        union
        {
            /* 0x0004 */ int m_i = 0;
            /* 0x0004 */ float m_f;
            /* 0x0004 */ bool m_b;
        }; /* size: 0x0004 */
        /* 0x0008 */ retruxx::string m_s;
        /* 0x0024 */ retruxx::string m_name;
    }; /* size: 0x0040 */

    static_assert(sizeof(DbgCounter) == 0x0040);

    class DbgCounterStack
    {
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
        /* 0x0000 */ retruxx::vector<m3d::DbgCounter*, retruxx::allocator<m3d::DbgCounter*> > m_stack;
        /* 0x0010 */ unsigned int m_numCounters = 0;
        /* 0x0014 */ retruxx::vector<retruxx::string> m_stringStack;
        /* 0x0024 */ unsigned int m_numStrings = 0;
    }; /* size: 0x0028 */

    static_assert(sizeof(DbgCounterStack) == 0x0028);
}
