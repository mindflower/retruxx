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
        WorkshopPriceCoeffProvider(ai::WorkshopPriceCoeffProvider const&);
        WorkshopPriceCoeffProvider(ai::Workshop const* workshop);
        virtual float GetPriceCoeffForObj(ai::Obj const* obj) const override;

    private:
        ai::Workshop const* m_workshop;
    };

    class WorkshopPrototypeInfo : public ai::BuildingPrototypeInfo
    {
    public:
        WorkshopPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

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
        friend class WorkshopPrototypeInfo;

    protected:
        virtual ~Workshop() override;

    private:
        Workshop(ai::WorkshopPrototypeInfo const& prototypeInfo);
        Workshop(ai::Workshop const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classWorkshop;
        virtual ai::WorkshopPrototypeInfo const* GetPrototypeInfo() const override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void Remove() override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        ai::GeomRepository* GetRepositoryByType(int type) const;
        ai::GeomRepository* GetRepositoryByType(ai::WorkshopRepositoryType type) const;
        ai::GeomRepository* GetRepositoryByTypename(CStr const& type) const;
        void GeneratePricesForVehiclePartsRepositories();
        void ApplyRandomizerToRepository(ai::WorkshopRepositoryType, float, float);
        void GenerateGunRepository(int GunGeneratorId, unsigned int DesiredGunsInWorkshop);
        void ApplyAffixGeneratorToRepository(
            ai::WorkshopRepositoryType type,
            int AffixGeneratorPrototypeId,
            unsigned int count,
            int itemsResourceId);
        unsigned int GetArticleSellPriceByObj(ai::Obj const* obj) const;
        unsigned int GetArticleSellPriceByPrototypeId(int articleProtoId) const;
        unsigned int GetArticleBuyPriceByObj(ai::Obj const* article) const;
        unsigned int GetArticleBuyPriceByPrototypeId(int articleProtoId) const;
        bool BuyArticle(int wareObjId);
        bool SellArticle(int warePrototypeId, unsigned int amount);
        ai::Article* GetArticle(int articlePrototypeId);
        ai::Article const* GetArticle(int articlePrototypeId) const;
        void AddArticle(ai::Article const& newArticle) const;
        void AddArticle(int articlePrototypeId) const;
        unsigned int GetArticleAmount(int pId);
        unsigned int GetObjectBuyPrice(ai::Obj const* obj) const;
        unsigned int GetObjectSellPrice(ai::Obj const* obj) const;
        bool BuyObject(ai::Obj const* obj);
        bool SellObject(ai::Obj const* obj);
        unsigned int GetObjectRepairPrice(ai::Obj const* obj) const;
        float GetHealthPriceForOneUnit() const;
        float GetRepairPriceForOneUnit(ai::Obj const* obj) const;
        static ai::WorkshopRepositoryType GetRepositoryTypeByResourceId(int resId);
        void OnSaveVisitedMap();

        using RepositoryTypeMap = retruxx::map<
            enum ai::WorkshopRepositoryType,
            ai::GeomRepository*,
            retruxx::less<enum ai::WorkshopRepositoryType>,
            retruxx::allocator<retruxx::pair<enum ai::WorkshopRepositoryType const, ai::GeomRepository*>>>;

    private:
        retruxx::map<
            enum ai::WorkshopRepositoryType,
            ai::GeomRepository*,
            retruxx::less<enum ai::WorkshopRepositoryType>,
            retruxx::allocator<retruxx::pair<enum ai::WorkshopRepositoryType const, ai::GeomRepository*>>>
            m_repositories;
        // The two AddArticle overloads are const but put a missing article on the books, as the shipped
        // code does, so the map has to be mutable.
        mutable retruxx::
            map<int, ai::Article, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, ai::Article>>>
                m_articles;
        ai::WorkshopPriceCoeffProvider* m_priceCoeffProvider;
        float _GetRealObjectResourceCoeff(ai::Obj const* obj) const;
        unsigned int _GetRealObjectBuyPrice(ai::Obj const* obj) const;
        unsigned int _GetRealObjectSellPrice(ai::Obj const* obj) const;
        bool _BuyRealObject(ai::Obj const* obj);
        bool _SellRealObject(ai::Obj const* obj);
        void _GetArticles(retruxx::vector<ai::Article, retruxx::allocator<ai::Article>>& articles) const;
        bool _ArticleExists(int articlePrototypeId) const;
        static int _GetWarePrototypeId(int wareObjId);
        retruxx::vector<int, retruxx::allocator<int>> m_originalObjectsInRepository;
        void ClearRepositoriesFromNonOriginalObjects();
    };
}  // namespace ai
