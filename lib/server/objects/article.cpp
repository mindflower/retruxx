#include "article.h"

#include <cstdlib>

#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>

#include "ware.h"
#include "base/globalproperties.h"
#include "base/prototypeinfo.h"
#include "base/prototypemanager.h"

namespace ai
{
    namespace
    {
        // ai::GetIntPrice is declared in ware.h and shared with ai::Ware pricing.

        // Uniform random price coefficient in [1 - d, 1 + d], where d = dispersion%.
        float RandomPriceCoefficient(float dispersion)
        {
            float const d = dispersion * 0.01f;
            float const lo = 1.0f - d;
            float const hi = 1.0f + d;
            return (static_cast<float>(rand()) * 0.000030518509f) * (hi - lo) + lo;
        }
    }  // namespace

    Article::Article(int protoId) : m_PrototypeId(protoId)
    {
        Init0();
        ReadFromPrototype();
        ReadDefaultCountFromPrototype();
        Init1();
    }

    Article::Article(m3d::cmn::XmlFile const* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        m_PrototypeId = -1;
        Init0();
        m3d::SafeStrAttrib(m_PrototypeName, xmlNode, "Prototype");
        LoadFromXml(xmlFile, xmlNode);
    }

    Article::Article(Article const& that) :
        m_PrototypeId(that.m_PrototypeId),
        m_PrototypeName(that.m_PrototypeName),
        m_BasePrice(that.m_BasePrice),
        m_Dispersion(that.m_Dispersion),
        m_ExternalPriceCoefficient(that.m_ExternalPriceCoefficient),
        m_RandomPriceCoefficient(that.m_RandomPriceCoefficient),
        m_RuntimePriceCoefficient(that.m_RuntimePriceCoefficient),
        m_Sellable(that.m_Sellable),
        m_Buyable(that.m_Buyable),
        m_Amount(that.m_Amount),
        m_MaxCount(that.m_MaxCount),
        m_MinCount(that.m_MinCount),
        m_AmountDynamic(that.m_AmountDynamic),
        m_PriceDynamic(that.m_PriceDynamic),
        m_RegenerationPeriod(that.m_RegenerationPeriod),
        m_AfterLastRegeneration(that.m_AfterLastRegeneration)
    {
    }

    void Article::Init0()
    {
        m_BasePrice = 0;
        m_Dispersion = 0.0f;
        m_ExternalPriceCoefficient = 1.0f;
        m_RandomPriceCoefficient = -1.0f;
        m_RuntimePriceCoefficient = 1.0f;
        m_Sellable = false;
        m_Buyable = true;
        m_Amount = 0;
        m_MinCount = -1;
        m_MaxCount = -1;
        m_AmountDynamic = adConst;
        m_PriceDynamic = pdConst;
        m_RegenerationPeriod = ai::theGlobProp.m_defaultArticleRegenerationTime;
        m_AfterLastRegeneration = 0.0f;
    }

    void Article::Init1()
    {
        ReadFromPrototype();
        if (m_RandomPriceCoefficient < 0.0f)
        {
            m_RandomPriceCoefficient = RandomPriceCoefficient(m_Dispersion);
        }
        if (m_Sellable || m_Buyable)
        {
            m_PriceDynamic = pdDemand;
            m_AmountDynamic = adSaturation;
        }
    }

    void Article::ReadFromPrototype()
    {
        auto const* proto = ai::thePrototypeManager->GetPrototypeInfo(m_PrototypeId);
        if (!proto)
        {
            M3D_LOG_ERR("Error: invalid prototype id: '" + CStr(m_PrototypeId));
            return;
        }

        m_BasePrice = proto->GetBasePrice();
        if (proto->m_className == "Ware")
        {
            m_Dispersion = static_cast<WarePrototypeInfo const*>(proto)->GetPriceDispersion();
        }
        else
        {
            m_Dispersion = 20.0f;
        }
    }

    void Article::ReadDefaultCountFromPrototype()
    {
        if (m_PrototypeId == -1)
        {
            return;
        }

        auto const* proto = ai::thePrototypeManager->GetPrototypeInfo(m_PrototypeId);
        if (!proto)
        {
            M3D_LOG_ERR("Error: invalid prototype id: '" + CStr(m_PrototypeId));
            return;
        }

        if (proto->m_className == "Ware")
        {
            auto const* wareProto = static_cast<WarePrototypeInfo const*>(proto);
            m_MinCount = wareProto->GetMinCount();
            m_MaxCount = wareProto->GetMaxCount();
        }
    }

