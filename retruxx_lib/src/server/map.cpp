#include "map.h"

#include <stdexcept>

#include "core/log.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"

namespace ai
{
    GMAPOFFSET::GMAPOFFSET(int Px, int Py)
    {
        this->x = Px;
        this->y = Py;
    }

    void DirSet::AddItem(bool IsDiag, int x, int y)
    {
        int tempx = x;
        int tempy = y;
        if (IsDiag)
        {
            m_Dir[4].m_Off.emplace_back(tempx, tempy);
            tempx = -x;
            tempy = -y;
            m_Dir[6].m_Off.emplace_back(tempx, tempy);
            tempx = -x;
            tempy = y;
            m_Dir[5].m_Off.emplace_back(tempx, tempy);
            tempx = x;
            tempy = -y;
            m_Dir[7].m_Off.emplace_back(tempx, tempy);
        }
        else
        {
            m_Dir[0].m_Off.emplace_back(tempx, tempy);
            tempx = -x;
            tempy = y;
            m_Dir[2].m_Off.emplace_back(tempx, tempy);
            tempx = y;
            tempy = x;
            m_Dir[1].m_Off.emplace_back(tempx, tempy);
            tempx = y;
            tempy = -x;
            m_Dir[3].m_Off.emplace_back(tempx, tempy);
        }
    }

    MapIndex::MapIndex(int xx, int yy) :
        x(xx),
        y(yy)
    {
    }

    void Map::DecCircle(CVector2 const&, float, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::DecLine4(CVector2 const&, CVector2 const&, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::DecLine8(CVector2 const&, CVector2 const&, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::GetIndex(CVector2 const&, MapIndex&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::IncLineTo(CVector2 const&, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Map::IsLine4Blocked(int, int, int, int, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::IncValue(CVector2 const&, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::IncValue(int, int, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::LineTo(CVector2 const&, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::SetGlobalMap(Map* pGlobalMap)
    {
        theGlobalMap = pGlobalMap;
    }

    CVector2 const& Map::GetCellSize() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::DecRectangle(CVector2 const&, CVector2 const&, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Map::IsSolidBlocked(MapIndex const&, MapIndex const&, int, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Map::IsLine8Blocked(int, int, int, int, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::SetValue(int, int, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::SetValue(CVector2 const&, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::DecValue(int, int, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::DecValue(CVector2 const&, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::Rectangle(CVector2 const&, CVector2 const&, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::Fill(unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::Line8(CVector2 const&, CVector2 const&, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::IncRectangle(CVector2 const&, CVector2 const&, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::GetCellCenter(CVector2 const&, CVector2&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::GetCellCenter(MapIndex const&, CVector2&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::MoveTo(CVector2 const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Map::~Map()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::IncCircle(CVector2 const&, float, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::IncLine8(CVector2 const&, CVector2 const&, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::IncLine4(CVector2 const&, CVector2 const&, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned char Map::GetValue(int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned char Map::GetValue(float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    MapIndex const& Map::GetLastIndex() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::Circle(CVector2 const&, float, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Map::Map() :
        m_lastIndex(0, 0),
        m_CurPos(0, 0)
    {
        pField = 0;

        m_DirSet.resize(8);
        m_DirSet[0].AddItem(false, 0, 0);
        m_DirSet[0].AddItem(false, 1, -1);
        m_DirSet[0].AddItem(false, 1, 0);
        m_DirSet[0].AddItem(false, 1, 1);
        m_DirSet[0].AddItem(true, 0, 0);
        m_DirSet[0].AddItem(true, 0, 1);
        m_DirSet[0].AddItem(true, 1, 0);
        m_DirSet[0].AddItem(true, 1, 1);


        m_DirSet[1].AddItem(false, 0, 0);
        m_DirSet[1].AddItem(false, 2, 0);
        m_DirSet[1].AddItem(false, 2, 1);
        m_DirSet[1].AddItem(false, 2, -1);
        m_DirSet[1].AddItem(false, 1, -2);
        m_DirSet[1].AddItem(false, 1,2);
        m_DirSet[1].AddItem(false, 0, -2);
        m_DirSet[1].AddItem(false, 0, 2);
        m_DirSet[1].AddItem(false, 0, -2);
        m_DirSet[1].AddItem(true, 0, 0);
        m_DirSet[1].AddItem(true, 2, 1);
        m_DirSet[1].AddItem(true, 1, 2);
        m_DirSet[1].AddItem(true, 2, 0);
        m_DirSet[1].AddItem(true, 0, 2);
        m_DirSet[1].AddItem(true, 2, -1);
        m_DirSet[1].AddItem(true, -1, 2);


        m_DirSet[2].AddItem(false, 0, 0);
        m_DirSet[2].AddItem(false, 3, 0);
        m_DirSet[2].AddItem(false, 3, 1);
        m_DirSet[2].AddItem(false, 3, -1);
        m_DirSet[2].AddItem(false, 2, 2);
        m_DirSet[2].AddItem(false, 2, -2);
        m_DirSet[2].AddItem(false, 1, 3);
        m_DirSet[2].AddItem(false, 1, 3);
        m_DirSet[2].AddItem(false, 0, 3);
        m_DirSet[2].AddItem(false, 0, 3);
        m_DirSet[2].AddItem(false, 0, 0);
        m_DirSet[2].AddItem(true, 2, 2);
        m_DirSet[2].AddItem(true, 1, 3);
        m_DirSet[2].AddItem(true, 3, 1);
        m_DirSet[2].AddItem(true, 3, 0);
        m_DirSet[2].AddItem(true, 0, 3);
        m_DirSet[2].AddItem(true, -1, 3);
        m_DirSet[2].AddItem(true, 3, -1);
        m_DirSet[2].AddItem(true, -2, 2);
        m_DirSet[2].AddItem(true, -2, 2);
        m_DirSet[2].AddItem(true, 2, -2);

        // TODO: implement MAP::MAP
    }

    bool Map::IsCircleBlocked(MapIndex const&, int, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Map::IsCircleBlocked(CVector2 const&, float, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Map::DecLineTo(CVector2 const&, unsigned char)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
