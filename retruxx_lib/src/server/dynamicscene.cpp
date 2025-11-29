#include "dynamicscene.h"

#include <stdexcept>
#include <ode/collision.h>
#include <ode/collision_space.h>
#include <ode/objects.h>

#include "config.h"
#include "geomobject.h"
#include "level.h"
#include "passagedata.h"
#include "colliders/breakableobjectcolliders.h"
#include "colliders/bulletcolliders.h"
#include "colliders/colliderkrnl.h"
#include "colliders/defaultcollider.h"
#include "colliders/physicunitcolliders.h"
#include "colliders/vehiclecolliders.h"
#include "colliders/vehiclesplintercolliders.h"
#include "colliders/wheelasphaltcollider.h"
#include "objects/blastwave.h"
#include "objects/breakableobject.h"
#include "objects/dummyobject.h"
#include "objects/particlesplinter.h"
#include "objects/physicunit.h"
#include "objects/player.h"
#include "objects/staticautogun.h"
#include "objects/town.h"
#include "objects/vehiclesplinter.h"
#include "objects/wheel.h"
#include "objects/base/geomobj.h"
#include "objects/base/shell.h"
#include "objects/monsters/boss03part.h"
#include "objects/monsters/boss04part.h"
#include "objects/monsters/boss04stationpart.h"
#include "objects/monsters/bossmetalarmload.h"
#include "objects/physicbodies/vehiclepart.h"
#include "server.h"
#include "world.h"
#include "core/ini.h"
#include "core/log.h"
#include "core/profilerstack.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "game/m3dgame.h"
#include "objects/chassis.h"
#include "objects/dynamicquestdestroy.h"
#include "objects/vehicle.h"
#include "objects/base/globalproperties.h"
#include "objects/base/jointedobj.h"
#include "objects/base/prototypemanager.h"
#include "objects/guns/bullet.h"
#include "objects/guns/gun.h"
#include "objects/guns/mine.h"
#include "objects/guns/mortarshell.h"
#include <algorithm>
#include <client.h>

namespace ai
{
	extern Player* thePlayer;

	namespace
	{
		dxJointGroup* contactGroup = nullptr;
	    int numNearCallbacksLastFrame = 0;
        CStr const STANDARD_EXPLOSION = "ET_PS_EXPLOSION";
	}

	class ShellTraceLineCallback : public TraceLineCallback
    {
    private:
        /* 0x0004 */ const ai::Bullet& m_Bullet;

    public:
        ShellTraceLineCallback(const ai::Bullet& bullet) : m_Bullet(bullet)
        {
        }

        virtual bool CollideId(int objId) const override /* 0x04 */
        {
            return objId != m_Bullet.GetEmittedObjId();
        }
        virtual bool CollidePhysicObj(const ai::PhysicObj* physicObj) const override /* 0x08 */
        {
            if (IS_KIND_OF(physicObj, Shell))
            {
                auto* shell = RT_DYNCAST(physicObj, const Shell);
                return shell->GetEmittedObjId() != m_Bullet.GetEmittedObjId();
            }
            return true;
        }
    }; /* size: 0x0008 */

	int FillDefaultContactParameters(dContact* contacts, unsigned int numContacts)
	{
		// TODO: check and refactor this
		if (numContacts)
		{
			auto p_slip1 = &contacts->surface.slip1;
			auto v3 = numContacts;
			do
			{
				*((int*)p_slip1 - 9) = 12312;
				*(p_slip1 - 8) = 0.80000001;
				*p_slip1 = 0.0000099999997;
				p_slip1[1] = 0.0000099999997;
				*(p_slip1 - 4) = 0.80000001;
				*(p_slip1 - 3) = 0.0000049999999;
				*(p_slip1 - 6) = 0.001;
				*(p_slip1 - 5) = 0.0099999998;
				p_slip1 += 26;
				--v3;
			} while (v3);
		}
		return 1;
	}

    void NearCallback(void* data, dxGeom* geom1, dxGeom* geom2)
	{
		// TODO: generated code
		if (!dGeomIsEnabled(geom1) || !dGeomIsEnabled(geom2))
			return;

		// Handle space collisions recursively
		if (dGeomIsSpace(geom1) || dGeomIsSpace(geom2))
		{
			if (geom1 != geom2)
			{
				dSpaceCollide2(geom1, geom2, data, NearCallback);
			}
			return;
		}

		// Get bodies for the geometries
		dGeomGetBody(geom1);
		dGeomGetBody(geom2);

		++numNearCallbacksLastFrame;

		// Initialize contact array
		dContact contacts[32];

		// Get objects associated with geometries
		m3d::Object* obj1 = (m3d::Object*)dGeomGetData(geom1);
		m3d::Object* obj2 = (m3d::Object*)dGeomGetData(geom2);

		// Cast to PhysicBody if possible
		PhysicBody* body1 = nullptr;
		PhysicBody* body2 = nullptr;

		if (obj1 && obj1->IsKindOf(&PhysicBody::m_classPhysicBody))
		{
			body1 = static_cast<PhysicBody*>(obj1);
		}

		if (obj2 && obj2->IsKindOf(&PhysicBody::m_classPhysicBody))
		{
			body2 = static_cast<PhysicBody*>(obj2);
		}

		// Get owners of physic bodies
		PhysicObj* owner1 = body1 ? body1->GetOwner() : nullptr;
		PhysicObj* owner2 = body2 ? body2->GetOwner() : nullptr;

		// Update object pointers to use owners when appropriate
		if (body1) obj1 = body1;
		if (body2) obj2 = body2;

		// Use owner objects instead of vehicle parts
		if (owner1 && !body1->IsKindOf(&VehiclePart::m_classVehiclePart))
		{
			obj1 = owner1;
		}

		if (owner2 && !body2->IsKindOf(&VehiclePart::m_classVehiclePart))
		{
			obj2 = owner2;
		}

		// Get class types
		m3d::Class* class1 = obj1 ? obj1->GetClass() : nullptr;
		m3d::Class* class2 = obj2 ? obj2->GetClass() : nullptr;

		// Check if collision should be processed
		if (!ColliderKrnl::MustCheckForCollision(obj1, obj2)) {
			return;
		}

		// Determine if collision should proceed based on object types and worlds
		bool shouldCollide = false;

		if ((class1 && class1->IsKindOf(&Shell::m_classShell)) ||
			(class2 && class2->IsKindOf(&Shell::m_classShell)))
		{
			shouldCollide = true;
		}
		else
		{
			// Check if both geometries are in the same world (global world)
			dxWorld* world1 = nullptr;
			dxWorld* world2 = nullptr;

			dxBody* body1 = dGeomGetBody(geom1);
			dxBody* body2 = dGeomGetBody(geom2);

			world1 = body1 ? dBodyGetWorld(body1) : gGlobalWorld;
			world2 = body2 ? dBodyGetWorld(body2) : gGlobalWorld;

			shouldCollide = (world1 == gGlobalWorld && world2 == gGlobalWorld);
		}

		if (!shouldCollide)
		{
			return;
		}

		// Perform collision detection
		unsigned int numContacts = dCollide(geom1, geom2, 32, &contacts[0].geom, sizeof(dContact));

		if (numContacts == 0)
		{
			return;
		}

		// Handle bullet collisions specially (find closest contact point)
		if ((owner1 && owner1->GetClass() == &Bullet::m_classBullet) ||
			(owner2 && owner2->GetClass() == &Bullet::m_classBullet))
		{

			Bullet* bullet = nullptr;
			if (owner1 && owner1->GetClass() == &Bullet::m_classBullet)
			{
				bullet = static_cast<Bullet*>(owner1);
			}
			else if (owner2 && owner2->GetClass() == &Bullet::m_classBullet)
			{
				bullet = static_cast<Bullet*>(owner2);
			}

			if (bullet)
			{
				Ray* bulletRay = bullet->_Ray();
				CVector bulletPos = bulletRay->GetPosition();

				// Find closest contact point to bullet
				unsigned int closestContactIndex = 0;
				float minDistanceSq = 1.0e30f;

				for (unsigned int i = 0; i < numContacts; ++i)
				{
					CVector contactPos;
					contactPos.x = contacts[i].geom.pos[0];
					contactPos.y = contacts[i].geom.pos[1];
					contactPos.z = contacts[i].geom.pos[2];

					CVector diff = bulletPos - contactPos;
					float distanceSq = diff.lengthSq();

					if (distanceSq < minDistanceSq) {
						minDistanceSq = distanceSq;
						closestContactIndex = i;
					}
				}

				// Use only the closest contact for bullets
				if (closestContactIndex != 0)
				{
					contacts[0] = contacts[closestContactIndex];
				}
				numContacts = 1;
			}
		}

		// Fill default contact parameters and check collision
		FillDefaultContactParameters(contacts, numContacts);

		if (!ColliderKrnl::CollideObjs(obj1, obj2, contacts, numContacts))
		{
			return;
		}

		// Create contact joints
		for (unsigned int i = 0; i < numContacts; ++i)
		{
			dxJoint* contactJoint = dJointCreateContact(gGlobalWorld,
														contactGroup,
														&contacts[i]);

			dxBody* body1 = dGeomGetLinkedBody(geom1);
			dxBody* body2 = dGeomGetLinkedBody(geom2);

			dJointAttach(contactJoint, body1, body2);
		}
	}