    void Article::LoadFromXml(m3d::cmn::XmlFile const*, m3d::cmn::XmlNode const* xmlNode)
    {
        if (auto const* attr = xmlNode->GetAttribute("PrototypeId"))
        {
            m_PrototypeId = atoi(attr);
        }
        if (auto const* attr = xmlNode->GetAttribute("BasePrice"))
        {
            int const value = atoi(attr);
            if (value >= 0)
            {
                m_BasePrice = value;
            }
        }
        if (auto const* attr = xmlNode->GetAttribute("Dispersion"))
        {
            m_Dispersion = static_cast<float>(atof(attr));
        }
        M3D_ASSERT(m_Dispersion >= 0.0f && m_Dispersion <= 100.0f);

        if (auto const* attr = xmlNode->GetAttribute("ExternalPriceCoefficient"))
        {
            m_ExternalPriceCoefficient = static_cast<float>(atof(attr));
        }
        if (auto const* attr = xmlNode->GetAttribute("RandomPriceCoefficient"))
        {
            m_RandomPriceCoefficient = static_cast<float>(atof(attr));
        }

        m3d::SafeBoolAttrib(m_Sellable, xmlNode, "Export");
        m3d::SafeBoolAttrib(m_Buyable, xmlNode, "Import");

        if (auto const* attr = xmlNode->GetAttribute("Amount"))
        {
            int const value = atoi(attr);
            if (value >= 0)
            {
                m_Amount = value;
            }
        }

        ReadDefaultCountFromPrototype();

        if (auto const* attr = xmlNode->GetAttribute("MaxCount"))
        {
            m_MaxCount = atoi(attr);
        }
        if (auto const* attr = xmlNode->GetAttribute("MinCount"))
        {
            m_MinCount = atoi(attr);
        }
        if (auto const* attr = xmlNode->GetAttribute("AmountDynamic"))
        {
            m_AmountDynamic = static_cast<AmountDynamic>(atoi(attr));
        }
        if (auto const* attr = xmlNode->GetAttribute("PriceDynamic"))
        {
            m_PriceDynamic = static_cast<PriceDynamic>(atoi(attr));
        }
        if (auto const* attr = xmlNode->GetAttribute("RegenerationPeriod"))
        {
            m_RegenerationPeriod = static_cast<float>(atof(attr));
        }
        if (auto const* attr = xmlNode->GetAttribute("AfterLastRegeneration"))
        {
            m_AfterLastRegeneration = static_cast<float>(atof(attr));
        }
    }

