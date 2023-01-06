#include "map.h"

#include <stdexcept>

namespace ai
{
    void Map::DecCircle(CVector2 const&, float, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::DecLine4(CVector2 const&, CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::DecLine8(CVector2 const&, CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::GetIndex(CVector2 const&, MapIndex&)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::Create(float, float, float, float, Map*)
    {
        throw std::logic_error("Not implemented");
    }

    bool Map::IsBlocked(int, int, int, int, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::IncLineTo(CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    bool Map::IsLine4Blocked(int, int, int, int, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::IncValue(CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::IncValue(int, int, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::LineTo(CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::LoadFromRawFile(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::Line4(CVector2 const&, CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::SetGlobalMap(Map*)
    {
        throw std::logic_error("Not implemented");
    }

    CVector2 const& Map::GetCellSize() const
    {
        throw std::logic_error("Not implemented");
    }

    void Map::DecRectangle(CVector2 const&, CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::Clear()
    {
        throw std::logic_error("Not implemented");
    }

    bool Map::IsSolidBlocked(CVector2 const&, CVector2 const&, float, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    bool Map::IsSolidBlocked(MapIndex const&, MapIndex const&, int, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    bool Map::IsLine8Blocked(int, int, int, int, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::SetValue(int, int, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::SetValue(CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::DecValue(int, int, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::DecValue(CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::Rectangle(CVector2 const&, CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::Fill(unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::Line8(CVector2 const&, CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::IncRectangle(CVector2 const&, CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::GetCellCenter(CVector2 const&, CVector2&)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::GetCellCenter(MapIndex const&, CVector2&)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::MoveTo(CVector2 const&)
    {
        throw std::logic_error("Not implemented");
    }

    Map::~Map()
    {
        throw std::logic_error("Not implemented");
    }

    void Map::IncCircle(CVector2 const&, float, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::IncLine8(CVector2 const&, CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::IncLine4(CVector2 const&, CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned char Map::GetValue(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned char Map::GetValue(float, float)
    {
        throw std::logic_error("Not implemented");
    }

    MapIndex const& Map::GetLastIndex() const
    {
        throw std::logic_error("Not implemented");
    }

    void Map::Circle(CVector2 const&, float, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    Map::Map()
    {
        throw std::logic_error("Not implemented");
    }

    bool Map::IsCircleBlocked(MapIndex const&, int, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    bool Map::IsCircleBlocked(CVector2 const&, float, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::DecLineTo(CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }
}
