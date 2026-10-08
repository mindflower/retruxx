#include "workshop.h"
#include "affixgenerator.h"
#include "article.h"
#include "base/objcontainer.h"
#include "base/prototypemanager.h"
#include "base/complexphysicobj.h"
#include "chassis.h"
#include "gadget.h"
#include "base/globalproperties.h"
#include "physicbodies/vehiclepart.h"
#include "player.h"
#include "repositoryobjectsgenerator.h"
#include "town.h"
#include "vehicle.h"
#include "ware.h"
#include "server/geomrepository.h"
#include "server/geomrepositoryitem.h"
#include "server/resourcemanager.h"
#include "server/server.h"
#include "server/statistic/intstatistic.h"
#include "server/statistic/statisticmanager.h"
#include <server/utils.h>

#include <algorithm>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <level.h>
#include <stdexcept>
#include <world.h>

RT_CLASS_EXPORT_METHOD_DEFINE(Workshop, GetRepositoryByType)
{
    auto* workshop = (ai::Workshop*)context->asObject(0, "Workshop");
    context->pushObject(workshop->GetRepositoryByType(context->asInt(1)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Workshop, GetRepositoryByTypename)
{
    auto* workshop = (ai::Workshop*)context->asObject(0, "Workshop");
    CStr const type = context->asString(1);
    context->pushObject(workshop->GetRepositoryByTypename(type));
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Workshop)
    RT_CLASS_EXPORT(Workshop, m3d::METHOD, GetRepositoryByType, "", "", "")
    RT_CLASS_EXPORT(Workshop, m3d::METHOD, GetRepositoryByTypename, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Workshop);

    namespace
    {
        // The name each repository goes under in the level XML.
        char const* const WorkshopRepositoryTypeNames[WORKSHOP_NUM_TYPES] = {
            "Goods",
            "CabinsAndBaskets",
            "Vehicles",
            "GunsAndGadgets",
        };

        WorkshopRepositoryType Str2WorkshopRepositoryType(CStr const& str)
        {
            // RVA 0x6B43A0
            for (unsigned i = 0; i < WORKSHOP_NUM_TYPES; ++i)
            {
                if (str == WorkshopRepositoryTypeNames[i])
                {
                    return static_cast<WorkshopRepositoryType>(i);
                }
            }
            M3D_LOG_INFO("Invalid workshop repository type name: '" + str + CStr("'"));
            return WORKSHOP_NUM_TYPES;
        }

        NumericInRange<float> const& getObjectDurability(Obj const* obj)
        {
            // RVA 0x6B66B0 - how worn an object is, whatever kind of object it happens to be. A vehicle has
            // no durability of its own, so one is made up out of the sum of its parts.
            static NumericInRange<float> const Zero(0.0f, 0.0f, 0.0f);
            static NumericInRange<float> const One(1.0f, 1.0f, 1.0f);
            static NumericInRange<float> Result(0.0f, 0.0f, 0.0f);

            if (!obj)
            {
                return Zero;
            }
            if (obj->IsKindOf(&ai::Ware::m_classWare))
            {
                return ((Ware const*)obj)->Durability();
            }
            if (obj->IsKindOf(&ai::VehiclePart::m_classVehiclePart))
            {
                return ((VehiclePart const*)obj)->Durability();
            }
            if (!obj->IsKindOf(&ai::Vehicle::m_classVehicle))
            {
                return obj->IsKindOf(&ai::Gadget::m_classGadget) ? One : Zero;
            }

            auto const* vehicle = (Vehicle const*)obj;
            Result.value().set(vehicle->GetFullDurability());
            Result.minValue().set(0.0f);
            Result.maxValue().set(vehicle->GetMaxFullDurability());
            return Result;
        }
    }  // namespace

    WorkshopPriceCoeffProvider::WorkshopPriceCoeffProvider(Workshop const* workshop) : m_workshop(workshop)
    {
    }

    float WorkshopPriceCoeffProvider::GetPriceCoeffForObj(Obj const* obj) const
    {
        // RVA 0x6B3760 - the town a workshop stands in decides what its goods are worth.
        Obj* const parent = m_workshop->GetParent();
        if (parent && parent->IsKindOf(&ai::Town::m_classTown))
        {
            return ((Town const*)parent)->GetObjResourceCoeff(obj);
        }
        return 1.0f;
    }

    WorkshopPrototypeInfo::WorkshopPrototypeInfo() = default;

    bool WorkshopPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return BuildingPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    ai::Obj* WorkshopPrototypeInfo::CreateTargetObject() const
    {
        return new Workshop(*this);
    }

    unsigned Workshop::GetArticleBuyPriceByPrototypeId(int articleProtoId) const
    {
        // RVA 0x6B82C0
        if (m_articles.find(articleProtoId) == m_articles.end())
        {
            AddArticle(articleProtoId);
        }
        auto const it = m_articles.find(articleProtoId);
        // NOTE: a still-missing article is asked for its price through a null pointer.
        return it != m_articles.end() ? it->second.getBuyPrice()
                                     : ((Article const*)nullptr)->getBuyPrice();
    }

    void Workshop::ApplyAffixGeneratorToRepository(
        WorkshopRepositoryType type,
        int AffixGeneratorPrototypeId,
        unsigned count,
        int itemsResourceId)
    {
        // RVA 0x6B5920 - gives every item of the named resource on one counter a set of random affixes.
        GeomRepository* const repository = GetRepositoryByType(type);
        if (!repository)
        {
            return;
        }
        // NOTE: the affix generator prototype is used without a null check.
        auto const* generator = (AffixGeneratorPrototypeInfo const*)
            thePrototypeManager->GetPrototypeInfo(AffixGeneratorPrototypeId);

        for (unsigned i = 0; i < repository->GetNumItems(); ++i)
        {
            GeomRepositoryItem const item = repository->GetItem(i);
            if (!theResourceManager->bResourceIsKindOf(item.GetResourceId(), itemsResourceId))
            {
                continue;
            }
            Obj* const obj = theObjects->GetEntityByObjId(item.GetObjId());
            if (obj)
            {
                generator->GenerateAffixesForObj(obj, count);
            }
        }
    }

    void Workshop::Remove()
    {
        // RVA 0x6B2DC0
        Building::Remove();
    }

    float Workshop::GetRepairPriceForOneUnit(Obj const* obj) const
    {
        // RVA 0x6B2EF0 - NOTE: the test is inverted. A real vehicle part returns 0, and anything that
        // is not one is cast to a part anyway and asked its repair price. Kept as shipped.
        if (!obj || obj->IsKindOf(&ai::VehiclePart::m_classVehiclePart))
        {
            return 0.0f;
        }
        float const resourceCoeff = _GetRealObjectResourceCoeff(obj);
        return ((VehiclePart const*)obj)->GetRepairPriceForOneUnit() * resourceCoeff;
    }

    m3d::Class* Workshop::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Building);
    }

    unsigned Workshop::GetObjectRepairPrice(Obj const* obj) const
    {
        // RVA 0x6B5B00 - mending a whole vehicle costs the sum of mending each of its parts.
        if (!obj)
        {
            return 0;
        }
        if (obj->IsKindOf(&ai::Vehicle::m_classVehicle))
        {
            unsigned price = 0;
            for (auto const& part : ((ComplexPhysicObj const*)obj)->m_vehicleParts)
            {
                price += GetObjectRepairPrice(part.second);
            }
            return price;
        }
        if (obj->IsKindOf(&ai::VehiclePart::m_classVehiclePart))
        {
            auto const* part = (VehiclePart const*)obj;
            float const resourceCoeff = _GetRealObjectResourceCoeff(part);
            return ai::GetIntRepairPrice(static_cast<float>(part->GetRepairPrice()) * resourceCoeff);
        }
        return 0;
    }

    Workshop::Workshop(WorkshopPrototypeInfo const& prototype) : Building(prototype)
    {
        m_priceCoeffProvider = new WorkshopPriceCoeffProvider(this);
    }

    void Workshop::AddArticle(Article const& newArticle) const
    {
        // RVA 0x6B7C00 - an article already on the books is left alone.
        if (m_articles.find(newArticle.getPrototypeId()) == m_articles.end())
        {
            m_articles.insert({newArticle.getPrototypeId(), newArticle});
        }
    }

    void Workshop::AddArticle(int articlePrototypeId) const
    {
        // RVA 0x6B7B50
        if (m_articles.find(articlePrototypeId) == m_articles.end())
        {
            m_articles.insert({articlePrototypeId, Article(articlePrototypeId)});
        }
    }

    Article const* Workshop::GetArticle(int articlePrototypeId) const
    {
        // RVA 0x6B5A30
        auto const it = m_articles.find(articlePrototypeId);
        return it != m_articles.end() ? &it->second : nullptr;
    }

    Article* Workshop::GetArticle(int articlePrototypeId)
    {
        // RVA 0x6B5A70
        auto const it = m_articles.find(articlePrototypeId);
        return it != m_articles.end() ? &it->second : nullptr;
    }

    GeomRepository* Workshop::GetRepositoryByType(int type) const
    {
        // RVA 0x6B6570
        auto const it = m_repositories.find(static_cast<WorkshopRepositoryType>(type));
        return it != m_repositories.end() ? it->second : nullptr;
    }

    GeomRepository* Workshop::GetRepositoryByType(WorkshopRepositoryType type) const
    {
        // RVA 0x6B5850
        auto const it = m_repositories.find(type);
        return it != m_repositories.end() ? it->second : nullptr;
    }

    bool Workshop::BuyArticle(int wareObjId)
    {
        // RVA 0x6B8330 - one ware sold to the workshop. NOTE: this always reports success, even when the
        // object was not a ware at all and its prototype id came back as -1.
        int const warePrototypeId = _GetWarePrototypeId(wareObjId);
        if (m_articles.find(warePrototypeId) == m_articles.end())
        {
            AddArticle(warePrototypeId);
        }
        auto const it = m_articles.find(warePrototypeId);
        if (it != m_articles.end())
        {
            it->second.Buy(1);
        }
        return true;
    }

    void Workshop::GenerateGunRepository(int GunGeneratorId, unsigned DesiredGunsInWorkshop)
    {
        // RVA 0x6B5890
        GeomRepository* const repository = GetRepositoryByType(WORKSHOP_GUNS_AND_GADGETS);
        if (!repository)
        {
            return;
        }
        // NOTE: the generator prototype is used without a null check.
        auto const* generator = (RepositoryObjectsGeneratorPrototypeInfo const*)
            thePrototypeManager->GetPrototypeInfo(GunGeneratorId);
        generator->Generate(DesiredGunsInWorkshop, repository);
    }

    float Workshop::GetHealthPriceForOneUnit() const
    {
        // RVA 0x6B2E80 - what one point of the player's own vehicle costs to mend here.
        if (!thePlayer || !thePlayer->GetVehicle())
        {
            return -1.0f;
        }
        Chassis const* const chassis = thePlayer->GetVehicle()->GetChassis();
        float const vehicleRepairCoef = chassis->GetPrototypeInfo()->m_repairCoef;
        return _GetRealObjectResourceCoeff(chassis) * ai::theGlobProp.m_healthUnitPrice * vehicleRepairCoef;
    }

    void Workshop::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x6B80A0
        Obj::LoadRuntimeValues(xmlFile, xmlNode);

        retruxx::vector<Article> tArticles;
        Article::LoadArticlesFromNode(tArticles, xmlFile, xmlNode);
        m_articles.clear();
        for (auto const& article : tArticles)
        {
            m_articles.insert({article.getPrototypeId(), article});
        }

        m_originalObjectsInRepository.clear();
        CStr strOriginalObjectsInRepository;
        if (!xmlNode->IsEmpty())
        {
            char const* const value = xmlNode->GetAttribute("OriginalObjectsInRepository");
            if (value)
            {
                strOriginalObjectsInRepository = value;
            }
        }
        ai::StrToIntVector(strOriginalObjectsInRepository, m_originalObjectsInRepository);
    }

    void Workshop::AddChild(Obj* pObj)
    {
        // RVA 0x6B2DD0
        Building::AddChild(pObj);
    }

    unsigned Workshop::GetObjectSellPrice(Obj const* obj) const
    {
        // RVA 0x6B89B0 - something the workshop stocks is sold at its article price; anything else at
        // whatever the object itself is worth here.
        if (m_articles.find(obj->GetPrototypeId()) == m_articles.end())
        {
            return obj->GetPrice(m_priceCoeffProvider);
        }
        return GetArticleSellPriceByPrototypeId(obj->GetPrototypeId());
    }

    unsigned Workshop::GetArticleAmount(int pId)
    {
        // RVA 0x6B5AB0
        auto const it = m_articles.find(pId);
        return it != m_articles.end() ? it->second.getAmount() : 0;
    }

    unsigned Workshop::GetObjectBuyPrice(Obj const* obj) const
    {
        // RVA 0x6B8900 - a workshop buys back at half price what it does not stock as an article.
        float price = 0.0f;
        if (m_articles.find(obj->GetPrototypeId()) == m_articles.end())
        {
            price = static_cast<float>(obj->GetPrice(m_priceCoeffProvider)) * 0.5;
        }
        else
        {
            price = ai::GetDurabilityPriceCoeff(getObjectDurability(obj)) *
                static_cast<float>(GetArticleBuyPriceByPrototypeId(obj->GetPrototypeId()));
        }
        return ai::GetIntPrice(price);
    }

    WorkshopPrototypeInfo const* Workshop::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), WorkshopPrototypeInfo const);
    }

    unsigned Workshop::GetArticleSellPriceByObj(Obj const* obj) const
    {
        // RVA 0x6B8890
        return obj ? GetArticleSellPriceByPrototypeId(obj->GetPrototypeId()) : -1;
    }

    GeomRepository* Workshop::GetRepositoryByTypename(CStr const& type) const
    {
        // RVA 0x6B5BC0
        auto const it = m_repositories.find(Str2WorkshopRepositoryType(type));
        return it != m_repositories.end() ? it->second : nullptr;
    }

    void Workshop::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x6B4B60
        Obj::SaveRuntimeValues(xmlFile, xmlNode);

        for (auto const& article : m_articles)
        {
            ref_ptr ndArticle = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Article");
            xmlNode->AddChild(ndArticle);
            article.second.SaveToXml(xmlFile, ndArticle);
        }

        xmlNode->SetAttribute(
            "OriginalObjectsInRepository", ai::IntVectorToStr(m_originalObjectsInRepository).c_str());
    }

    void Workshop::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x6B8CA0 - each counter goes under a node named after its type, then the articles.
        Obj::SaveToXML(xmlFile, xmlNode);

        for (auto const& repositoryEntry : m_repositories)
        {
            CStr const typeName = repositoryEntry.first < WORKSHOP_NUM_TYPES
                ? CStr(WorkshopRepositoryTypeNames[repositoryEntry.first])
                : CStr("");
            ref_ptr ndRepository = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, typeName.c_str());
            repositoryEntry.second->SaveToXML(xmlFile, ndRepository);
            xmlNode->AddChild(ndRepository);
        }

        retruxx::vector<Article> articles;
        _GetArticles(articles);
        Article::SaveArticlesToNode(articles, xmlFile, xmlNode);
    }

    void Workshop::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x6B57D0 - the articles restock themselves over time.
        Obj::Update(elapsedTime, workTime);
        for (auto& article : m_articles)
        {
            article.second.Update(elapsedTime);
        }
    }

    bool Workshop::SellArticle(int warePrototypeId, unsigned amount)
    {
        // RVA 0x6B6B70 - the workshop only sells what it actually has on the shelf.
        auto const it = m_articles.find(warePrototypeId);
        unsigned const available = it != m_articles.end() ? it->second.getAmount() : 0;
        if (available < amount)
        {
            return false;
        }

        auto const toSell = m_articles.find(warePrototypeId);
        if (toSell != m_articles.end())
        {
            toSell->second.Sell(amount);
        }

        // The player's trading is counted both overall and per level.
        auto* const waresStat = dynamic_cast<IntStatistic*>(
            theStatisticManager->GetStatistic(ai::STATISTIC_WARES, "IntStatistic"));
        waresStat->m_bGlobalFlag = true;
        waresStat->Increase(amount);

        CStr const levelStatName = ai::STATISTIC_WARES + ai::pServer->GetWorld()->m_level->m_levelName;
        auto* const levelWaresStat = dynamic_cast<IntStatistic*>(
            theStatisticManager->GetStatistic(levelStatName, "IntStatistic"));
        levelWaresStat->m_bGlobalFlag = false;
        levelWaresStat->Increase(amount);
        return true;
    }

    unsigned Workshop::GetArticleSellPriceByPrototypeId(int articleProtoId) const
    {
        // RVA 0x6B8250 - asking the price of something the workshop does not stock puts it on the books.
        if (m_articles.find(articleProtoId) == m_articles.end())
        {
            AddArticle(articleProtoId);
        }
        auto const it = m_articles.find(articleProtoId);
        // NOTE: a still-missing article is asked for its price through a null pointer.
        return it != m_articles.end() ? it->second.getSellPrice()
                                     : ((Article const*)nullptr)->getSellPrice();
    }

    m3d::Class* Workshop::GetClass() const
    {
        return RT_CLASS_LOCAL(Workshop);
    }

    unsigned Workshop::GetArticleBuyPriceByObj(Obj const* article) const
    {
        // RVA 0x6B88B0 - a worn item fetches less than a new one.
        // NOTE: unlike GetArticleSellPriceByObj this does not check the object for null.
        float const price = static_cast<float>(GetArticleBuyPriceByPrototypeId(article->GetPrototypeId()));
        return ai::GetIntPrice(ai::GetDurabilityPriceCoeff(getObjectDurability(article)) * price);
    }

    WorkshopRepositoryType Workshop::GetRepositoryTypeByResourceId(int resId)
    {
        // RVA 0x6B3860 - which of the workshop's four counters a resource belongs behind.
        if (theResourceManager->bResourceIsKindOf(resId, theResourceManager->GetResourceId(CStr("GUN"))))
        {
            return WORKSHOP_GUNS_AND_GADGETS;
        }
        if (theResourceManager->bResourceIsKindOf(resId, theResourceManager->GetResourceId(CStr("GOODS"))))
        {
            return WORKSHOP_GOODS;
        }
        if (theResourceManager->bResourceIsKindOf(resId, theResourceManager->GetResourceId(CStr("GADGET"))))
        {
            return WORKSHOP_GUNS_AND_GADGETS;
        }
        if (theResourceManager->bResourceIsKindOf(
                resId, theResourceManager->GetResourceId(CStr("VEHICLE_PART"))))
        {
            return WORKSHOP_CABINS_AND_BASKETS;
        }
        if (theResourceManager->bResourceIsKindOf(resId, theResourceManager->GetResourceId(CStr("VEHICLE"))))
        {
            return WORKSHOP_VEHICLES;
        }
        return WORKSHOP_NUM_TYPES;
    }

    bool Workshop::BuyObject(Obj const* obj)
    {
        // RVA 0x6B8400 - something the workshop already trades in becomes another unit of that article;
        // anything else is taken in as the object it is.
        if (m_articles.find(obj->GetPrototypeId()) == m_articles.end())
        {
            return _BuyRealObject(obj);
        }
        return BuyArticle(obj->GetId());
    }

    void Workshop::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x6B8540 - one counter per repository node present in the level file.
        Obj::LoadFromXML(xmlFile, xmlNode);
        if (xmlNode->IsEmpty())
        {
            return;
        }

        for (int i = 0; i < WORKSHOP_NUM_TYPES; ++i)
        {
            CStr const typeName(WorkshopRepositoryTypeNames[i]);
            ref_ptr ndRepository = xmlFile->CreateNode();
            xmlNode->GetFirstChild(ndRepository, typeName.c_str());
            if (ndRepository->IsEmpty())
            {
                continue;
            }

            auto* const repository = (GeomRepository*)M3D_KERNEL->New("GeomRepository");
            repository->SetGeomSize(ai::theGlobProp.m_groundRepositorySize);
            repository->LoadFromXML(xmlFile, ndRepository);
            if (i == WORKSHOP_GUNS_AND_GADGETS)
            {
                repository->SetSortStyle(GeomRepository::SORT_BY_RESOURCE, true);
            }

            auto const type = static_cast<WorkshopRepositoryType>(i);
            auto const existing = m_repositories.find(type);
            if (existing != m_repositories.end())
            {
                delete m_repositories[type];
                m_repositories[type] = nullptr;
            }
            m_repositories[type] = repository;
        }

        // A workshop is a shop for guns whether or not the level gave it that counter.
        if (GetPrototypeInfo()->m_buildingType == WORKSHOP &&
            m_repositories.find(WORKSHOP_GUNS_AND_GADGETS) == m_repositories.end())
        {
            auto* const repository = (GeomRepository*)M3D_KERNEL->New("GeomRepository");
            repository->SetGeomSize(ai::theGlobProp.m_groundRepositorySize);
            repository->SetSortStyle(GeomRepository::SORT_BY_RESOURCE, true);
            m_repositories[WORKSHOP_GUNS_AND_GADGETS] = repository;
        }

        // Remember what the level shipped with, so that a revisit can throw away the rest.
        if (theObjects->m_SaveType == ObjContainer::SAVE_LEVEL)
        {
            m_originalObjectsInRepository.clear();
            for (auto const& repositoryEntry : m_repositories)
            {
                GeomRepository* const repository = repositoryEntry.second;
                if (!repository)
                {
                    continue;
                }
                int const numItems = repository->GetNumItems();
                for (int i = 0; i < numItems; ++i)
                {
                    m_originalObjectsInRepository.push_back(repository->GetItem(i).GetObjId());
                }
            }
            std::sort(m_originalObjectsInRepository.begin(), m_originalObjectsInRepository.end());
        }
    }

    void Workshop::OnSaveVisitedMap()
    {
        // RVA 0x6B6D70 - a map the player leaves keeps only the stock it started with.
        ClearRepositoriesFromNonOriginalObjects();
    }

    bool Workshop::SellObject(Obj const* obj)
    {
        // RVA 0x6B6D20
        if (m_articles.find(obj->GetPrototypeId()) == m_articles.end())
        {
            return _SellRealObject(obj);
        }
        return SellArticle(obj->GetPrototypeId(), 1);
    }

    Workshop::~Workshop()
    {
        // RVA 0x6B8B50 - the workshop owns its counters and its price provider.
        for (auto& repositoryEntry : m_repositories)
        {
            delete repositoryEntry.second;
            repositoryEntry.second = nullptr;
        }
        m_repositories.clear();

        delete m_priceCoeffProvider;
        m_priceCoeffProvider = nullptr;
        m_originalObjectsInRepository.clear();
    }

    m3d::Object* Workshop::CreateObject()
    {
        // RVA 0x6B41E0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    void Workshop::_GetArticles(retruxx::vector<Article>& articles) const
    {
        // Not emitted in the shipped binary - it was inlined into its only caller, SaveToXML. Written here
        // from what that caller needs: every article the workshop keeps, in map order.
        articles.clear();
        for (auto const& article : m_articles)
        {
            articles.push_back(article.second);
        }
    }

    m3d::Object* Workshop::Clone()
    {
        // RVA 0x6B4020
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    bool Workshop::_ArticleExists(int articlePrototypeId) const
    {
        // RVA 0x6B5E00
        return m_articles.find(articlePrototypeId) != m_articles.end();
    }

    bool Workshop::_SellRealObject(Obj const* obj)
    {
        // RVA 0x6B5C70
        GeomRepository* const repository =
            GetRepositoryByType(GetRepositoryTypeByResourceId(obj->GetPrototypeInfo()->m_resourceId));
        if (!repository)
        {
            return false;
        }

        // The player's trading is counted both overall and per level.
        auto* const waresStat = dynamic_cast<IntStatistic*>(
            theStatisticManager->GetStatistic(ai::STATISTIC_WARES, "IntStatistic"));
        waresStat->m_bGlobalFlag = true;
        waresStat->Increase(1);

        CStr const levelStatName = ai::STATISTIC_WARES + ai::pServer->GetWorld()->m_level->m_levelName;
        auto* const levelWaresStat = dynamic_cast<IntStatistic*>(
            theStatisticManager->GetStatistic(levelStatName, "IntStatistic"));
        levelWaresStat->m_bGlobalFlag = false;
        levelWaresStat->Increase(1);
        return repository->GiveUpThingByObjId(obj->GetId());
    }

    int Workshop::_GetWarePrototypeId(int wareObjId)
    {
        // RVA 0x6B5E30
        Obj* const obj = theObjects->GetEntityByObjId(wareObjId);
        if (!obj || !obj->IsKindOf(&ai::Ware::m_classWare))
        {
            return -1;
        }
        return obj->GetPrototypeId();
    }

    float Workshop::_GetRealObjectResourceCoeff(Obj const* obj) const
    {
        // RVA 0x6B2DE0
        Obj* const parent = GetParent();
        if (parent && parent->IsKindOf(&ai::Town::m_classTown))
        {
            return ((Town const*)parent)->GetObjResourceCoeff(obj);
        }
        return 1.0f;
    }

    void Workshop::ClearRepositoriesFromNonOriginalObjects()
    {
        // RVA 0x6B5E80 - anything the player left on a counter is thrown away when the map is saved; only
        // the stock the level shipped with survives.
        for (auto const& repositoryEntry : m_repositories)
        {
            GeomRepository* const repository = repositoryEntry.second;
            if (!repository)
            {
                continue;
            }
            int const numItems = repository->GetNumItems();
            for (int i = 0; i < numItems; ++i)
            {
                int const objId = repository->GetItem(i).GetObjId();
                if (std::find(
                        m_originalObjectsInRepository.begin(), m_originalObjectsInRepository.end(), objId) !=
                    m_originalObjectsInRepository.end())
                {
                    continue;
                }
                Obj* const obj = theObjects->GetEntityByObjId(objId);
                if (obj)
                {
                    repository->GiveUpThingFromSlotUnsafe(repository->GetSlotByObjId(obj->GetId()), 1);
                    obj->Remove();
                }
            }
            repository->Purge();
        }
        m_originalObjectsInRepository.clear();
    }

    unsigned Workshop::_GetRealObjectBuyPrice(Obj const* obj) const
    {
        // RVA 0x6B2E20
        return ai::GetIntPrice(static_cast<float>(obj->GetPrice(m_priceCoeffProvider)) * 0.5);
    }

    bool Workshop::_BuyRealObject(Obj const* obj)
    {
        // RVA 0x6B5C00 - the object goes onto the counter its resource belongs behind.
        GeomRepository* const repository =
            GetRepositoryByType(GetRepositoryTypeByResourceId(obj->GetPrototypeInfo()->m_resourceId));
        if (!repository)
        {
            return false;
        }
        GeomRepositoryItem item(obj->GetId());
        return repository->AddThing(item, 0);
    }

    unsigned Workshop::_GetRealObjectSellPrice(Obj const* obj) const
    {
        // RVA 0x6B2E60
        return obj->GetPrice(m_priceCoeffProvider);
    }
}  // namespace ai
