#include "objectcollection.h"
#include <stdexcept>
#include <core/kernel.h>
#include <core/stringm3d.h>

#include "core/clazz.h"
#include "server/objects/base/obj.h"

ObjectCollection::~ObjectCollection() = default;

int ObjectCollection::RemoveObject(ai::Obj* obj)
{
    if (!obj || !obj->GetClass())
    {
        return 0;
    }

    for (auto const* cls = obj->GetClass(); IsClassCollected(cls); cls = cls->m_fnGetBaseClass())
    {
        auto it = m_objects.find(cls);
        if (it != m_objects.end())
        {
            return it->second.erase(obj->GetId()) != 0;
        }
    }
    return 1;
}

std::set<int> const* ObjectCollection::GetObjectsByClass(m3d::Class const* cl) const
{
    if (!cl)
    {
        return nullptr;
    }

    auto it = m_objects.find(cl);
    if (it == m_objects.end())
    {
        return nullptr;
    }
    return &it->second;
}

ObjectCollection::ObjectCollection()
{
    InitClasses();
}

bool ObjectCollection::IsClassCollected(m3d::Class const* cl) const
{
    if (!cl)
    {
        return false;
    }

    for (auto const& colCl : m_collectedClasses)
    {
        if (cl->IsKindOf(colCl))
        {
            return true;
        }
    }
    return false;
}

void ObjectCollection::ClearObjects()
{
    m_objects.clear();
}

int ObjectCollection::AddObject(ai::Obj* obj)
{
    if (!obj || !obj->GetClass())
    {
        return 0;
    }

    for (auto cls = obj->GetClass(); IsClassCollected(cls); cls = cls->m_fnGetBaseClass())
    {
        m_objects[cls].insert(obj->GetId());
    }

    return 1;
}

void ObjectCollection::ClearClasses()
{
    m_collectedClasses.clear();
}

void ObjectCollection::InitClasses()
{
    m_collectedClasses.clear();
    CStr collectedClassNames[] = {"Vehicle", "DynamicQuest", "Town", "Lair", "Location", "StaticAutoGun"};
    for (auto const& className : collectedClassNames)
    {
        if (auto* cls = m3d::g_Kernel->FindClass(className.c_str()))
        {
            m_collectedClasses.push_back(cls);
        }
    }
}
