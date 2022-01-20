#include "globalproperties.h"
#include <stdexcept>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <server/utils.h>

namespace ai
{
    GlobalProperties theGlobProp;

    GlobalProperties::CoeffsForDifficultyLevel::CoeffsForDifficultyLevel()
    {
    }

    GlobalProperties::GlobalProperties()
    {
    }

    GlobalProperties::CoeffsForDifficultyLevel const& GlobalProperties::GetCoeffsForCurrentDifficultyLevel() const
    {
        throw std::logic_error("Not implemented");
    }

    void GlobalProperties::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild_(node, "IzvratRepository");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            CVector2 vec;
            m3d::SafeVector2Attrib(vec, node, "MaxSize");
            m_izvratRepositoryMaxSize.x = vec.x;
            m_izvratRepositoryMaxSize.y = vec.y;
        }

        xmlNode->GetFirstChild_(node, "GroundRepository");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            CVector2 vec;
            m3d::SafeVector2Attrib(vec, node, "Size");
            m_groundRepositorySize.x = vec.x;
            m_groundRepositorySize.y = vec.y;
        }

        xmlNode->GetFirstChild_(node, "Belongs");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            CStr strBelongs;
            m3d::SafeStrAttrib(strBelongs, node, "Values");
            std::vector<int> namedBelongIdsVector;
            StrToIntVector(strBelongs, namedBelongIdsVector);
            for (auto const id : namedBelongIdsVector)
            {
                m_namedBelongIds.insert(id);
            }
        }

        xmlNode->GetFirstChild_(node, "Mult");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeFloatAttrib(m_gameTimeMult, node, "GameTimeMult");
            m3d::SafeFloatAttrib(m_vehicleAiFiringRangeMult, node, "VehicleAIFiringRangeMult");
        }
        M3D_ASSERT(m_gameTimeMult > 1e-4f);

        xmlNode->GetFirstChild_(node, "BurstParameters");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeUintAttrib(m_maxBurstTime, node, "MaxBurstTime");
            m3d::SafeUintAttrib(m_minBurstTime, node, "MinBurstTime");
            m3d::SafeUintAttrib(m_timeBetweenBursts, node, "TimeBetweenBursts");
        }

        xmlNode->GetFirstChild_(node, "DynamicQuest");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeFloatAttrib(m_probabilityToGenerateDynamicQuestInTown, node, "ProbabilityToGenerateDynamicQuestInTown");
            if (m_probabilityToGenerateDynamicQuestInTown < 0.0 || m_probabilityToGenerateDynamicQuestInTown > 1.0)
            {
                M3D_CRITICAL_ERROR("invalid ProbabilityToGenerateDynamicQuestInTown in GlobalProperties: " + CStr(m_probabilityToGenerateDynamicQuestInTown));
            }
        }

        xmlNode->GetFirstChild_(node, "CommonPaths");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeStrAttrib(m_pathToRelationship, node, "Relationship");
            m3d::SafeStrAttrib(m_pathToGameObjects, node, "GameObjects");
            m3d::SafeStrAttrib(m_pathToQuests, node, "Quests");
            m3d::SafeStrAttrib(m_pathToResourceTypes, node, "ResourceTypes");
            m3d::SafeStrAttrib(m_pathToAffixes, node, "Affixes");
            m3d::SafeStrAttrib(m_pathToVehiclePartTypes, node, "VehiclePartTypes");
        }

        xmlNode->GetFirstChild_(node, "Physics");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeFloatAttrib(m_distToTurnOnPhysics, node, "DistToTurnOnPhysics");
            m3d::SafeFloatAttrib(m_distToTurnOffPhysics, node, "DistToTurnOffPhysics");
            m3d::SafeFloatAttrib(m_physicStepTime, node, "PhysicStepTime");
        }
        M3D_ASSERT(m_distToTurnOnPhysics < m_distToTurnOffPhysics - 10.0f);

        xmlNode->GetFirstChild_(node, "Npc");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeStrAttrib(m_barmenModelName, node, "BarmenModelName");
        }

        xmlNode->GetFirstChild_(node, "BreakableObjectSplinters");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeFloatAttrib(m_splintersAutoDisableLinearThreshold, node, "AutoDisableLinearThreshold");
            m3d::SafeFloatAttrib(m_splintersAutoDisableAngularThreshold, node, "AutoDisableAngularThreshold");
            m3d::SafeIntAttrib(m_splintersAutoDisableNumSteps, node, "AutoDisableNumSteps");
        }

        xmlNode->GetFirstChild_(node, "Vehicles");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeBoolAttrib(m_vehiclesDropChests, node, "VehiclesDropChests");
            m3d::SafeFloatAttrib(m_maxSpeedWithNoFuel, node, "MaxSpeedWithNoFuel");
            m_maxSpeedWithNoFuel *= 0.27777779;
        }

        xmlNode->GetFirstChild_(node, "SmartCursor");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeIntAttrib(m_infoAreaRadius, node, "InfoAreaRadius");
            m3d::SafeFloatAttrib(m_lockTimeout, node, "LockTimeout");
            m3d::SafeVector2Attrib(m_unlockRegion, node, "UnlockRegion");
        }

        xmlNode->GetFirstChild_(node, "CameraController");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeFloatAttrib(m_blastWaveCameraShakeRadiusCoeff, node, "BlastWaveCameraShakeRadiusCoeff");
            m3d::SafeFloatAttrib(m_shakeDamageToDurationCoeff, node, "ShakeDamageToDurationCoeff");
            m3d::SafeFloatAttrib(m_maxShakeDamage, node, "MaxShakeDamage");
        }
        M3D_ASSERT(m_maxShakeDamage > 1.0f);

        xmlNode->GetFirstChild_(node, "Caravans");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeFloatAttrib(m_distanceFromPlayerToMoveout, node, "DistanceFromPlayerToMoveout");
        }

        xmlNode->GetFirstChild_(node, "ObstacleAvoidance");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeFloatAttrib(m_defaultLookBoxLength, node, "DefaultLookBoxLength");
            m3d::SafeFloatAttrib(m_defaultTargetBoxLength, node, "DefaultTargetBoxLength");
            m3d::SafeFloatAttrib(m_attractiveCoeff, node, "AttractiveCoeff");
            m3d::SafeFloatAttrib(m_repulsiveCoeff, node, "RepulsiveCoeff");
            m3d::SafeFloatAttrib(m_maxDistToAvoid, node, "MaxDistToAvoid");
            m3d::SafeFloatAttrib(m_predictionTime, node, "PredictionTime");
        }

        xmlNode->GetFirstChild_(node, "DeathProperties");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeFloatAttrib(m_throwCoeff, node, "ThrowCoeff");
            m3d::SafeFloatAttrib(m_flowVpVelocity, node, "FlowVpVelocity");
            m3d::SafeFloatAttrib(m_flowWheelVelocity, node, "FlowWheelVelocity");
            m3d::SafeFloatAttrib(m_energyBlowDeltaTime, node, "EnergyBlowDeltaTime");
            m3d::SafeIntAttrib(m_energyVpBlowProbability, node, "EnergyVpBlowProbability");
            m3d::SafeIntAttrib(m_energyWheelBlowProbability, node, "EnergyWheelBlowProbability");
        }

        xmlNode->GetFirstChild_(node, "Repair");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeFloatAttrib(m_healthUnitPrice, node, "HealthUnitPrice");
        }

        xmlNode->GetFirstChild_(node, "Articles");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeFloatAttrib(m_defaultArticleRegenerationTime, node, "DefaultRegenerationTime");
            m3d::SafeFloatAttrib(m_probabilityToDropArticlesFromDeadVehicles, node, "ProbabilityToDropArticlesFromDeadVehicles");
            m3d::SafeFloatAttrib(m_probabilityToDropGunsFromDeadVehicles, node, "ProbabilityToDropGunsFromDeadVehicles");
        }

        xmlNode->GetFirstChild_(node, "InfectionZones");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeFloatAttrib(m_zoneRespawnTimeOutIncreaseCoeff, node, "ZoneRespawnTimeOutIncreaseCoeff");
            m3d::SafeFloatAttrib(m_zoneDefaultFirstSpawnTime, node, "ZoneDefaultFirstSpawnTime");
        }

        xmlNode->GetFirstChild_(node, "InterfaceStuff");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeClrAttrib(m_colorFriend, node, "ColorFriend");
            m3d::SafeClrAttrib(m_colorEnemy, node, "ColorEnemy");
            m3d::SafeClrAttrib(m_colorTargetCaptured, node, "ColorTargetCaptured");
            m3d::SafeFloatAttrib(m_targetInfoContourWidth, node, "TargetInfoContourWidth");
            m3d::SafeFloatAttrib(m_targetCapturedContourWidth, node, "TargetCapturedContourWidth");
        }

        xmlNode->GetFirstChild_(node, "PlayerPassmap");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeFloatAttrib(m_playerPassMapUnpassableMu, node, "PlayerPassMapUnpassableMu");
            m3d::SafeFloatAttrib(m_playerPassMapUnpassableErp, node, "PlayerPassMapUnpassableErp");
            m3d::SafeFloatAttrib(m_playerPassMapUnpassableCfm, node, "PlayerPassMapUnpassableCfm");
        }

        xmlNode->GetFirstChild_(node, "Weapon");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            float fullGroupingAngleDegree = 0.0;
            m3d::SafeFloatAttrib(fullGroupingAngleDegree, node, "MaxGroupingAngle");
            m_maxGroupingAngle = (fullGroupingAngleDegree * 0.017453292) * 0.5;
            m3d::SafeFloatAttrib(m_timeOutForReAimGuns, node, "TimeOutForReAimGuns");
        }

        m_difficultyLevelCoeffs.clear();
        xmlNode->GetFirstChild_(node, "DifficultyLevels");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            ref_ptr temp = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            for (node->GetFirstChild_(temp, "Level"); !temp->IsEmpty(); temp->GetNextSibling_(temp, "Level"))
            {
                CoeffsForDifficultyLevel coeff;
                m3d::SafeStrAttrib(coeff.m_name, temp, "Name");
                m3d::SafeFloatAttrib(coeff.m_damageCoeffForPlayerFromEnemies, temp, "EnemyWeaponCoeff");
                m3d::SafeFloatAttrib(coeff.m_enemiesShootingDelay, temp, "EnemyShootingDelay");
                if (coeff.m_damageCoeffForPlayerFromEnemies < 0.0)
                {
                    M3D_CRITICAL_ERROR("invalid EnemyWeaponCoeff in GlobalProperties: " + CStr(coeff.m_damageCoeffForPlayerFromEnemies));
                }
                if (coeff.m_enemiesShootingDelay < 0.0 || coeff.m_enemiesShootingDelay > 3.0)
                {
                    M3D_CRITICAL_ERROR("invalid EnemyShootingDelay in GlobalProperties: " + CStr(coeff.m_enemiesShootingDelay));
                }
                m_difficultyLevelCoeffs.push_back(coeff);
            }
        }

        xmlNode->GetFirstChild_(node, "Price");
        if (!node->IsEmpty() && node->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            m3d::SafeFloatAttrib(m_property2PriceCoeff, node, "Property2PriceCoeff");
        }

        if (m_difficultyLevelCoeffs.empty())
        {
            M3D_CRITICAL_ERROR("no difficulty levels in GlobalProperties");
        }
    }
}
