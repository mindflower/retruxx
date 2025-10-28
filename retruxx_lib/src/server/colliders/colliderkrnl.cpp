#include "colliderkrnl.h"
#include <map>
#include <stdexcept>

#include "defaultcollider.h"
#include "core/kernel.h"
#include "core/clazz.h"
#include "server/objects/base/obj.h"

namespace ai
{
	namespace
	{
		std::map<unsigned, ColliderEntry>* g_collidersMap = nullptr;
		m3d::Class** g_allClasses = nullptr;
		unsigned g_numClasses = 0;

		ColliderEntry const& GetCollider(m3d::Object* obj1, m3d::Object* obj2)
		{
			static const ColliderEntry emptyCollider{ ai::EmptyCollider, 0 };
			static const ColliderEntry defaultCollider{ ai::DefaultCollider, 0 };

			unsigned index1 = 0xFFFF;
			unsigned index2 = 0xFFFF;

			if (obj1)
			{
				index1 = obj1->GetClass()->m_index;
				if (IS_KIND_OF(obj1, Obj) && ((ai::Obj*)obj1)->GetPassedToAnotherMapStatus())
				{
					return emptyCollider;
				}
			}

			if (obj2)
			{
				index2 = obj2->GetClass()->m_index;
				if (IS_KIND_OF(obj2, Obj) && ((ai::Obj*)obj2)->GetPassedToAnotherMapStatus())
				{
					return emptyCollider;
				}
			}

			unsigned merged = index1 | (index2 << 16);
			auto it = g_collidersMap->find(merged);
			if (it != g_collidersMap->end())
			{
				it->second;
			}

			return defaultCollider;
		}

		void RegisterColliderForClassesOnly(m3d::Class *c1,m3d::Class *c2,int (*fn)(m3d::Object *,m3d::Object *,dContact *,unsigned int &,bool))
		{
			int v3; // ecx
			int v4; // eax
			int v5; // edx
			unsigned int v6; // ecx
			int v7; // eax
			ai::ColliderEntry* v8; // eax
			ai::ColliderEntry* v9; // eax
			unsigned int merged; // [esp+0h] [ebp-8h] BYREF
			unsigned int mergedRev; // [esp+4h] [ebp-4h] BYREF

			if (c1)
			{
				v3 = c1->m_index;
				v4 = v3 << 16;
			}
			else
			{
				v3 = 0xFFFF;
				v4 = -65536;
			}
			if (c2)
			{
				v5 = c2->m_index;
				v6 = (v5 << 16) | v3;
				v7 = v5 | v4;
			}
			else
			{
				v6 = v3 | 0xFFFF0000;
				v7 = v4 | 0xFFFF;
			}
			mergedRev = v7;
			merged = v6;

			g_collidersMap->emplace(merged, ColliderEntry{ fn, 0 });
			g_collidersMap->emplace(mergedRev, ColliderEntry{ fn, 1 });
		}

		void RegisterColliderForAllDescendants(m3d::Class *c1,m3d::Class *c2,int (*fn)(m3d::Object *,m3d::Object *,dContact *,unsigned int &,bool))
		{
			auto clazz = c1;
			if (c1)
			{
				if (c2)
				{
					for (int i = 0; i < g_numClasses; ++i )
					{
						if (g_allClasses[i]->IsKindOf(c1) )
						{
							for (int j = 0; j < g_numClasses; ++j )
							{
								if (g_allClasses[j]->IsKindOf(c2) )
									RegisterColliderForClassesOnly(g_allClasses[i], g_allClasses[j], fn);
							}
						}
					}
					return;
				}
			}
			else
			{
				if (!c2)
				{
					RegisterColliderForClassesOnly(0, 0, fn);
						return;
				}
				clazz = c2;
			}
			for (int k = 0; k < g_numClasses; ++k )
			{
				if (g_allClasses[k]->IsKindOf(clazz) )
					RegisterColliderForClassesOnly(g_allClasses[k], 0, fn);
			}
		}
	}

	ColliderEntry::ColliderEntry(int(*f)(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool), int r)
		: fn(f), reverse(r)
	{
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

	bool ColliderKrnl::MustCheckForCollision(m3d::Object* obj1, m3d::Object* obj2)
	{
		return GetCollider(obj1, obj2).fn != ai::EmptyCollider;
	}

	void ColliderKrnl::RegisterCollider(m3d::Class* c1, m3d::Class* c2,
		int(*fn)(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool))
	{
		RegisterColliderForAllDescendants(c1, c2, fn);
	}
}