    void Article::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode* xmlNode) const
    {
        xmlNode->SetAttribute("PrototypeId", CStr(m_PrototypeId).c_str());
        xmlNode->SetAttribute("BasePrice", CStr(m_BasePrice).c_str());
        xmlNode->SetAttribute("Dispersion", CStr(m_Dispersion).c_str());
        xmlNode->SetAttribute("ExternalPriceCoefficient", CStr(m_ExternalPriceCoefficient).c_str());
        xmlNode->SetAttribute("RandomPriceCoefficient", CStr(m_RandomPriceCoefficient).c_str());
        xmlNode->SetAttribute("Export", CStr(m_Sellable).c_str());
        xmlNode->SetAttribute("Import", CStr(m_Buyable).c_str());
        xmlNode->SetAttribute("Amount", CStr(m_Amount).c_str());
        xmlNode->SetAttribute("MaxCount", CStr(m_MaxCount).c_str());
        xmlNode->SetAttribute("MinCount", CStr(m_MinCount).c_str());
        xmlNode->SetAttribute("AmountDynamic", CStr(static_cast<int>(m_AmountDynamic)).c_str());
        xmlNode->SetAttribute("PriceDynamic", CStr(static_cast<int>(m_PriceDynamic)).c_str());
        xmlNode->SetAttribute("RegenerationPeriod", CStr(m_RegenerationPeriod).c_str());
        xmlNode->SetAttribute("AfterLastRegeneration", CStr(m_AfterLastRegeneration).c_str());
    }

    void Article::PostLoad()
    {
        if (!m_PrototypeName.empty())
        {
            m_PrototypeId = ai::thePrototypeManager->GetPrototypeId(m_PrototypeName);
            if (m_PrototypeId == -1)
            {
                M3D_LOG_INFO("Unknown ware prototype name");
            }
        }
        else
        {
            M3D_LOG_INFO("Empty ware prototype name");
        }

        ReadFromPrototype();
        if (m_RandomPriceCoefficient < 0.0f)
        {
            m_RandomPriceCoefficient = RandomPriceCoefficient(m_Dispersion);
        }
        if (m_Sellable || m_Buyable)
        {
            m_PriceDynamic = pdDemand;
            m_AmountDynamic = adSaturation;
        }
    }

    void Article::Update(float elapsedTime)
    {
        if (m_AmountDynamic != adSaturation)
        {
            return;
        }

        m_AfterLastRegeneration += elapsedTime;
        if (m_AfterLastRegeneration <= m_RegenerationPeriod)
        {
            return;
        }

        if (m_Buyable && m_Amount > static_cast<unsigned int>(m_MinCount))
        {
            --m_Amount;
        }
        if (m_Sellable && m_Amount < static_cast<unsigned int>(m_MaxCount))
        {
            ++m_Amount;
        }
        m_AfterLastRegeneration = 0.0f;
    }

    void Article::Buy(unsigned int amount)
    {
        m_Amount += amount;
    }

    void Article::Sell(unsigned int amount)
    {
        m_Amount -= amount;
    }

    bool Article::IsBuyable() const
    {
        return m_Buyable;
    }

    bool Article::IsSellable() const
    {
        return m_Sellable;
    }

    unsigned int Article::getAmount() const
    {
        return m_Amount;
    }

    int Article::getPrototypeId() const
    {
        return m_PrototypeId;
    }

    float Article::getRuntimePriceCoefficient() const
    {
        return m_RuntimePriceCoefficient;
    }

    void Article::setRuntimePriceCoefficient(float value)
    {
        m_RuntimePriceCoefficient = value;
    }

    float Article::getFixedPrice() const
    {
        return static_cast<float>(
            static_cast<double>(m_BasePrice) * m_RuntimePriceCoefficient * m_RandomPriceCoefficient *
            m_ExternalPriceCoefficient);
    }

    float Article::getDemandCoeff() const
    {
        if (m_MaxCount == m_MinCount)
        {
            return 1.0f;
        }

        float demand = 1.0f;
        if (m_PriceDynamic == pdDemand)
        {
            demand = static_cast<float>(
                static_cast<double>(m_Amount - m_MinCount) / static_cast<double>(m_MaxCount - m_MinCount));
        }

        float const coeff = 1.6666666f - demand * 1.3333334f;
        return coeff >= 0.0f ? coeff : 0.0f;
    }

    unsigned int Article::getBuyPrice() const
    {
        float price = getFixedPrice();
        if (!m_Buyable)
        {
            price *= 0.33333334f;
        }
        return GetIntPrice(getDemandCoeff() * price);
    }

    unsigned int Article::getSellPrice() const
    {
        float price;
        if (m_Sellable)
        {
            price = getDemandCoeff() * getFixedPrice();
        }
        else
        {
            price = getFixedPrice() * 1000.0f;
        }
        return GetIntPrice(price);
    }

    void Article::LoadArticlesFromNode(
        std::vector<ai::Article, std::allocator<ai::Article>>& articles,
        m3d::cmn::XmlFile* xmlFile,
        m3d::cmn::XmlNode const* xmlNode)
    {
        articles.clear();
        if (xmlNode->IsEmpty())
        {
            return;
        }

        ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        for (xmlNode->GetFirstChild(node, "Article"); !node->IsEmpty(); node->GetNextSibling(node, "Article"))
        {
            articles.push_back(Article(xmlFile, node));
        }
    }

    void Article::SaveArticlesToNode(
        std::vector<ai::Article> const& articles,
        m3d::cmn::XmlFile* xmlFile,
        m3d::cmn::XmlNode* xmlNode)
    {
        for (auto const& article : articles)
        {
            ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Article");
            xmlNode->AddChild(node);
            article.SaveToXml(xmlFile, node);
        }
    }
}  // namespace ai
