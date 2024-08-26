#include "objectcollection.h"
#include <stdexcept>
#include <core/kernel.h>
#include <core/stringm3d.h>

ObjectCollection::~ObjectCollection()
{
    throw std::logic_error("Not implemented");
}

int ObjectCollection::RemoveObject(ai::Obj*)
{
    throw std::logic_error("Not implemented");
}

std::set<int> const* ObjectCollection::GetObjectsByClass(m3d::Class const*) const
{
    throw std::logic_error("Not implemented");
}

ObjectCollection::ObjectCollection()
{
    InitClasses();
}

bool ObjectCollection::IsClassCollected(m3d::Class const*) const
{
    throw std::logic_error("Not implemented");
}

void ObjectCollection::ClearObjects()
{
    throw std::logic_error("Not implemented");
}

int ObjectCollection::AddObject(ai::Obj*)
{
    throw std::logic_error("Not implemented");
}

void ObjectCollection::ClearClasses()
{
    throw std::logic_error("Not implemented");
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
