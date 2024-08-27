#pragma once
#include <vector>
#include <math/vector2.h>

namespace ai
{

    class GMAPOFFSET
    {
    public:
        GMAPOFFSET(int, int);

    private:
        int x;
        int y;
    };

    class OffsetSet
    {
    private:
        std::vector<ai::GMAPOFFSET> m_Off;
    };

    class DirSet
    {
    public:
        void AddItem(bool, int, int);

    private:
        OffsetSet m_Dir[8];
    };

    class MapIndex
    {
    public:
        MapIndex(int, int);

    public:
        int x;
        int y;
    };

    class Map
    {
    public:
        void DecCircle(CVector2 const &,float,unsigned char);
        void DecLine4(CVector2 const &,CVector2 const &,unsigned char);
        void DecLine8(CVector2 const &,CVector2 const &,unsigned char);
        void GetIndex(CVector2 const &,MapIndex &);
        void Create(float,float,float,float,Map *);
        bool IsBlocked(int,int,int,int,unsigned char);
        void IncLineTo(CVector2 const &,unsigned char);
        bool IsLine4Blocked(int,int,int,int,unsigned int);
        void IncValue(CVector2 const &,unsigned char);
        void IncValue(int,int,unsigned char);
        void LineTo(CVector2 const &,unsigned char);
        void LoadFromRawFile(char const *);
        void Line4(CVector2 const &,CVector2 const &,unsigned char);
        static void __fastcall SetGlobalMap(Map *);
        CVector2 const & GetCellSize() const ;
        void DecRectangle(CVector2 const &,CVector2 const &,unsigned char);
        void Clear();
        bool IsSolidBlocked(CVector2 const &,CVector2 const &,float,unsigned char);
        bool IsSolidBlocked(MapIndex const &,MapIndex const &,int,unsigned char);
        bool IsLine8Blocked(int,int,int,int,unsigned int);
        void SetValue(int,int,unsigned char);
        void SetValue(CVector2 const &,unsigned char);
        void DecValue(int,int,unsigned char);
        void DecValue(CVector2 const &,unsigned char);
        void Rectangle(CVector2 const &,CVector2 const &,unsigned char);
        void Fill(unsigned char);
        void Line8(CVector2 const &,CVector2 const &,unsigned char);
        void IncRectangle(CVector2 const &,CVector2 const &,unsigned char);
        void GetCellCenter(CVector2 const &,CVector2 &);
        void GetCellCenter(MapIndex const &,CVector2 &);
        void MoveTo(CVector2 const &);
        ~Map();
        void IncCircle(CVector2 const &,float,unsigned char);
        void IncLine8(CVector2 const &,CVector2 const &,unsigned char);
        void IncLine4(CVector2 const &,CVector2 const &,unsigned char);
        unsigned char GetValue(int,int);
        unsigned char GetValue(float,float);
        MapIndex const & GetLastIndex() const ;
        void Circle(CVector2 const &,float,unsigned char);
        Map();
        bool IsCircleBlocked(MapIndex const &,int,unsigned char);
        bool IsCircleBlocked(CVector2 const &,float,unsigned char);
        void DecLineTo(CVector2 const &,unsigned char);

    public:
        static inline Map* theGlobalMap = nullptr;

    private:
        CVector2 m_cellSize;
        CVector2 m_size;
        MapIndex m_lastIndex;
        MapIndex m_CurPos;
        std::vector<DirSet> m_DirSet;
        Map *m_pParentMap;
        unsigned __int8 *pField = nullptr;
    };
}
