#pragma once

namespace m3d
{
    class Object;
}

namespace ai
{
    class ColliderEntry
    {
    public:
        ColliderEntry(int (__fastcall*)(class m3d::Object *,class m3d::Object *,struct dContact *,unsigned int &,bool),int);

    private:
        int (__fastcall *fn)(m3d::Object *, m3d::Object *, dContact *, unsigned int *, bool);
        int reverse;
    };
}
