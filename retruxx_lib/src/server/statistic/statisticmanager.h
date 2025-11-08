#pragma once
#include <core/clazz.h>
#include <map>
#include <vector>

namespace ai
{
    class Statistic : public m3d::Object
    {
    protected:
        Statistic(const ai::Statistic&);
        Statistic();

    public:
        virtual  ~Statistic() override /* 0x00 */;
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetRtClass() const /* 0x3c */;
        static m3d::Class m_classStatistic;
        /* 0x0034 */ bool m_bGlobalFlag;
        virtual void LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) /* 0x40 */;
        virtual void SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const /* 0x44 */;
        virtual CStr GetValue() const = 0 /* 0x48 */;
        virtual void Zero() = 0 /* 0x4c */;
        virtual m3d::AIParam GetValueAsAIParam() const = 0 /* 0x50 */;
        bool GetGlobalFlag() const;
        void SetGlobalFlag(bool bGlobal);
    }; /* size: 0x0038 */

    class StatisticManager
    {
    public:
        StatisticManager();
        CStr GetAllStatisticsDescription() const;
        void ZeroStatisticsForLevel(CStr const&);
        ~StatisticManager();
        void SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        void Clear();
        void LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        void GetAllStatistics(std::vector<Statistic const*>&) const;
        Statistic* GetStatistic(CStr const&, CStr const&);
        Statistic const* GetStatistic(CStr const&) const;

    private:
        static Statistic* _CreateStatisticByClassName(CStr const&);
        std::map<CStr, Statistic*> m_statistics;
    };

    inline StatisticManager* theStatisticManager = nullptr;

    inline const CStr STATISTIC_PATH_ELAPSED = "PathElapsed";
}