	RT_CLASS_EXPORTS_BEGIN(DynamicScene)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(DynamicScene);

	void DynamicScene::SoilProps::LoadFromXml(m3d::cmn::XmlNode const* xmlNode)
	{
        m3d::SafeStrAttrib(m_splashTypeName, xmlNode, "splash");
        m3d::SafeFloatAttrib(m_friction, xmlNode, "friction");
        m3d::SafeFloatAttrib(m_resistance, xmlNode, "resistance");
        m3d::SafeStrAttrib(m_wheelTraceTextureName, xmlNode, "wheeltracetexture");
	}

	DynamicScene::SoilProps::SoilProps()
	{
		m_wheelTraceTextureName = M3D_ENGINE_CFG.m_skidTexName.GetS();
		m_friction = 1.0;
		m_resistance = 0;
		m_idx = 0;
	}

	short DynamicScene::GetBoEffectTypeByName(CStr const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void DynamicScene::DeleteAll()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void DynamicScene::PurgeBodies()
	{
        if (thePlayer)
        {
            auto vehicle = thePlayer->GetVehicle();
            if (!vehicle
                || ((vehicle->GetFlags() & 8) != 0)
                || (vehicle->GetFlags() & 2) != 0
                || vehicle->GetParentRepository())
            {
                M3D_APP->ImmediateMessage(66544, 0, 0, 0, 0, {}, {});
                thePlayer->CauseEvent(GE_PLAYER_VEHICLE_CHANGED, 0.0, {}, {});
            }
        }
        theObjects->Purge();
	}

	CStr const& DynamicScene::GetBoEffectTypeName(unsigned short)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* DynamicScene::Clone()
	{
		return Object::Clone();
	}

	void DynamicScene::LinkNodesFromBodyToSceneGraph(Obj* pObj)
	{
		if (pObj)
		{
			retruxx::list<m3d::SgNode*> nodelist;
			pObj->ReceiveNodesToLink(nodelist);
			for (auto& node : nodelist)
			{
				node->UpdateXForm(true, false);
				ai::pServer->GetWorld()->GetGraph().LinkNode(node);
            }
		}
	}

	int DynamicScene::ProcessShellAndBody(Shell* shell, PhysicBody* body, dContact* contact, unsigned& numContacts, bool reverse)
	{
		// TODO: check all this shit!!!
		using namespace m3d;

        if (!numContacts)
        {
            return 0;
        }

        contact->geom.depth = 0.0;

		if (!shell || !shell->IsAlive())
        {
            return 0;
        }

		auto const* gunPrototypeInfo = shell->GetGunPrototypeInfo();
        auto const* gun = shell->GetGun();
        auto const attackerId = shell->GetEmittedObjId();
        auto* attackerObj = RT_DYNCAST(theObjects->GetEntityByObjId(attackerId), PhysicObj);

		if (!gunPrototypeInfo || IS_KIND_OF(shell, Mine))
        {
            auto* mine = RT_DYNCAST(shell, Mine);
            if (mine->getState() == Mine::msActivation || IS_KIND_OF(body, GeomObject))
            {
                return 0;
            }
        }
        if (IS_KIND_OF(shell, Rocket))
        {
            auto* rocket = RT_DYNCAST(shell, Rocket);
            rocket->CreateBlastWave();
        }
        else if (IS_KIND_OF(shell, MortarShell))
        {
            auto* mortarShell = RT_DYNCAST(shell, MortarShell);
            mortarShell->CreateBlastWave();
        }
        else if (IS_KIND_OF(shell, Bullet))
        {
            auto* bullet = RT_DYNCAST(shell, Bullet);
            CVector pos;
            pos.x = contact->geom.pos[0];
            pos.y = contact->geom.pos[1];
            pos.z = contact->geom.pos[2];
            bullet->SpecifyTracer(pos);
        }

		bool isShellHit = false;
		PhysicObj* hitPhysicObj = nullptr;
		if (body && IS_KIND_OF(body, PhysicBody))
        {
            auto* physBody = RT_DYNCAST(body, PhysicBody);
            hitPhysicObj = physBody->GetOwner();

		}
        else
        {
            isShellHit = true;
            if (body && IS_KIND_OF(body, PhysicObj))
            {
                hitPhysicObj = RT_DYNCAST(body, PhysicObj);
                if (IS_KIND_OF(body, Wheel))
                {
                    auto* wheel = RT_DYNCAST(body, Wheel);
                    auto* vehicle = wheel->GetVehicle();
                    if (vehicle)
                    {
                        hitPhysicObj = vehicle;
                        body = vehicle->GetChassis();
                    }
                    wheel->BreakModel();
                }
            }
        }

		auto const gunObjId = shell->GetGunObjId();
        CVector contactPos;
        contactPos.x = contact->geom.pos[0];
        contactPos.y = contact->geom.pos[1];
        contactPos.z = contact->geom.pos[2];

		gunPrototypeInfo->CreateBlastWave(contactPos, gunObjId);
        if (hitPhysicObj && (hitPhysicObj->GetId() != shell->GetEmittedObjId() || IS_KIND_OF(shell, Mine)))
        {
            if (hitPhysicObj->GetBelong() != shell->GetBelong())
            {
                CStr partName;
                if (IS_KIND_OF(hitPhysicObj, ComplexPhysicObj))
                {
                    auto* chassis = RT_DYNCAST(body, Chassis);
                    partName = chassis->GetPartName();

                    auto* vehicle = RT_DYNCAST(chassis->GetOwner(), Vehicle);
                    M3D_ASSERT(hitPhysicObj == vehicle);
                    M3D_ASSERT(vehicle->GetPartByName(partName));

					auto* gun = shell->GetGun();
					if (gun)
					{
					    // TODO: increase statistics
					}
                }

				DamageInfo info;
                info.hitDir = shell->GetDirection().getNormalized();
                info.attackingAgentId = shell->GetId();
                info.attackerId = shell->GetEmittedObjId();
                info.damageType = gunPrototypeInfo->m_damageType;
                info.decalId = gunPrototypeInfo->m_decalId;
                info.gunPrototypeId = gunPrototypeInfo->m_prototypeId;
                info.damagedPartName = partName;


				float damage = 0.0;
                auto* gun = shell->GetGun();
				if (gun)
				{
                    damage = gun->GetDamageForOneShell();
				}
                else
                {
                    damage = gunPrototypeInfo->GetDamageForOneShell();
                }

				if (IS_KIND_OF(shell, Bullet) || !IS_KIND_OF(body, PhysicBody))
                {
                    info.hitPos.x = contact->geom.pos[0];
                    info.hitPos.y = contact->geom.pos[1];
                    info.hitPos.z = contact->geom.pos[2];

					info.normal.x = contact->geom.normal[0];
                    info.normal.y = contact->geom.normal[1];
                    info.normal.z = contact->geom.normal[2];
                }
                else
                {
                    RETRUXX_NOT_IMPLEMENTED;
                }
                hitPhysicObj->InflictDamage(info);
            }
        }
		// TODO: check this!!!!
        else if (!isShellHit)
        {
            return 0;
        }

		shell->Remove();

		if (IS_KIND_OF(shell, Rocket))
        {
            RETRUXX_NOT_IMPLEMENTED;
		}
		// TODO: check this!!!
        if (body && (!hitPhysicObj || hitPhysicObj->bIsUpdatingByODE()) && (!attackerObj || attackerObj->bIsUpdatingByODE()))
        {
            if (IS_KIND_OF(body, GeomObjectLandscape))
            {
                auto const landSize = 4 * ai::pServer->GetWorld()->m_level->land_size;
                auto const levelSize = pServer->GetLevelSize();
                auto const scale = levelSize / landSize;

				int gridX = static_cast<int>((contact->geom.pos[0] * (1.0 / scale)) + 0.5f);
                int gridZ = static_cast<int>((contact->geom.pos[2] * (1.0 / scale)) + 0.5f);

                SoilProps const& soilProps = ai::gDynamicScene->GetSoilProps(gridX, gridZ);
                unsigned short splashType = soilProps.m_splashType;
                unsigned short explosionType = gunPrototypeInfo->m_explosionType;

				CVector pos;
                pos.x = contact->geom.pos[0];
                pos.y = contact->geom.pos[1];
                pos.z = contact->geom.pos[2];

                CStr const& effectName = ai::gDynamicScene->GetShellEffectName(explosionType, splashType);
                PhysicBody::CreateEffectNode(
                    effectName, pos,
                    IdentityQuaternion,
                    true,
                    1.0f);
            }
            else if (IS_KIND_OF(body, VehiclePart))
            {
                auto* part = RT_DYNCAST(body, VehiclePart);
                // Vehicle part impact effect
                if (part->m_Node)
                {
                    CStr const& vehicleEffectName = ai::gDynamicScene->GetShellVehicleEffectName(gunPrototypeInfo->m_explosionType);

                    CVector scale(1.0f, 1.0f, 1.0f);
                    m3d::SgNode* effectNode = PhysicBody::CreateNode(vehicleEffectName, 0, scale, 0, 0);

                    part->m_Node->AddChild(effectNode);
                    effectNode->SetOriginAbs(part->GetLastHitPos());
                    effectNode->UpdateXForm(0, true);
                    effectNode->GetGraph()->InsertInRemoveIfFree(effectNode);
                }
            }
            else
            {
                // Other object types impact effects
                CStr effectName;
                if (IS_KIND_OF(body,GeomObjectRoad))
                {
                    effectName = ai::gDynamicScene->GetShellRoadEffectName(gunPrototypeInfo->m_explosionType);
                }
                else if (IS_KIND_OF(body, GeomObjectStatics))
                {
                    effectName = ai::gDynamicScene->GetShellStaticsEffectName(gunPrototypeInfo->m_explosionType);
                }
                else if (IS_KIND_OF(body, SimplePhysicObj))
                {
                    effectName = ai::gDynamicScene->GetShellVehicleEffectName(gunPrototypeInfo->m_explosionType);
                }

				CVector pos;
                pos.x = contact->geom.pos[0];
                pos.y = contact->geom.pos[1];
                pos.z = contact->geom.pos[2];

                PhysicBody::CreateEffectNode(
                    effectName, pos,
                    IdentityQuaternion,
                    true,
                    1.0f);
            }
        }
        return 1;
	}

	void DynamicScene::InitClashDecalId()
	{
		m_clashDecalId = AddDecalName("DC_CLASH");
	}

	void DynamicScene::ReadSoilProps(char const* fileName)
	{
		// TODO: check this!!!
        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!stream->Open(fileName, m3d::fs::IStream::OPEN_READ))
        {
            M3D_LOG_ERR("[Error] DynamicScene::ReadSoilProps : Can't open file " + CStr(fileName));
            return;
        }

        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        if (!xmlFile->Read(*stream))
        {
            M3D_LOG_ERR("[Error] DynamicScene::ReadSoilProps : Can't read file " + CStr(fileName));
            return;
        }

        ref_ptr tilepropsNode = xmlFile->CreateNode();
        xmlFile->GetFirstChild(tilepropsNode, "tileprops");
        if (tilepropsNode->IsEmpty())
        {
            return;
        }

		std::map<CStr, SoilProps> soilProps;
        ref_ptr typesNode = xmlFile->CreateNode();
        tilepropsNode->GetFirstChild(typesNode, "types");
        if (!typesNode->IsEmpty())
		{
			ref_ptr typeNode = xmlFile->CreateNode();
			typesNode->GetFirstChild(typeNode, "type");
			while (!typeNode->IsEmpty())
			{
                CStr typeName = typeNode->GetAttribute("name");
				SoilProps props;
				props.LoadFromXml(typeNode);
                soilProps[typeName] = props;
				typeNode->GetNextSibling(typeNode, "type");
			}
        }

		m_soilSplashTypeNames.clear();

		auto& landscape = ai::pServer->GetWorld()->GetLandscape();
        auto const numTiles = landscape.GetNumTiles();
        m_soilSplashTypeNames.resize(numTiles);
        m_soilProps.resize(numTiles);

		ref_ptr tilesNode = xmlFile->CreateNode();
        tilepropsNode->GetFirstChild(tilesNode, "tiles");
        ref_ptr texPropsNode = xmlFile->CreateNode();
        for (int i = 0; i < numTiles; ++i)
		{
            CStr texName;
            auto texHandle = landscape.GetTexHandleFromList(i);
            if (texHandle.IsValid())
            {
                CStr texFileName;
                M3D_RENDERER->GetTextureName(texHandle, texFileName);

                CStr const texFullName = NameFromFileName(texFileName);
                texName = texFullName.substr(0, texFullName.find('.'));
            }

			CStr tileType;
			tilesNode->GetFirstChild(texPropsNode, texName.c_str());
            if (texPropsNode->IsEmpty())
            {
                M3D_LOG_ERR("Error: couldn't load tile props for tile '" + texName + "'");
                tileType = "GRASS_GENERAL";
            }
			else
			{
                m3d::SafeStrAttrib(tileType, texPropsNode, "type");
			}

			auto const soilIt = soilProps.find(tileType);
            if (soilIt == soilProps.end())
            {
                M3D_LOG_ERR("Error: invalid tile type: '" + tileType + "'" + "' for texture '" + texName + "'");
                M3D_ASSERT(!"Error reading tileprops, see log");
            }

			auto const& soilProp = soilIt->second;
			m_soilProps[i] = soilProp;
            m_soilProps[i].LoadFromXml(texPropsNode);
            m_soilProps[i].m_idx = i;
            m_soilSplashTypeNames[i] = m_soilProps[i].m_splashTypeName;
        }

		std::sort(
			m_soilSplashTypeNames.begin(), m_soilSplashTypeNames.end());
        auto const last = std::unique(m_soilSplashTypeNames.begin(), m_soilSplashTypeNames.end());
        m_soilSplashTypeNames.erase(last, m_soilSplashTypeNames.end());


		// TODO: check this!!!
		// Update splash type indices
        for (size_t i = 0; i < m_soilProps.size(); ++i)
        {
            auto it = std::find(m_soilSplashTypeNames.begin(), m_soilSplashTypeNames.end(), m_soilProps[i].m_splashTypeName);
            if (it != m_soilSplashTypeNames.end())
            {
                m_soilProps[i].m_splashType = static_cast<int>(std::distance(m_soilSplashTypeNames.begin(), it));
            }
        }

        // Initialize wheel traces
        _InitWheelTraces();

        // Build soil properties index map
        size_t tileSize = landscape.GetTileSize();
        m_soilPropsIdx.resize(tileSize);

        for (size_t y = 0; y < tileSize; ++y)
        {
            for (size_t x = 0; x < tileSize; ++x)
            {
                m3d::Landscape::TileInfo const& tileInfo = landscape.GetTileInfo(static_cast<int>(y), static_cast<int>(x));
                uint16_t texIndex = tileInfo.m_texIndex0;

                size_t index = y * tileSize + x;
                if (index < m_soilPropsIdx.size())
                {
                    m_soilPropsIdx[index].push_back(texIndex);
                }
            }
        }

        // Generate vehicle soil effect names
        m_vehicleSoilEffectNames.resize(m_soilProps.size());
        for (size_t i = 0; i < m_soilProps.size(); ++i)
        {
            CStr effectName = "ET_PS_" + m_soilProps[i].m_splashTypeName + "SPARKLE";
            m_vehicleSoilEffectNames[i] = effectName;
        }

        // Initialize effect names
        m_soilEffectNames.clear();
        m_roadEffectNames.clear();

        for (CStr const& wheelTypeName : m_wheelTypeNames)
        {
            _AddSoilEffectNameForWheelTypeName(wheelTypeName);
        }
	}

