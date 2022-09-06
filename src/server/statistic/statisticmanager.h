#pragma once
#include <core/clazz.h>
#include <map>
#include <vector>

namespace ai
{
    class Statistic :  public m3d::Object
    {
    public:
        virtual m3d::Class * GetRtClass() const ;
        static m3d::Class * GetBaseClass();
        bool GetGlobalFlag() const ;
        virtual void LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void SetGlobalFlag(bool);
        virtual ~Statistic();
        virtual void SaveToXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;

    public:
        RT_CLASS_DECLARE(Statistic);

    protected:
        Statistic();

    private:
        bool m_bGlobalFlag;
    };

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
}
