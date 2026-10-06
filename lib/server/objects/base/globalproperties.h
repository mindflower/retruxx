#pragma once
#include <set>
#include <vector>
#include <core/stringm3d.h>
#include <math/point2d.h>
#include <math/vector2.h>

namespace m3d
{
    namespace cmn
    {
        class XmlFile;
        struct XmlNode;
    }
}

namespace ai
{
    class GlobalProperties
    {
    public:
        class CoeffsForDifficultyLevel
        {
        public:
            CoeffsForDifficultyLevel();

        public:
            CStr m_name;
            float m_damageCoeffForPlayerFromEnemies = 0.0;
            float m_enemiesShootingDelay = 0.0;
        };

    public:
        GlobalProperties();
        CoeffsForDifficultyLevel const& GetCoeffsForCurrentDifficultyLevel() const;
        void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    public:
        PointBase<int> m_izvratRepositoryMaxSize{13, 13};
        PointBase<int> m_groundRepositorySize{14, 300};
        CStr m_pathToRelationship;
        CStr m_pathToGameObjects;
        CStr m_pathToQuests;
        CStr m_pathToResourceTypes;
        CStr m_pathToAffixes;
        CStr m_pathToVehiclePartTypes;
        float m_gameTimeMult = 60.0f;
        float m_vehicleAiFiringRangeMult = 1.0f;
        unsigned int m_minBurstTime;
        unsigned int m_maxBurstTime;
        unsigned int m_timeBetweenBursts;
        std::set<int> m_namedBelongIds;
        float m_distToTurnOnPhysics = 300.0f;
        float m_distToTurnOffPhysics = 320.0f;
        float m_physicStepTime = 0.039999999f;
        CStr m_barmenModelName = "mask1";
        float m_splintersAutoDisableLinearThreshold = 0.1f;
        float m_splintersAutoDisableAngularThreshold = 0.1f;
        int m_splintersAutoDisableNumSteps = 5;
        bool m_vehiclesDropChests = true;
        float m_maxSpeedWithNoFuel = 0.0f;
        float m_probabilityToGenerateDynamicQuestInTown = 1.0f;
        int m_infoAreaRadius = 10;
        float m_lockTimeout = 5.0f;
        CVector2 m_unlockRegion{100.0, 100.0};
        float m_infoObjUpdateTimeout = 0.5f;
        float m_blastWaveCameraShakeRadiusCoeff = 2.0f;
        float m_shakeDamageToDurationCoeff = 2.0f;
        float m_maxShakeDamage = 100.0f;
        float m_distanceFromPlayerToMoveout = 300.0f;
        float m_defaultLookBoxLength = 50.0f;
        float m_defaultTargetBoxLength = 50.0f;
        float m_attractiveCoeff = 1.0f;
        float m_repulsiveCoeff = 1.0f;
        float m_maxDistToAvoid = 1.0f;
        float m_predictionTime = 1.0f;
        float m_throwCoeff = 3.0f;
        float m_flowVpVelocity = 6.0f;
        float m_flowWheelVelocity = 4.0f;
        float m_energyBlowDeltaTime = 1.2f;
        int m_energyVpBlowProbability = 5;
        int m_energyWheelBlowProbability = 2;
        float m_healthUnitPrice = 1.0f;
        float m_defaultArticleRegenerationTime = 100.0f;
        float m_probabilityToDropArticlesFromDeadVehicles = 0.69999999f;
        float m_probabilityToDropGunsFromDeadVehicles = 0.69999999f;
        float m_zoneRespawnTimeOutIncreaseCoeff = 1.1f;
        float m_zoneDefaultFirstSpawnTime = 10.0f;
        unsigned int m_colorFriend = -16711936;
        unsigned int m_colorEnemy = -65536;
        unsigned int m_colorTargetCaptured = -256;
        float m_targetInfoContourWidth = 0.69999999f;
        float m_targetCapturedContourWidth = 0.69999999f;
        float m_playerPassMapUnpassableMu = 0.30000001f;
        float m_playerPassMapUnpassableErp = 0.1f;
        float m_playerPassMapUnpassableCfm = 0.0099999998f;
        float m_maxGroupingAngle = 0.043633234f;
        float m_timeOutForReAimGuns = 0.5f;
        std::vector<CoeffsForDifficultyLevel> m_difficultyLevelCoeffs;
        float m_property2PriceCoeff = 1.0;
    };

    extern GlobalProperties theGlobProp;
}
