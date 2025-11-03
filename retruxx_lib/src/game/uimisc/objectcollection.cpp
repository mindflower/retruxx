#include "objectcollection.h"
#include <stdexcept>
#include <core/kernel.h>
#include <core/stringm3d.h>

#include "core/clazz.h"
#include "server/objects/base/obj.h"

ObjectCollection::~ObjectCollection()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ObjectCollection::RemoveObject(ai::Obj*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

std::set<int> const* ObjectCollection::GetObjectsByClass(m3d::Class const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
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

    for (const auto& colCl : m_collectedClasses)
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
    RETRUXX_NOT_IMPLEMENTED;
}

void ObjectCollection::InitClasses()
{
    m_collectedClasses.clear();
    CStr collectedClassNames[] = { "Vehicle", "DynamicQuest", "Town", "Lair", "Location", "StaticAutoGun" };
    for (auto const& className : collectedClassNames)
    {
        if (auto* cls = m3d::g_Kernel->FindClass(className.c_str()))
        {
            m_collectedClasses.push_back(cls);
        }
    }
}
