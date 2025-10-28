#pragma once
#include "ode/contact.h"

namespace m3d
{
	struct Class;
	class Object;
}

namespace ai
{
    struct ColliderEntry
    {
        /* 0x0000 */ int (*fn)(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
        /* 0x0004 */ int reverse;
        ColliderEntry(int (*f)(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool), int r);
    }; /* size: 0x0008 */

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
