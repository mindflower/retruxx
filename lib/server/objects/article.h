#pragma once
#include <vector>
#include <core/stringm3d.h>

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }
}

namespace ai
{
    class Article
    {
    public:
        enum PriceDynamic
        {
            pdConst = 0x0,
            pdDemand = 0x1,
        };

        enum AmountDynamic
        {
            adConst = 0x0,
            adSaturation = 0x1,
        };

    public:
        void SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        static void __fastcall LoadArticlesFromNode(std::vector<ai::Article, std::allocator<ai::Article> >&, m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        float getRuntimePriceCoefficient() const;
        void Sell(unsigned int);
        bool IsBuyable() const;
        Article(int);
        Article(ai::Article const&);
        void Buy(unsigned int);
        unsigned int getBuyPrice() const;
        void setRuntimePriceCoefficient(float);
        int getPrototypeId() const;
        unsigned int getSellPrice() const;
        float getDemandCoeff() const;
        unsigned int getAmount() const;
        void PostLoad();
        void Update(float);
        bool IsSellable() const;
        static void __fastcall SaveArticlesToNode(std::vector<ai::Article> const&, m3d::cmn::XmlFile*, m3d::cmn::XmlNode*);
        void LoadFromXml(m3d::cmn::XmlFile const*, m3d::cmn::XmlNode const*);

    private:
        Article(m3d::cmn::XmlFile const*, m3d::cmn::XmlNode const*);
        void ReadFromPrototype();
        void ReadDefaultCountFromPrototype();
        void Init1();
        void Init0();
        float getFixedPrice() const;

    private:
        int m_PrototypeId;
        CStr m_PrototypeName;
        unsigned int m_BasePrice;
        float m_Dispersion;
        float m_ExternalPriceCoefficient;
        float m_RandomPriceCoefficient;
        float m_RuntimePriceCoefficient;
        bool m_Sellable;
        bool m_Buyable;
        unsigned int m_Amount;
        int m_MaxCount;
        int m_MinCount;
        AmountDynamic m_AmountDynamic;
        PriceDynamic m_PriceDynamic;
        float m_RegenerationPeriod;
        float m_AfterLastRegeneration;
    };
}
