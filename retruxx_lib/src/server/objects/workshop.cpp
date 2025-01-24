#include "workshop.h"
#include "article.h"
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Workshop, GetRepositoryByType)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Workshop, GetRepositoryByTypename)
{
    throw retruxx::logic_error("Not implemented");
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Workshop)
        RT_CLASS_EXPORT(Workshop, m3d::METHOD, GetRepositoryByType, "", "", "")
        RT_CLASS_EXPORT(Workshop, m3d::METHOD, GetRepositoryByTypename, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Workshop);

    WorkshopPriceCoeffProvider::WorkshopPriceCoeffProvider(Workshop const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float WorkshopPriceCoeffProvider::GetPriceCoeffForObj(Obj const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    WorkshopPrototypeInfo::WorkshopPrototypeInfo()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool WorkshopPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    ai::Obj* WorkshopPrototypeInfo::CreateTargetObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Workshop::GetArticleBuyPriceByPrototypeId(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Workshop::ApplyAffixGeneratorToRepository(WorkshopRepositoryType, int, unsigned, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Workshop::Remove()
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Workshop::GetRepairPriceForOneUnit(Obj const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Workshop::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Building);
    }

    unsigned Workshop::GetObjectRepairPrice(Obj const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Workshop::Workshop(WorkshopPrototypeInfo const& prototype) : Building(prototype)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Workshop::AddArticle(Article const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Workshop::AddArticle(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Article const* Workshop::GetArticle(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Article* Workshop::GetArticle(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    GeomRepository* Workshop::GetRepositoryByType(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    GeomRepository* Workshop::GetRepositoryByType(WorkshopRepositoryType) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Workshop::BuyArticle(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Workshop::GenerateGunRepository(int, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Workshop::GetHealthPriceForOneUnit() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Workshop::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Workshop::AddChild(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Workshop::GetObjectSellPrice(Obj const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Workshop::GetArticleAmount(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Workshop::GetObjectBuyPrice(Obj const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    WorkshopPrototypeInfo const* Workshop::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Workshop::GetArticleSellPriceByObj(Obj const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    GeomRepository* Workshop::GetRepositoryByTypename(CStr const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Workshop::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Workshop::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Workshop::Update(float, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Workshop::SellArticle(int, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Workshop::GetArticleSellPriceByPrototypeId(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Workshop::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Workshop::GetArticleBuyPriceByObj(Obj const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    WorkshopRepositoryType Workshop::GetRepositoryTypeByResourceId(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Workshop::BuyObject(Obj const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Workshop::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Workshop::OnSaveVisitedMap()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Workshop::SellObject(Obj const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Workshop::~Workshop()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Workshop::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Workshop::_GetArticles(retruxx::vector<Article>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Workshop::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Workshop::_ArticleExists(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Workshop::_SellRealObject(Obj const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Workshop::_GetWarePrototypeId(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Workshop::_GetRealObjectResourceCoeff(Obj const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Workshop::ClearRepositoriesFromNonOriginalObjects()
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Workshop::_GetRealObjectBuyPrice(Obj const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Workshop::_BuyRealObject(Obj const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Workshop::_GetRealObjectSellPrice(Obj const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }
}
