#include "geomrepositoryitem.h"
#include <stdexcept>

int ai::GeomRepositoryItem::GetObjId() const
{
    throw std::logic_error("Not implemented");
}

void ai::GeomRepositoryItem::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw std::logic_error("Not implemented");
}

PointBase<int> ai::GeomRepositoryItem::GetGeomSize() const
{
    throw std::logic_error("Not implemented");
}

unsigned ai::GeomRepositoryItem::GetMaxAmount() const
{
    throw std::logic_error("Not implemented");
}

int ai::GeomRepositoryItem::GetPrototypeId() const
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepositoryItem::GeomRepositoryItem(int)
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepositoryItem::GeomRepositoryItem(int, unsigned)
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepositoryItem::GeomRepositoryItem()
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepositoryItem::GeomRepositoryItem(GeomRepositoryItem const&)
{
    throw std::logic_error("Not implemented");
}

CStr ai::GeomRepositoryItem::GetIcoName() const
{
    throw std::logic_error("Not implemented");
}

void ai::GeomRepositoryItem::Clear(bool)
{
    throw std::logic_error("Not implemented");
}

BoundsBase<int> ai::GeomRepositoryItem::GetBounds() const
{
    throw std::logic_error("Not implemented");
}

unsigned ai::GeomRepositoryItem::GetAmount() const
{
    throw std::logic_error("Not implemented");
}

int ai::GeomRepositoryItem::GetResourceId() const
{
    throw std::logic_error("Not implemented");
}

void ai::GeomRepositoryItem::Invalidate()
{
    throw std::logic_error("Not implemented");
}

CStr ai::GeomRepositoryItem::GetDebugDescription() const
{
    throw std::logic_error("Not implemented");
}

bool ai::GeomRepositoryItem::IsValid() const
{
    throw std::logic_error("Not implemented");
}

void ai::GeomRepositoryItem::AddToRepository(GeomRepository*, PointBase<int> const&)
{
    throw std::logic_error("Not implemented");
}

bool ai::GeomRepositoryItem::IsComplete() const
{
    throw std::logic_error("Not implemented");
}

void ai::GeomRepositoryItem::RemoveObj(bool)
{
    throw std::logic_error("Not implemented");
}

ai::Obj* ai::GeomRepositoryItem::GetObj() const
{
    throw std::logic_error("Not implemented");
}

unsigned ai::GeomRepositoryItem::AcceptItem(GeomRepositoryItem const&)
{
    throw std::logic_error("Not implemented");
}

unsigned ai::GeomRepositoryItem::GetAcceptedNum(GeomRepositoryItem const&) const
{
    throw std::logic_error("Not implemented");
}

unsigned ai::GeomRepositoryItem::GiveUpAmount(unsigned)
{
    throw std::logic_error("Not implemented");
}

void ai::GeomRepositoryItem::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepositoryItem ai::GeomRepositoryItem::GetConvertedToObjectItem() const
{
    throw std::logic_error("Not implemented");
}

void ai::GeomRepositoryItem::_Init(int, int)
{
    throw std::logic_error("Not implemented");
}

void ai::GeomRepositoryItem::_Init(int)
{
    throw std::logic_error("Not implemented");
}

void ai::GeomRepositoryItem::_Init()
{
    throw std::logic_error("Not implemented");
}
