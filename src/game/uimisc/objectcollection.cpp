#include "objectcollection.h"
#include <stdexcept>

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
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}
