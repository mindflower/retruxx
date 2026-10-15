#pragma once
#include <core/clazz.h>
#include <map>
#include <vector>

namespace ai
{
    class Statistic : public m3d::Object
    {
    protected:
        Statistic(ai::Statistic const&);
        Statistic();

    public:
        virtual ~Statistic() override;
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetRtClass() const;
        static m3d::Class m_classStatistic;
        bool m_bGlobalFlag;
        virtual void LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        virtual void SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        virtual CStr GetValue() const = 0;
        virtual void Zero() = 0;
        virtual m3d::AIParam GetValueAsAIParam() const = 0;
        bool GetGlobalFlag() const;
        void SetGlobalFlag(bool bGlobal);
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

    // Canonical statistic names, registered by the server-side static initializers
    // ($E29_14 .. $E53_11 in the shipped build).
    inline CStr const STATISTIC_REAL_TIME = "RealTime";
    inline CStr const STATISTIC_GAME_TIME = "GameTime";
    inline CStr const STATISTIC_PEOPLES_KILLED = "PeoplesKilled";
    inline CStr const STATISTIC_PATH_ELAPSED = "PathElapsed";
    inline CStr const STATISTIC_GDP = "GDP";
    inline CStr const STATISTIC_DEATH_COUNTER = "DeathCounter";
    inline CStr const STATISTIC_VEHICLE_KILLED = "VehiclesKilled";
    inline CStr const STATISTIC_SHELLS_EMITTED = "ShellsEmitted";
    inline CStr const STATISTIC_FAVORITE_GUN = "FavoriteGun";
    inline CStr const STATISTIC_HIT_RATIO = "HitRatio";
    inline CStr const STATISTIC_WARES = "Wares";
}  // namespace ai
