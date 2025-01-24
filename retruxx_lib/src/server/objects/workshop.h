#pragma once
#include "building.h"
#include <server/ipricecoeffprovider.h>

namespace ai
{
    class Article;
    class Workshop;

    class WorkshopPriceCoeffProvider : public ai::IPriceCoeffProvider
    {
    public:
        WorkshopPriceCoeffProvider(const ai::WorkshopPriceCoeffProvider&);
        WorkshopPriceCoeffProvider(const ai::Workshop* workshop);
        virtual float GetPriceCoeffForObj(const ai::Obj* obj) const override /* 0x04 */;

    private:
        /* 0x0004 */ const ai::Workshop* m_workshop;
    }; /* size: 0x0008 */

    static_assert(sizeof(WorkshopPriceCoeffProvider) == 0x0008);

    class WorkshopPrototypeInfo : public ai::BuildingPrototypeInfo
    {
    public:
        WorkshopPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
    }; /* size: 0x0044 */

    static_assert(sizeof(WorkshopPrototypeInfo) == 0x0044);

    enum WorkshopRepositoryType
    {
        WORKSHOP_GOODS = 0x0,
        WORKSHOP_CABINS_AND_BASKETS = 0x1,
        WORKSHOP_VEHICLES = 0x2,
        WORKSHOP_GUNS_AND_GADGETS = 0x3,
        WORKSHOP_NUM_TYPES = 0x4,
    };

    class Workshop : public ai::Building
    {
    protected:
        virtual  ~Workshop() override /* 0x00 */;

    private:
        Workshop(const ai::WorkshopPrototypeInfo& prototypeInfo);
        Workshop(const ai::Workshop&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classWorkshop;
        virtual const ai::WorkshopPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void Remove() override /* 0x54 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x94 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        ai::GeomRepository* GetRepositoryByType(int type) const;
        ai::GeomRepository* GetRepositoryByType(ai::WorkshopRepositoryType type) const;
        ai::GeomRepository* GetRepositoryByTypename(const CStr& type) const;
        void GeneratePricesForVehiclePartsRepositories();
        void ApplyRandomizerToRepository(ai::WorkshopRepositoryType, float, float);
        void GenerateGunRepository(int GunGeneratorId, unsigned int DesiredGunsInWorkshop);
        void ApplyAffixGeneratorToRepository(ai::WorkshopRepositoryType type, int AffixGeneratorPrototypeId, unsigned int count, int itemsResourceId);
        unsigned int GetArticleSellPriceByObj(const ai::Obj* obj) const;
        unsigned int GetArticleSellPriceByPrototypeId(int articleProtoId) const;
        unsigned int GetArticleBuyPriceByObj(const ai::Obj* article) const;
        unsigned int GetArticleBuyPriceByPrototypeId(int articleProtoId) const;
        bool BuyArticle(int wareObjId);
        bool SellArticle(int warePrototypeId, unsigned int amount);
        ai::Article* GetArticle(int articlePrototypeId);
        const ai::Article* GetArticle(int articlePrototypeId) const;
        void AddArticle(const ai::Article& newArticle) const;
        void AddArticle(int articlePrototypeId) const;
        unsigned int GetArticleAmount(int pId);
        unsigned int GetObjectBuyPrice(const ai::Obj* obj) const;
        unsigned int GetObjectSellPrice(const ai::Obj* obj) const;
        bool BuyObject(const ai::Obj* obj);
        bool SellObject(const ai::Obj* obj);
        unsigned int GetObjectRepairPrice(const ai::Obj* obj) const;
        float GetHealthPriceForOneUnit() const;
        float GetRepairPriceForOneUnit(const ai::Obj* obj) const;
        static ai::WorkshopRepositoryType __fastcall GetRepositoryTypeByResourceId(int resId);
        void OnSaveVisitedMap();

        using RepositoryTypeMap = retruxx::map<enum ai::WorkshopRepositoryType, ai::GeomRepository*, retruxx::less<enum ai::WorkshopRepositoryType>, retruxx::allocator<retruxx::pair<enum ai::WorkshopRepositoryType const, ai::GeomRepository*> > >;

    private:
        /* 0x00d0 */ retruxx::map<enum ai::WorkshopRepositoryType, ai::GeomRepository*, retruxx::less<enum ai::WorkshopRepositoryType>, retruxx::allocator<retruxx::pair<enum ai::WorkshopRepositoryType const, ai::GeomRepository*> > > m_repositories;
        /* 0x00dc */ retruxx::map<int, ai::Article, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, ai::Article> > > m_articles;
        /* 0x00e8 */ ai::WorkshopPriceCoeffProvider* m_priceCoeffProvider;
        float _GetRealObjectResourceCoeff(const ai::Obj* obj) const;
        unsigned int _GetRealObjectBuyPrice(const ai::Obj* obj) const;
        unsigned int _GetRealObjectSellPrice(const ai::Obj* obj) const;
        bool _BuyRealObject(const ai::Obj* obj);
        bool _SellRealObject(const ai::Obj* obj);
        void _GetArticles(retruxx::vector<ai::Article, retruxx::allocator<ai::Article> >& articles) const;
        bool _ArticleExists(int articlePrototypeId) const;
        static int __fastcall _GetWarePrototypeId(int wareObjId);
        /* 0x00ec */ retruxx::vector<int, retruxx::allocator<int> > m_originalObjectsInRepository;
        void ClearRepositoriesFromNonOriginalObjects();
    }; /* size: 0x00fc */

    static_assert(sizeof(Workshop) == 0x00fc);
}
