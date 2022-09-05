#pragma once
#include "building.h"
#include <server/ipricecoeffprovider.h>

namespace ai
{
    class Article;
    class Workshop;

    class WorkshopPriceCoeffProvider : public IPriceCoeffProvider
    {
    public:
        WorkshopPriceCoeffProvider(Workshop const*);
        virtual float GetPriceCoeffForObj(Obj const*) const;

    private:
        const Workshop* m_workshop;
    };

    class WorkshopPrototypeInfo : public BuildingPrototypeInfo
    {
    public:
        WorkshopPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual ai::Obj* CreateTargetObject() const;
    };

    enum WorkshopRepositoryType
    {
        WORKSHOP_GOODS = 0x0,
        WORKSHOP_CABINS_AND_BASKETS = 0x1,
        WORKSHOP_VEHICLES = 0x2,
        WORKSHOP_GUNS_AND_GADGETS = 0x3,
        WORKSHOP_NUM_TYPES = 0x4,
    };

    class Workshop :  public Building
    {
    public:
        unsigned int GetArticleBuyPriceByPrototypeId(int) const ;
        void ApplyAffixGeneratorToRepository(WorkshopRepositoryType,int,unsigned int,int);
        virtual void Remove();
        float GetRepairPriceForOneUnit(Obj const *) const ;
        static m3d::Class * GetBaseClass();
        unsigned int GetObjectRepairPrice(Obj const *) const ;
        Workshop(WorkshopPrototypeInfo const &);
        void AddArticle(Article const &) const ;
        void AddArticle(int) const ;
        Article const * GetArticle(int) const ;
        Article * GetArticle(int);
        GeomRepository * GetRepositoryByType(int) const ;
        GeomRepository * GetRepositoryByType(WorkshopRepositoryType) const ;
        bool BuyArticle(int);
        void GenerateGunRepository(int,unsigned int);
        float GetHealthPriceForOneUnit() const ;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual void AddChild(Obj *);
        unsigned int GetObjectSellPrice(Obj const *) const ;
        unsigned int GetArticleAmount(int);
        unsigned int GetObjectBuyPrice(Obj const *) const ;
        virtual WorkshopPrototypeInfo const * GetPrototypeInfo() const ;
        unsigned int GetArticleSellPriceByObj(Obj const *) const ;
        GeomRepository * GetRepositoryByTypename(CStr const &) const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void Update(float,unsigned int);
        bool SellArticle(int,unsigned int);
        unsigned int GetArticleSellPriceByPrototypeId(int) const ;
        virtual m3d::Class * GetClass() const ;
        unsigned int GetArticleBuyPriceByObj(Obj const *) const ;
        static WorkshopRepositoryType __fastcall GetRepositoryTypeByResourceId(int);
        bool BuyObject(Obj const *);
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void OnSaveVisitedMap();
        bool SellObject(Obj const *);

    protected:
        virtual ~Workshop();

    private:
        static m3d::Object * CreateObject();
        void _GetArticles(std::vector<Article> &) const ;
        virtual m3d::Object * Clone();
        bool _ArticleExists(int) const ;
        bool _SellRealObject(Obj const *);
        static int __fastcall _GetWarePrototypeId(int);
        float _GetRealObjectResourceCoeff(Obj const *) const ;
        void ClearRepositoriesFromNonOriginalObjects();
        unsigned int _GetRealObjectBuyPrice(Obj const *) const ;
        bool _BuyRealObject(Obj const *);
        unsigned int _GetRealObjectSellPrice(Obj const *) const ;

    public:
        RT_CLASS_DECLARE(Workshop);

    private:
        std::map<WorkshopRepositoryType,GeomRepository *> m_repositories;
       // std::map<int,Article> m_articles;
        WorkshopPriceCoeffProvider *m_priceCoeffProvider;
        std::vector<int> m_originalObjectsInRepository;
    };
}
