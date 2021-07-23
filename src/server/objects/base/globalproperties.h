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
        class XmlNode;
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

        private:
            CStr m_name;
            float m_damageCoeffForPlayerFromEnemies;
            float m_enemiesShootingDelay;
        };

    public:
        GlobalProperties();
        CoeffsForDifficultyLevel const& GetCoeffsForCurrentDifficultyLevel() const;
        void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        PointBase<int> m_izvratRepositoryMaxSize;
        PointBase<int> m_groundRepositorySize;
        CStr m_pathToRelationship;
        CStr m_pathToGameObjects;
        CStr m_pathToQuests;
        CStr m_pathToResourceTypes;
        CStr m_pathToAffixes;
        CStr m_pathToVehiclePartTypes;
        float m_gameTimeMult;
        float m_vehicleAiFiringRangeMult;
        unsigned int m_minBurstTime;
        unsigned int m_maxBurstTime;
        unsigned int m_timeBetweenBursts;
        std::set<int> m_namedBelongIds;
        float m_distToTurnOnPhysics;
        float m_distToTurnOffPhysics;
        float m_physicStepTime;
        CStr m_barmenModelName;
        float m_splintersAutoDisableLinearThreshold;
        float m_splintersAutoDisableAngularThreshold;
        int m_splintersAutoDisableNumSteps;
        bool m_vehiclesDropChests;
        float m_maxSpeedWithNoFuel;
        float m_probabilityToGenerateDynamicQuestInTown;
        int m_infoAreaRadius;
        float m_lockTimeout;
        CVector2 m_unlockRegion;
        float m_infoObjUpdateTimeout;
        float m_blastWaveCameraShakeRadiusCoeff;
        float m_shakeDamageToDurationCoeff;
        float m_maxShakeDamage;
        float m_distanceFromPlayerToMoveout;
        float m_defaultLookBoxLength;
        float m_defaultTargetBoxLength;
        float m_attractiveCoeff;
        float m_repulsiveCoeff;
        float m_maxDistToAvoid;
        float m_predictionTime;
        float m_throwCoeff;
        float m_flowVpVelocity;
        float m_flowWheelVelocity;
        float m_energyBlowDeltaTime;
        int m_energyVpBlowProbability;
        int m_energyWheelBlowProbability;
        float m_healthUnitPrice;
        float m_defaultArticleRegenerationTime;
        float m_probabilityToDropArticlesFromDeadVehicles;
        float m_probabilityToDropGunsFromDeadVehicles;
        float m_zoneRespawnTimeOutIncreaseCoeff;
        float m_zoneDefaultFirstSpawnTime;
        unsigned int m_colorFriend;
        unsigned int m_colorEnemy;
        unsigned int m_colorTargetCaptured;
        float m_targetInfoContourWidth;
        float m_targetCapturedContourWidth;
        float m_playerPassMapUnpassableMu;
        float m_playerPassMapUnpassableErp;
        float m_playerPassMapUnpassableCfm;
        float m_maxGroupingAngle;
        float m_timeOutForReAimGuns;
        std::vector<CoeffsForDifficultyLevel> m_difficultyLevelCoeffs;
        float m_property2PriceCoeff;
    };
}
