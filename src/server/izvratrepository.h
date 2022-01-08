#pragma once
#include "geomrepository.h"
#include <math/bounds2d.h>

namespace ai
{
    class IzvratRepository :  public GeomRepository
    {
    public:
        virtual void Purge();
        std::vector<int,std::allocator<int> > const & GetCells() const ;
        virtual ~IzvratRepository();
        virtual int CanAddThingToPlace(GeomRepositoryItem const &,PointBase<int> const &,int *) const ;
        virtual m3d::Object * Clone();
        static m3d::Class* GetBaseClass();
        virtual bool AddThingToPlace(GeomRepositoryItem &,PointBase<int> const &);
        static m3d::Object * CreateObject();
        virtual bool SetGeomSize(PointBase<int> const &);
        BoundsBase<int> ToMaxSzRelative(BoundsBase<int>) const ;
        PointBase<int> ToMaxSzRelative(PointBase<int>) const ;
        int SnapPiece(BoundsBase<int> const &);
        void SetMaxGeomSize(PointBase<int> const &);
        virtual m3d::Class* GetClass() const ;
        virtual void Clear(bool);
        IzvratRepository & operator=(IzvratRepository const &);
        PointBase<int> const & GetMaxGeomSize() const ;
        PointBase<int> ToGeomSzRelative(PointBase<int>) const ;
        BoundsBase<int> ToGeomSzRelative(BoundsBase<int>) const ;

    protected:
        IzvratRepository(IzvratRepository const &);
        IzvratRepository();
        int GetUnifyCellValueFromPiece(BoundsBase<int> const &,int) const ;
        void MarkCellPieceByValue(int,BoundsBase<int> const &);
        void MarkCellPieceByValueExcluding(int,BoundsBase<int> const &,int);
        bool IsValueAnItemId(int) const ;

    private:
        PointBase<int> m_maxGeomSize;
        std::vector<int> m_cells;
    };
}
