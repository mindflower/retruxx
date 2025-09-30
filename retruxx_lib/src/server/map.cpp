#include "map.h"

#include <stdexcept>

#include "core/log.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"

namespace ai
{
    MapIndex::MapIndex(int xx, int yy) :
        x(xx),
        y(yy)
    {
    }

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

    void Map::Create(float cellSizeX, float cellSizeY, float sizeX, float sizeY, Map* pParentMap)
    {
        this->m_size.x = sizeX;
        this->m_lastIndex.x = (sizeX / cellSizeX);
        this->m_size.y = sizeY;
        this->m_cellSize.x = cellSizeX;
        this->m_cellSize.y = cellSizeY;
        this->m_pParentMap = pParentMap;
        this->m_lastIndex.y = (sizeY / cellSizeY);
        if (pField)
            delete[] pField;
        this->pField = 0;
        auto size = this->m_lastIndex.y * this->m_lastIndex.x;
        this->pField = new unsigned char[size];
        memset(pField, 0, size);
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

    void Map::LoadFromRawFile(char const* fileName)
    {
        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (stream->Open(fileName, m3d::fs::IStream::OPEN_READ))
        {
            stream->ReadBytes(pField, this->m_lastIndex.x * this->m_lastIndex.y);
            stream->Close();
        }
    }

    void Map::Line4(CVector2 const&, CVector2 const&, unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    void Map::SetGlobalMap(Map* pGlobalMap)
    {
        theGlobalMap = pGlobalMap;
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
        if (pField)
        {
            memset(pField, 0, this->m_lastIndex.x * this->m_lastIndex.y);
        }
        else
        {
            M3D_LOG_ERR("Error: map field was not created");
        }
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

    Map::Map() :
        m_lastIndex(0, 0),
        m_CurPos(0, 0)
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
