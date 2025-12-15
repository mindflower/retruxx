#include "workshop.h"
#include "article.h"
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Workshop, GetRepositoryByType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Workshop, GetRepositoryByTypename)
{
    RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    float WorkshopPriceCoeffProvider::GetPriceCoeffForObj(Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WorkshopPrototypeInfo::WorkshopPrototypeInfo() = default;

    bool WorkshopPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return BuildingPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    ai::Obj* WorkshopPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Workshop::GetArticleBuyPriceByPrototypeId(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Workshop::ApplyAffixGeneratorToRepository(WorkshopRepositoryType, int, unsigned, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Workshop::Remove()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Workshop::GetRepairPriceForOneUnit(Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Workshop::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Building);
    }

    unsigned Workshop::GetObjectRepairPrice(Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Workshop::Workshop(WorkshopPrototypeInfo const& prototype) : Building(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Workshop::AddArticle(Article const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Workshop::AddArticle(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Article const* Workshop::GetArticle(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Article* Workshop::GetArticle(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomRepository* Workshop::GetRepositoryByType(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomRepository* Workshop::GetRepositoryByType(WorkshopRepositoryType) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Workshop::BuyArticle(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Workshop::GenerateGunRepository(int, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Workshop::GetHealthPriceForOneUnit() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Workshop::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Workshop::AddChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Workshop::GetObjectSellPrice(Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Workshop::GetArticleAmount(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Workshop::GetObjectBuyPrice(Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WorkshopPrototypeInfo const* Workshop::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Workshop::GetArticleSellPriceByObj(Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomRepository* Workshop::GetRepositoryByTypename(CStr const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Workshop::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Workshop::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Workshop::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Workshop::SellArticle(int, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Workshop::GetArticleSellPriceByPrototypeId(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Workshop::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Workshop::GetArticleBuyPriceByObj(Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WorkshopRepositoryType Workshop::GetRepositoryTypeByResourceId(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Workshop::BuyObject(Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Workshop::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Workshop::OnSaveVisitedMap()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Workshop::SellObject(Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Workshop::~Workshop()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Workshop::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Workshop::_GetArticles(retruxx::vector<Article>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Workshop::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Workshop::_ArticleExists(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Workshop::_SellRealObject(Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Workshop::_GetWarePrototypeId(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Workshop::_GetRealObjectResourceCoeff(Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Workshop::ClearRepositoriesFromNonOriginalObjects()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Workshop::_GetRealObjectBuyPrice(Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Workshop::_BuyRealObject(Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Workshop::_GetRealObjectSellPrice(Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
