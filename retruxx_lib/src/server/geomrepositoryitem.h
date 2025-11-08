#pragma once
#include "geomrepository.h"
#include <math/bounds2d.h>

namespace ai
{
    class Obj;

    struct GeomRepositoryItem
    {
        enum GeomRepositoryItemType
        {
            ITEMTYPE_RESOURCE = 0,
            ITEMTYPE_OBJECT = 1,
        };

        /* 0x0000 */ int m_resourceId;
        /* 0x0004 */ int m_objId;
        /* 0x0008 */ unsigned int m_amount;
        /* 0x000c */ ai::GeomRepositoryItem::GeomRepositoryItemType m_repositoryItemType;
        /* 0x0010 */ ai::GeomRepository* m_parentRepository;
        /* 0x0014 */ PointBase<int> m_origin;
        GeomRepositoryItem(const ai::GeomRepositoryItem& rhs);
        GeomRepositoryItem(int resourceId, unsigned int amount);
        GeomRepositoryItem(int objId);
        GeomRepositoryItem();
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        PointBase<int> GetGeomSize() const;
        BoundsBase<int> GetBounds() const;
        unsigned int GetMaxAmount() const;
        unsigned int GiveUpAmount(unsigned int amount);
        bool IsComplete() const;
        void AddToRepository(ai::GeomRepository* parent, const PointBase<int>& origin);
        bool IsValid() const;
        void Invalidate();
        CStr GetIcoName() const;
        CStr GetDebugDescription() const;
        bool bIsResourceItem() const;
        unsigned int GetAcceptedNum(const ai::GeomRepositoryItem& item) const;
        unsigned int AcceptItem(const ai::GeomRepositoryItem& item);
        unsigned int GetAmount() const;
        ai::Obj* GetObj() const;
        int GetObjId() const;
        int GetResourceId() const;
        int GetPrototypeId() const;
        ai::GeomRepositoryItem GetConvertedToObjectItem() const;
        void Clear(bool bUnsafe);
        void RemoveObj(bool bUnsafe);
        void _Init(int resourceId, int amount);
        void _Init(int objId);
        void _Init();
    }; /* size: 0x001c */
}
