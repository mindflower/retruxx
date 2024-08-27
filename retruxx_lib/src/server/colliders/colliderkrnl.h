#pragma once
#include "ode/contact.h"

namespace m3d
{
	struct Class;
	class Object;
}

namespace ai
{
    class ColliderEntry
    {
    public:
        ColliderEntry(int (*)(m3d::Object *,m3d::Object *,dContact *,unsigned int &,bool),int);

    public:
        int (*fn)(m3d::Object *, m3d::Object *, dContact *, unsigned int &, bool);
        int reverse;
    };

    class ColliderKrnl
    {
    public:
        static int Init();
        static void Release();
        static int CollideObjs(m3d::Object*, m3d::Object*, dContact*, unsigned int&);
        static bool MustCheckForCollision(m3d::Object*, m3d::Object*);
        static void RegisterCollider(m3d::Class*, m3d::Class*, int(*)(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool));
    };
}
