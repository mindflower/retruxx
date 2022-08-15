#include "colliderkrnl.h"
#include <map>
#include <stdexcept>
#include "core/kernel.h"

namespace ai
{
	namespace
	{
		std::map<unsigned, ColliderEntry>* g_collidersMap = nullptr;
		m3d::Class** g_allClasses = nullptr;
		unsigned g_numClasses = 0;

		void RegisterColliderForAllDescendants(m3d::Class *,m3d::Class *,int (*)(m3d::Object *,m3d::Object *,dContact *,unsigned int &,bool))
		{
			throw std::logic_error("Not implemented");
		}
	}

	ColliderEntry::ColliderEntry(int(*)(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool), int)
	{
		throw std::logic_error("Not implemented");
	}

	int ColliderKrnl::Init()
	{
		g_collidersMap = new std::map<unsigned, ColliderEntry>;
		m3d::g_Kernel->GetListOfClasses(g_allClasses, g_numClasses);
		return 1;
	}

	void ColliderKrnl::Release()
	{
		throw std::logic_error("Not implemented");
	}

	int ColliderKrnl::CollideObjs(m3d::Object*, m3d::Object*, dContact*, unsigned&)
	{
		throw std::logic_error("Not implemented");
	}

	bool ColliderKrnl::MustCheckForCollision(m3d::Object*, m3d::Object*)
	{
		throw std::logic_error("Not implemented");
	}

	void ColliderKrnl::RegisterCollider(m3d::Class* c1, m3d::Class* c2,
		int(*fn)(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool))
	{
		RegisterColliderForAllDescendants(c1, c2, fn);
	}
}
