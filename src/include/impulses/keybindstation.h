#pragma once
#include <set>
#include <vector>

namespace m3d
{
    class KeysSet
    {
    public:
        KeysSet();
        KeysSet(KeysSet const&);
        KeysSet(int);

        void clear();
        unsigned int size() const;
        bool empty() const;
        KeysSet& operator-=(int);
        KeysSet& operator+=(int);
        bool IsThere(int);

        std::set<int>::iterator begin();
        std::set<int>::iterator end();
        std::set<int>::const_iterator begin() const;
        std::set<int>::const_iterator end() const;
        std::set<int>::reverse_iterator rbegin() const;
        std::set<int>::reverse_iterator rend() const;

    private:
        std::set<int> m_set;
    };

    class KeyBindStation
    {
    public:
        class BindKey
        {
        private:
            std::vector<m3d::KeysSet> m_keys;
            int m_impulse;
        };

    public:
        KeyBindStation();
        ~KeyBindStation();
        void UnbindImpulse(int);
        void UnbindAll();
        void UnbindImpulseFromKeyset(int, KeysSet const&);
        int FindImpulseByLongestSetPossible(class m3d::KeysSet const&, int, KeysSet&);
        BindKey* GetBindByKey(KeysSet const&);
        void BindKeyToImpulse(KeysSet const&, int);
        BindKey* GetBindByImpulse(int);

    private:
        BindKey* FindImpulseBySet_r(KeysSet const&, int);

    private:
        std::vector<BindKey> m_bindings;
        KeysSet* m_ks;
        std::vector<BindKey>::iterator m_lastIndex;
        int m_longestComboLen;
    };
}