	CStr const& DynamicScene::GetShellWaterEffectName(unsigned short) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool DynamicScene::LoadSceneFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* rootNode, retruxx::vector<m3d::Class*> const& allowedClasses)
	{
        // TODO: generated code
        // Validate allowed classes
        if (allowedClasses.empty())
        {
            M3D_ASSERT(ai::theObjects->empty());
        }

        // Create XML node for parsing
        ref_ptr xmlNode = xmlFile->CreateNode();

        if (rootNode->IsEmpty())
        {
            return false;
        }

        // Reset physic time accumulator
        this->m_physicTimeAccumulator = 0.0;

        // Load physic time accumulator if doing full save
        if (ai::theObjects->m_SaveType == ObjContainer::SAVE_FULL)
        {
            m3d::SafeFloatAttrib(m_physicTimeAccumulator, rootNode, "PhysicTimeAccumulator");
        }

        // Load relationships
        rootNode->GetFirstChild(xmlNode, "relationship");
        if (!xmlNode->IsEmpty() && xmlNode->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            // Clean up existing relationship
            delete theRelationship;
            theRelationship = nullptr;

            // Create and load new relationship
            ai::theRelationship = new ai::Relationship();
            ai::theRelationship->LoadFromXML(xmlFile, xmlNode);
            ai::theRelationship->LoadDefaultFromXmlFile(ai::theGlobProp.m_pathToRelationship.c_str());
        }

        // Load target names for destruction
        rootNode->GetFirstChild(xmlNode, "TargetNamesForDestroy");
        ai::DynamicQuestDestroy::LoadNamesForTargetsFromXml(xmlNode);

        // Load runtime object container state for full save
        if (ai::theObjects->m_SaveType == ObjContainer::SAVE_FULL)
        {
            rootNode->GetFirstChild(xmlNode, "ObjContainerRuntime");
            if (!xmlNode->IsEmpty())
            {
                theObjects->LoadNodeStatesFromXml(xmlFile, xmlNode);
            }
        }

        // Process passage data if exists
        if (ai::thePassageData)
        {
            thePassageData->PutPassedObjectsToObjContainer();
        }

        // Validate event recipients for all objects
        for (auto* obj : *theObjects)
        {
            obj->ValidateEventRecipientsList();
        }

        // Deny object creation during loading for full save
        if (ai::theObjects->m_SaveType == ObjContainer::SAVE_FULL)
        {
            theObjects->DenyCreation();
        }

        // Load objects from XML
        rootNode->GetFirstChild(xmlNode, "Object");
        while (!xmlNode->IsEmpty())
        {
            this->ReadNewObjectFromXml(xmlFile, xmlNode, allowedClasses);
            xmlNode->GetNextSibling(xmlNode, "Object");
        }

        // Permit creation and purge objects
        if (ai::theObjects->m_SaveType == ObjContainer::SAVE_FULL)
        {
            theObjects->PermitCreation();
        }
        theObjects->Purge();

        // Create player if doesn't exist
        if (!ai::thePlayer)
        {
            CStr prototypeName("player");
            int prototypeId = thePrototypeManager->GetPrototypeId(prototypeName);
            int objId = theObjects->CreateNewObject(prototypeId, "Player1", -1, -1);

            ai::thePlayer = dynamic_cast<ai::Player*>(ai::theObjects->GetEntityByObjId(objId));
            thePlayer->SetBelong(1100);
        }

        // Set player name properly
        CStr playerName(ai::thePlayer->GetName());
        CStr tempName("random name which cannot be used !$%");

        theObjects->SetObjName(ai::thePlayer->GetId(), tempName);
        theObjects->SetObjName(ai::thePlayer->GetId(), playerName);

        // Handle player vehicle naming
        ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
        if (vehicle)
        {
            CStr vehicleName(vehicle->GetName());
            CStr tempVehicleName("random name which cannot be used !$%");

            theObjects->SetObjName(vehicle->GetId(), tempVehicleName);
            theObjects->SetObjName(vehicle->GetId(), vehicleName);
        }

        // Register player as global
        m3d::g_Kernel->UnRegisterGlobal("g_Player");
        m3d::g_Kernel->RegisterGlobal(ai::thePlayer, "g_Player");

        // Post-load processing for all objects
        for (auto* obj : *theObjects)
        {
            obj->PostLoad();

            // Send immediate messages based on object flags
            if (obj->GetFlags() & 2)
            { // Some specific flag
                M3D_APP->ImmediateMessage(66542, (int)obj, 0, 0, 0, {}, {});
            }

            if (obj->GetFlags() & 8)
            {
                M3D_APP->ImmediateMessage(66542, (int)obj, 0, 0, 0, {}, {});
            }

        }

        // Create visual parts for objects that need them
        for (auto* obj : *theObjects)
        {
            if ((obj->GetFlags() & 2) == 0)
            {
                obj->CreateVisualPart();
            }
        }

        // Clean up passage data
        if (ai::thePassageData)
        {
            thePassageData->SetPositionToPlayerVehicle();
            delete ai::thePassageData;
            ai::thePassageData = nullptr;
        }

        // Send completion message
        M3D_APP->EnqueueMessage(66544, 0, 0, 0, 0, {}, {});

        // Load last ID
        int lastId = 0;
        m3d::SafeIntAttrib(lastId, rootNode, "LastId");
        ai::pServer->SetLastId(lastId);

        return 1;
	}

	void DynamicScene::StepScene(float elapsedTime)
	{
        if (elapsedTime >= 0.0001)
        {
            ai::pServer->GetTmpProfiler()->StartCountdown();
            dWorldQuickStep(ai::gGlobalWorld, elapsedTime);
            ai::pServer->GetTmpProfiler()->EndCountdown();
            dJointGroupEmpty(contactGroup);
        }
	}

	bool DynamicScene::SaveSceneToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	DynamicScene::SoilProps const& DynamicScene::GetSoilProps(unsigned x, unsigned z) const
	{
		if (x < m_soilPropsIdx.size() && z < m_soilPropsIdx[x].size())
		{
			return m_soilProps[m_soilPropsIdx[x][z]];
		}

		static const DynamicScene::SoilProps dummy;
		return dummy;
	}

	CStr const& DynamicScene::GetShellStaticsEffectName(unsigned short) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CStr const STANDARD_GROUNDSPLASH = "ET_PS_GROUNDSPLASH";

	CStr const& DynamicScene::GetSoilEffectName(unsigned wheelType, unsigned short soilType, bool bVehicleIsBraking) const
	{
        if (m_soilEffectNames.empty())
        {
            return STANDARD_GROUNDSPLASH;
        }

		auto const idx = 2 * soilType + bVehicleIsBraking;
		if (wheelType < m_soilEffectNames.size() && idx < m_soilEffectNames[wheelType].size())
		{
            return m_soilEffectNames[wheelType][idx];
		}
        return STANDARD_GROUNDSPLASH;
	}

	void DynamicScene::RenderDebugInfo()
	{
		// TODO: implement DynamicScene::RenderDebugInfo
        // RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* DynamicScene::CreateObject()
	{
		return new DynamicScene;
	}

	CStr const& DynamicScene::GetBoVehicleEffectName(unsigned short) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void DynamicScene::CollideScene(float elapsedTime)
	{
        if (elapsedTime >= 0.0001)
        {
            numNearCallbacksLastFrame = 0;
            ai::pServer->GetCollideProfiler()->StartCountdown();

            dSpaceCollide(ai::gGlobalSpace, 0, ai::NearCallback);
            
            ai::pServer->GetCollideProfiler()->EndCountdown();

            theObjects->PostCollide();
        }
	}

	CStr const& DynamicScene::GetShellVehicleEffectName(unsigned short) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void DynamicScene::CreateBoShellEffectNames()
	{
		// TODO: generated code
        // Clear existing effect names
        m_BoShellEffectNames.clear();

        // For each effect type
        for (size_t effectTypeIndex = 0; effectTypeIndex < m_BoEffectTypeNames.size(); ++effectTypeIndex)
        {
            const CStr& effectType = m_BoEffectTypeNames[effectTypeIndex];

            // Create a new vector for this effect type with the same size as shell types
            std::vector<CStr> effectNames;
            effectNames.resize(m_shellTypesNames.size());

            // Add the new vector to the main container
            m_BoShellEffectNames.push_back(effectNames);

            // For each shell type, generate the effect name
            for (size_t shellTypeIndex = 0; shellTypeIndex < m_shellTypesNames.size(); ++shellTypeIndex)
            {
                const CStr& shellType = m_shellTypesNames[shellTypeIndex];

                // Build effect name: "ET_PS_" + shellTypeName + effectTypeName + "HIT"
                CStr effectName = "ET_PS_";
                effectName += shellType;
                effectName += effectType;
                effectName += "HIT";

                // Store the generated effect name
                m_BoShellEffectNames[effectTypeIndex][shellTypeIndex] = effectName;
            }
        }
	}

	CStr const& DynamicScene::GetVehicleSoilEffectName(unsigned short) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Class* DynamicScene::GetBaseClass()
	{
		return RT_CLASS_LOCAL(Object);
	}

	CStr const& DynamicScene::GetDecalName(int)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void DynamicScene::ClearOnce()
	{
		if (contactGroup)
			dJointGroupDestroy(contactGroup);
		contactGroup = 0;
		if (ai::gGlobalWorld)
			dWorldDestroy(ai::gGlobalWorld);
		ai::gGlobalWorld = 0;
		if (ai::gBulletSpace)
			dSpaceDestroy(ai::gBulletSpace);
		if (ai::gGlobalSpace)
			dSpaceDestroy(ai::gGlobalSpace);
		if (ai::gTempSpace)
			dSpaceDestroy(ai::gTempSpace);
		if (ai::gIntersectionSpace)
			dSpaceDestroy(ai::gIntersectionSpace);
		if (ai::gSpaceForAllPhysicParticles)
			dSpaceDestroy(ai::gSpaceForAllPhysicParticles);
		ai::gBulletSpace = 0;
		ai::gGlobalSpace = 0;
		ai::gTempSpace = 0;
		ai::gIntersectionSpace = 0;
		ai::gSpaceForAllPhysicParticles = 0;
	}

	int DynamicScene::GetNumNearCallbacksLastFrame()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int DynamicScene::AddDecalName(CStr const& name)
	{
		for (int i = 0; i < m_decalsNames.size(); ++i)
		{
		    if (m_decalsNames[i] == name)
		    {
				return i;
		    }
		}
		m_decalsNames.push_back(name);
		return m_decalsNames.size() - 1;
	}

	int DynamicScene::GetClashDecalId()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void DynamicScene::InitOnce()
	{
		ai::gGlobalSpace = dHashSpaceCreate(0);
		ai::gTempSpace = dHashSpaceCreate(0);
		ai::gBulletSpace = dHashSpaceCreate(ai::gGlobalSpace);
		dSpaceSetCleanup(ai::gGlobalSpace, 0);
		dSpaceSetCleanup(ai::gTempSpace, 0);
		dSpaceSetCleanup(ai::gBulletSpace, 0);
		ai::gIntersectionSpace = dSimpleSpaceCreate(0);
		dSpaceSetCleanup(ai::gIntersectionSpace, 0);
		ai::gSpaceForAllPhysicParticles = dSimpleSpaceCreate(ai::gGlobalSpace);
		dSpaceSetCleanup(ai::gSpaceForAllPhysicParticles, 0);
		ai::gGlobalWorld = dWorldCreate();
		dWorldSetGravity(ai::gGlobalWorld, 0.0, -9.8100004, 0.0);
		dWorldSetContactMaxCorrectingVel(ai::gGlobalWorld, 10.0);
		dWorldSetContactSurfaceLayer(ai::gGlobalWorld, 0.0099999998);
		dWorldSetCFM(ai::gGlobalWorld, 0.000099999997);
		dWorldSetERP(ai::gGlobalWorld, 0.1);
		dWorldSetQuickStepW(ai::gGlobalWorld, 1.0);
		dWorldSetAutoDisableFlag(ai::gGlobalWorld, 0);
		dWorldSetAutoDisableLinearThreshold(ai::gGlobalWorld, 0.1);
		dWorldSetAutoDisableAngularThreshold(ai::gGlobalWorld, 0.1);
		dWorldSetAutoDisableSteps(ai::gGlobalWorld, 5);
		dWorldSetAutoDisableTime(ai::gGlobalWorld, 0.0);
		dWorldSetDampingFlag(ai::gGlobalWorld, 1);
		dWorldSetDampingParameters(ai::gGlobalWorld, 0.1, 0.30000001);
		dWorldSetQuickStepNumIterations(ai::gGlobalWorld, 5);
		ai:contactGroup = dJointGroupCreate(0);
	}

	DynamicScene::~DynamicScene()
	{
		ColliderKrnl::Release();
	}

	unsigned DynamicScene::GetWheelTypeByName(CStr const& wheelTypeName)
	{
		int i = 0;
		for (; i < m_wheelTypeNames.size(); ++i)
		{
		    if (m_wheelTypeNames[i] == wheelTypeName)
		    {
				return i;
		    }
		}

		m_wheelTypeNames.push_back(wheelTypeName);
		_AddSoilEffectNameForWheelTypeName(wheelTypeName);
		return m_wheelTypeNames.size() - 1;
	}

	CStr const& DynamicScene::GetBoShellEffectName(unsigned short, unsigned short)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void DynamicScene::CollideBullet(Bullet const& bullet)
	{
		// TODO: check this
        static scoped_ptr bulletCollideRay = ai::Ray::CreateObject(nullptr, 1.0, nullptr);

	    auto* ray = bullet._Ray();

        const auto pos = bullet.GetPosition();
        dGeomSetPosition(bulletCollideRay->GetGeomId(), pos.x, pos.y, pos.z);

		bulletCollideRay->SetLength(ray->GetLength());
        bulletCollideRay->SetDirection(ray->GetDirection());

		auto* data = dGeomGetData(ray->GetGeomId());
        dGeomSetData(bulletCollideRay->GetGeomId(), data);

		ShellTraceLineCallback shellTraceLineCallback(bullet);
        dContact closestContact;
        if (ai::TraceLine(*bulletCollideRay, closestContact, 0, 0, 0, 0, &shellTraceLineCallback, 0, 0))
        {
            auto g1 = closestContact.geom.g1;
            auto IsEnabled = dGeomIsEnabled(closestContact.geom.g1);
            auto g2 = closestContact.geom.g2;
            auto wasEnabled1 = IsEnabled;
            auto wasEnabled2 = dGeomIsEnabled(closestContact.geom.g2);
            auto Body = dGeomGetBody(g1);
            auto v14 = dGeomGetBody(g1);
            int wasBodyEnabled1 = 0;
            int wasBodyEnabled2 = 0;
            if (Body)
                wasBodyEnabled1 = dBodyIsEnabled(Body);
            else
                wasBodyEnabled1 = 0;
            if (v14)
                wasBodyEnabled2 = dBodyIsEnabled(v14);
            else
                wasBodyEnabled2 = 0;
            if (Body)
                dBodyEnable(Body);
            if (v14)
                dBodyEnable(v14);
            dGeomEnable(g1);
            dGeomEnable(g2);
            ai::NearCallback(0, g1, g2);
            if (Body && !wasBodyEnabled1)
                dBodyDisable(Body);
            if (v14 && !wasBodyEnabled2)
                dBodyDisable(v14);
            if (!wasEnabled1)
                dGeomDisable(g1);
            if (!wasEnabled2)
                dGeomDisable(g2);
        }
	}

	bool DynamicScene::LoadSceneFromFile(char const* fileName, retruxx::vector<m3d::Class*> const& allowedClasses)
	{
        CStr err;
        if (ref_ptr xmlFile = m3d::ReadXmlFile(fileName, &err))
        {
            ref_ptr xmlNode = xmlFile->CreateNode();
            xmlFile->GetFirstChild(xmlNode, "DynamicScene");

            M3D_LOG_INFO("\t\t Scene loading begin");
            LoadSceneFromXml(xmlFile, xmlNode, allowedClasses);
            M3D_LOG_INFO("\t\t Scene loading end");
            return true;
        }
        M3D_LOG_ERR("Error: No DynamicScene file: " + CStr(fileName));
        return false;
	}

	int DynamicScene::ReadNewObjectFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode,
		retruxx::vector<m3d::Class*> const& allowedClasses)
	{
        CStr name = xmlNode->GetAttribute("Name");
        CStr prototypeName = xmlNode->GetAttribute("Prototype");

        auto prototypeId = thePrototypeManager->GetPrototypeId(prototypeName);
        if (prototypeId == -1)
        {
            M3D_LOG_ERR("Error: unknown prototype in DynamicScene.xml: '" + prototypeName + "'");
            return -1;
        }

        if (!allowedClasses.empty())
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int objId = -1;
        m3d::SafeIntAttrib(objId, xmlNode, "ObjectId");

        if (theObjects->GetObjIdByObjName(name) != -1)
        {
            M3D_LOG_ERR("Attempting to load object " + name + " of prototype " + prototypeName + ", but an object with this name already exists!");
        }

        auto entityForLoadId = theObjects->CreateEntityForLoad(prototypeId, name.c_str(), -1, objId);
        if (entityForLoadId == -1)
        {
            return -1;
        }

        auto* obj = theObjects->GetEntityByObjId(entityForLoadId);
        if (!obj)
        {
            M3D_LOG_ERR("Error: object with id " + CStr(entityForLoadId) + " was created but it is not in the ObjContainer");
            return -1;
        }

        obj->LoadFromXML(xmlFile, xmlNode);
        if (obj->GetClass() == RT_CLASS_LOCAL(Player))
        {
            if (ai::thePlayer)
                obj->Remove();
            else
                ai::thePlayer = (Player*)obj;
        }

        if (obj->IsKindOf(&ai::JointedObj::m_classJointedObj) && ai::theObjects->m_SaveType != ObjContainer::SAVE_FULL)
        {
            M3D_CRITICAL_ERROR("invalid object in DynamicScene : " + obj->GetDebugDescription());
        }

        return obj->GetId();
	}

	short DynamicScene::GetExplosionType(const CStr& shellTypeName)
	{
		// TODO: generated code
        // Check if shell type already exists
        for (size_t i = 0; i < m_shellTypesNames.size(); ++i)
        {
            if (m_shellTypesNames[i] == shellTypeName)
            {
                return static_cast<int>(i);
            }
        }

        // If not found, add new shell type
        m_shellTypesNames.push_back(shellTypeName);

        // Add new empty effects vector for this shell type
        std::vector<CStr> newEffects;
        size_t soilPropsCount = m_soilProps.size();
        newEffects.resize(soilPropsCount);
        m_shellsEffectsNames.push_back(newEffects);

        // Generate effect names for each soil type
        for (size_t i = 0; i < m_soilProps.size(); ++i)
        {
            const SoilProps& soilProp = m_soilProps[i];

            // Build effect name: "ET_PS_" + shellTypeName + soilSplashTypeName + "EXPLOSION"
            CStr effectName = "ET_PS_";
            effectName += shellTypeName;
            effectName += soilProp.m_splashTypeName;
            effectName += "EXPLOSION";

            // Store in the effects vector
            m_shellsEffectsNames.back()[i] = effectName;
        }

        // Generate water splash effect name
        CStr waterEffectName = "ET_PS_";
        waterEffectName += shellTypeName;
        waterEffectName += "WATERSPLASH";
        m_shellWaterEffectNames.push_back(waterEffectName);

        // Generate road explosion effect name
        CStr roadEffectName = "ET_PS_";
        roadEffectName += shellTypeName;
        roadEffectName += "ROADEXPLOSION";
        m_shellsRoadEffNames.push_back(roadEffectName);

        // Generate statics explosion effect name
        CStr staticsEffectName = "ET_PS_";
        staticsEffectName += shellTypeName;
        staticsEffectName += "STATICSEXPLOSION";
        m_shellsStaticsEffNames.push_back(staticsEffectName);

        // Generate vehicle explosion effect name
        CStr vehicleEffectName = "ET_PS_";
        vehicleEffectName += shellTypeName;
        vehicleEffectName += "VEHICLEEXPLOSION";
        m_shellsVehiclesEffNames.push_back(vehicleEffectName);

        // Create additional shell effect names
        CreateBoShellEffectNames();

        // Return the index of the newly added shell type
        return static_cast<int>(m_shellTypesNames.size() - 1);
	}

	bool DynamicScene::SaveSceneToFile(char const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Vehicle* DynamicScene::GetVehicleControlledByPlayer() const
	{
		if (thePlayer)
		{
			return thePlayer->GetVehicle();
		}
		return nullptr;
	}

	CStr const& DynamicScene::GetRoadEffectName(unsigned, bool) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CStr const& DynamicScene::GetShellRoadEffectName(unsigned short) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void DynamicScene::Clear()
	{
		if (contactGroup)
		{
			dJointGroupEmpty(contactGroup);
		}
	}

	m3d::Class* DynamicScene::GetClass() const
	{
		return Object::GetClass();
	}

	CStr const& DynamicScene::GetShellEffectName(unsigned short shellType, unsigned short soilType) const
	{
        if (shellType < m_shellsEffectsNames.size() && soilType < m_shellsEffectsNames[shellType].size())
        {
            return m_shellsEffectsNames[shellType][soilType];
        }
        return STANDARD_EXPLOSION;
	}

	void DynamicScene::UpdateSceneItems(float)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	DynamicScene::DynamicScene(DynamicScene const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TraceLineCallback::~TraceLineCallback() = default;

    DynamicScene::DynamicScene()
	{
		using namespace m3d;

		ColliderKrnl::Init();
		ColliderKrnl::RegisterCollider(0, 0, DefaultCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(GeomObjectWater),
			RT_CLASS_LOCAL(Object),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(DummyObject),
			RT_CLASS_LOCAL(Object),
			DefaultCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(DummyObject),
			RT_CLASS_LOCAL(VehiclePart),
			CollideDummyAndVehiclePart);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(ParticleSplinter),
			RT_CLASS_LOCAL(Object),
			CollideParticleSplinter);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(BreakableObject),
			CollideVehicleAndBreakableObject);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(VehiclePart),
			CollideVehiclePartAndVehiclePart);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(GeomObjectLandscape),
			CollideVehicleAndLandscape);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(GeomObjectStatics),
			CollideVehicleAndStatics);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(GeomObjectRoad),
			CollideVehicleAndRoad);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(GeomObjectWater),
			CollideVehicleAndWater);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(Town),
			CollideVehicleAndStatics);
		ColliderKrnl::RegisterCollider(RT_CLASS_LOCAL(Wheel), 0, CollideWheelDefault);
		ColliderKrnl::RegisterCollider(RT_CLASS_LOCAL(Wheel), RT_CLASS_LOCAL(Object), CollideWheelDefault);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Wheel),
			RT_CLASS_LOCAL(GeomObject),
			CollideWheelDefault);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Wheel),
			RT_CLASS_LOCAL(GeomObjectRoad),
			CollideWheelAndAsphalt);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Wheel),
			RT_CLASS_LOCAL(GeomObjectLandscape),
			CollideWheelAndLandscape);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Wheel),
			RT_CLASS_LOCAL(GeomObjectWater),
			CollideWheelAndWater);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Wheel),
			RT_CLASS_LOCAL(BreakableObject),
			CollideVehicleAndBreakableObject);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(PhysicObj),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(Wheel),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(PhysicBody),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(StaticAutoGun),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(VehiclePart),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(GeomObject),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(GeomObjectWater),
			CollideShellAndWater);
		ColliderKrnl::RegisterCollider(RT_CLASS_LOCAL(Shell), 0, (int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(GeomObjectRoad),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(BreakableObject),
			CollideVehicleAndBreakableObject);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(BreakableObject),
			RT_CLASS_LOCAL(GeomObject),
			CollideBreakableObjectAndGeomObject);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(BreakableObject),
			RT_CLASS_LOCAL(GeomObjectWater),
			CollidePOAndWater);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehicleSplinter),
			RT_CLASS_LOCAL(GeomObjectLandscape),
			CollideVehicleSplinterAndLandscape);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehicleSplinter),
			RT_CLASS_LOCAL(GeomObjectRoad),
			CollideVehicleSplinterAndLandscape);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehicleSplinter),
			RT_CLASS_LOCAL(ParticleSplinter),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(DummyObject),
			RT_CLASS_LOCAL(GeomObjectWater),
			CollidePOAndWater);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(ParticleSplinter),
			RT_CLASS_LOCAL(GeomObjectWater),
			CollideParticleSplinterAndWater);
		ColliderKrnl::RegisterCollider(RT_CLASS_LOCAL(BlastWave), RT_CLASS_LOCAL(Object), EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(BlastWave),
			RT_CLASS_LOCAL(PhysicObj),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))BlastWave::CollideBlastWaveAndPhysicObj);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(BlastWave),
			RT_CLASS_LOCAL(PhysicBody),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))BlastWave::CollideBlastWaveAndPhysicObj);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(PhysicUnit),
			RT_CLASS_LOCAL(VehiclePart),
			CollidePhysicUnitAndVehicle);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(PhysicUnit),
			RT_CLASS_LOCAL(Wheel),
			CollidePhysicUnitAndVehicle);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(PhysicUnit),
			RT_CLASS_LOCAL(Shell),
			CollidePhysicUnitAndShell);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(PhysicUnit),
			RT_CLASS_LOCAL(BlastWave),
			CollidePhysicUnitAndBlastWave);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(GeomObj),
			RT_CLASS_LOCAL(Object),
			CollideGeomObjAndLandscape);
		ColliderKrnl::RegisterCollider(RT_CLASS_LOCAL(GeomObj), RT_CLASS_LOCAL(Shell), CollideGeomObjAndShell);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(GeomObj),
			RT_CLASS_LOCAL(GeomObjectWater),
			CollidePOAndWater);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(BlastWave),
			RT_CLASS_LOCAL(GeomObj),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))BlastWave::CollideBlastWaveAndPhysicObj);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(GeomObj),
			CollideVehiclePartAndGeomObj);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(ParticleSplinter),
			RT_CLASS_LOCAL(GeomObj),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(BossMetalArmLoad),
			RT_CLASS_LOCAL(Object),
			BossMetalArmLoad::CollideBossMetalArmLoadWithObject);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(BossMetalArmLoad),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Boss03Part),
			RT_CLASS_LOCAL(VehiclePart),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))Boss03Part::CollideBoss03PartAndVehiclePart);
		ColliderKrnl::RegisterCollider(RT_CLASS_LOCAL(Boss03Part), RT_CLASS_LOCAL(Wheel), EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Boss04Part),
			RT_CLASS_LOCAL(VehiclePart),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(RT_CLASS_LOCAL(Boss04Part), RT_CLASS_LOCAL(Wheel), EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Boss04Part),
			RT_CLASS_LOCAL(GeomObjectLandscape),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Boss04StationPart),
			RT_CLASS_LOCAL(Boss04StationPart),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Boss04StationPart),
			RT_CLASS_LOCAL(GeomObjectLandscape),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Object),
			RT_CLASS_LOCAL(GeomObjectPassCell),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(GeomObjectPassCell),
			CollideVehicleAndPassCell);
	}

	void DynamicScene::_InitWheelTraces()
	{
		// TODO: generated code DynamicScene::_InitWheelTraces
        m3d::RoadManager& roadManager = m3d::pClient->GetWorld().GetRoadManager();
        m3d::WheelTraceMgr& wheelTraceMgr = pServer->GetWorld()->GetWheelTracesMgr();

        // Collect all unique wheel trace texture names from road sets
        std::set<CStr> roadWheelTraces;

        for (size_t i = 0; i < roadManager.m_roadSets.size(); ++i)
        {
            roadWheelTraces.insert(roadManager.m_roadSets[i]->m_wheeltraceTexName);
        }

        // Calculate total number of soil types (soil props + road wheel traces)
        size_t soilPropsCount = m_soilProps.size();
        size_t totalTypes = soilPropsCount + roadWheelTraces.size();

        // Initialize wheel trace manager with the total number of types
        wheelTraceMgr.Init(totalTypes);

        // Add soil properties textures to wheel trace manager
        for (size_t i = 0; i < soilPropsCount; ++i)
        {
            wheelTraceMgr.AddTextureBySoilType(m_soilProps[i].m_idx, m_soilProps[i].m_wheelTraceTextureName);
        }

        // Add road wheel trace textures and update road set soil types
        int roadTypeIndex = 0;
        for (auto it = roadWheelTraces.begin(); it != roadWheelTraces.end(); ++it, ++roadTypeIndex)
        {
            CStr const& wheelTraceName = *it;

            // Add to wheel trace manager
            wheelTraceMgr.AddTextureBySoilType(static_cast<int>(soilPropsCount + roadTypeIndex), wheelTraceName);

            // Update road sets that use this wheel trace texture
            for (size_t j = 0; j < roadManager.m_roadSets.size(); ++j)
            {
                m3d::RoadSet* roadSet = roadManager.m_roadSets[j];

                if (roadSet->m_wheeltraceTexName == wheelTraceName)
                {
                    roadSet->m_soilType = static_cast<int>(soilPropsCount + roadTypeIndex);
                }
            }
        }
	}

	void DynamicScene::_RecalcWheelEffectNames()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void DynamicScene::_AddSoilEffectNameForWheelTypeName(CStr const&)
    {
        // TODO: implement DynamicScene::_AddSoilEffectNameForWheelTypeName
        //RETRUXX_NOT_IMPLEMENTED;
    }

    ObjIdExceptionalTraceLineCallback::ObjIdExceptionalTraceLineCallback(const ai::ObjIdExceptionalTraceLineCallback&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ObjIdExceptionalTraceLineCallback::ObjIdExceptionalTraceLineCallback(const std::vector<int, std::allocator<int>>& Exceptions) : m_Exceptions(Exceptions)
    {
    }

    bool ObjIdExceptionalTraceLineCallback::CollideId(int objId) const
    {
		// TODO: check this
        for (auto const& exception : m_Exceptions)
        {
            if (exception == objId)
            {
                return false;
            }
        }
        return true;
    }

    bool ObjIdExceptionalTraceLineCallback::CollidePhysicObj(const ai::PhysicObj* physicObj) const
    {
        return true;
    }
}  // namespace ai
