#pragma once
#include "geomrepository.h"
#include <math/bounds2d.h>

namespace ai
{
    class Obj;

    class GeomRepositoryItem
    {
    public:
        enum GeomRepositoryItemType
        {
            ITEMTYPE_RESOURCE = 0x0,
            ITEMTYPE_OBJECT = 0x1,
        };

    public:
        int GetObjId() const ;
        void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        PointBase<int> GetGeomSize() const ;
        unsigned int GetMaxAmount() const ;
        int GetPrototypeId() const ;
        GeomRepositoryItem(int);
        GeomRepositoryItem(int,unsigned int);
        GeomRepositoryItem();
        GeomRepositoryItem(GeomRepositoryItem const &);
        CStr GetIcoName() const ;
        void Clear(bool);
        BoundsBase<int> GetBounds() const ;
        unsigned int GetAmount() const ;
        int GetResourceId() const ;
        void Invalidate();
        CStr GetDebugDescription() const ;
        bool IsValid() const ;
        void AddToRepository(GeomRepository *,PointBase<int> const &);
        bool IsComplete() const ;
        void RemoveObj(bool);
        Obj * GetObj() const ;
        unsigned int AcceptItem(GeomRepositoryItem const &);
        unsigned int GetAcceptedNum(GeomRepositoryItem const &) const ;
        unsigned int GiveUpAmount(unsigned int);
        void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        GeomRepositoryItem GetConvertedToObjectItem() const ;

    private:
        void _Init(int,int);
        void _Init(int);
        void _Init();

    private:
        int m_resourceId;
        int m_objId;
        unsigned int m_amount;
        GeomRepositoryItemType m_repositoryItemType;
        GeomRepository *m_parentRepository;
        PointBase<int> m_origin;
    };
}
