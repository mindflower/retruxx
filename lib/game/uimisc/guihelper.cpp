#include "guihelper.h"
#include <client.h>
#include <level.h>
#include <world.h>

#include "core/kernel.h"
#include "core/log.h"
#include "file/fileenum.h"
#include "file/fileserver.h"
#include "server/objects/bar.h"
#include "server/objects/basket.h"
#include "server/objects/building.h"
#include "server/objects/cabin.h"
#include "server/objects/chassis.h"
#include "server/objects/dynamicquest.h"
#include "server/objects/dynamicquestpeace.h"
#include "game/uimisc/objectcollection.h"
#include "server/queststate.h"
#include "game/uimisc/questinfo.h"
#include "server/objects/gadget.h"
#include "server/objects/player.h"
#include "server/objects/town.h"
#include "server/objects/vehicle.h"
#include "server/objects/workshop.h"
#include "server/geomrepository.h"
#include "server/geomrepositoryitem.h"
#include "server/objects/base/complexphysicobj.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/guns/compoundgun.h"
#include "server/objects/guns/gun.h"
#include "server/izvratrepository.h"
#include "server/objects/guns/bulletlauncher.h"
#include "server/objects/ware.h"
#include "game/uimanager/uidefs.h"
#include "game/uiwindows/charwindows/znayukakprodatwnd.h"
#include "server/objects/physicbodies/vehiclepart.h"
#include "server/server.h"
#include "ui/ui.h"
#include "ui/button.h"
#include "ui/font.h"
#include "ui/image.h"
#include "ui/ui_srv.h"
#include <m3dapp.h>
#include "game/m3dgame.h"  // CMiracle3d - M3D_APP->m_pInterfaceManager (help::ftoa)

#include <sstream>
#include <math/vector.h>
#include <renderer/i_renderer.h>
#include <server/resourcemanager.h>

namespace m3d
{
    extern CClient* pClient;
}

namespace help
{
    void DeleteAllFilesInDirectory(char const* dir)
    {
        auto fileMask = dir + CStr("\\*.*");
        WIN32_FIND_DATAA data;
        auto file = FindFirstFileA(fileMask.c_str(), &data);
        if (file = INVALID_HANDLE_VALUE)
        {
            FindClose(INVALID_HANDLE_VALUE);
            return;
        }
        CStr strDir(dir);
        assert(strDir.length() > 0);
        strDir += "\\";
        do
        {
            auto fileName = data.cFileName;
            if (fileName != "." && fileName != "..")
            {
                auto fullName = strDir + fileName;
                auto attr = GetFileAttributesA(fullName.c_str());
                SetFileAttributesA(fullName.c_str(), attr & 0xFA);
                DeleteFileA(fullName.c_str());
                m3d::g_Kernel->GetFileServer().RemoveFile(fullName.c_str());
            }
        } while (FindNextFileA(file, &data));
        FindClose(file);
    }

    CStr GetCurrentLevelName()
    {
        if (m3d::pClient != nullptr)
        {
            if (auto* level = m3d::pClient->GetWorld().m_level; level != nullptr)
            {
                return level->GetLevelName();
            }
        }
        return {};
    }

    namespace
    {
        // RVA 0x555520 - a dynamic quest reports a richer set of states than the
        // journal cares about; only "in progress", "done" and "failed" map across.
        UnifyQuestStatus DynamicQuestStatus2UnifyQuestStatus(ai::DynamicQuest::QuestStatus status)
        {
            switch (status)
            {
            case ai::DynamicQuest::STATUS_PROCESSING:
                return QUESTSTATUS_NONCOMPLETE;
            case ai::DynamicQuest::STATUS_COMPLETE:
                return QUESTSTATUS_COMPLETE;
            case ai::DynamicQuest::STATUS_FAILED:
                return QUESTSTATUS_FAILED;
            default:
                return QUESTSTATUS_INVALID;
            }
        }
    }  // namespace

    UnifyQuestStatus GetQuestUnifyStatusByQuestId(help::QuestType questType, int questId)
    {
        // RVA 0x5555B0 - static and dynamic quests keep their progress in two
        // completely different places, so both are folded onto one scale here.
        if (questType == QUESTTYPE_STATIC)
        {
            ai::QuestState const* qs = ai::theQuestStateManager->GetQuestStateById(questId);
            if (qs)
            {
                switch (qs->GetCompleteStatus())
                {
                case ai::QuestState::NOT_COMPLETE:
                    return QUESTSTATUS_NONCOMPLETE;
                case ai::QuestState::COMPLETE:
                    return QUESTSTATUS_COMPLETE;
                case ai::QuestState::FAILED:
                    return QUESTSTATUS_FAILED;
                default:
                    break;
                }
            }
        }
        else if (questType == QUESTTYPE_DYNAMIC)
        {
            ai::Obj* obj = ai::theObjects->GetEntityByObjId(questId);
            if (obj && obj->IsKindOf(&ai::DynamicQuest::m_classDynamicQuest))
            {
                return DynamicQuestStatus2UnifyQuestStatus(static_cast<ai::DynamicQuest*>(obj)->GetQuestStatus());
            }
        }
        return QUESTSTATUS_INVALID;
    }

    bool CanNavPointBeAddedOnQuest(help::QuestType questType, int questId)
    {
        // RVA 0x555650
        if (questType == QUESTTYPE_NUM_QUEST_TYPES || questId == -1 ||
            GetQuestUnifyStatusByQuestId(questType, questId) != QUESTSTATUS_NONCOMPLETE)
        {
            return false;
        }

        QuestInfoManager* qim = M3D_APP->m_pInterfaceManager->GetQuestInfoManager();
        QuestInfo const* qi = (questType == QUESTTYPE_STATIC) ? qim->GetQuestInfoForStaticQuest(questId)
                                                              : qim->GetQuestInfoForDynamicQuest(questId);
        // Without a coordinate on this level there is nowhere for the point to go.
        return qi && qi->GetCoordinateForMap(GetCurrentLevelName()) != nullptr;
    }

    bool IsPeaceWithEnemyAvailable(int enemyBelong)
    {
        // RVA 0x555320
        if (!ai::thePlayer || ai::pServer->CheckTolerance(ai::thePlayer->GetBelong(), enemyBelong) != ai::RS_ENEMY)
        {
            return false;
        }

        std::set<int> const* peaceQuests =
            M3D_APP->m_pInterfaceManager->GetObjectCollection().GetObjectsByClass(
                &ai::DynamicQuestPeace::m_classDynamicQuestPeace);
        if (!peaceQuests)
        {
            return false;
        }

        for (auto it = peaceQuests->begin(); it != peaceQuests->end(); ++it)
        {
            ai::Obj* obj = ai::theObjects->GetEntityByObjId(*it);
            if (!obj || !obj->IsKindOf(&ai::DynamicQuestPeace::m_classDynamicQuestPeace))
            {
                continue;
            }
            // NOTE: a peace quest stores the clan it settles with in its target
            // *object* id field, so this compares an objId slot against a belong.
            auto* quest = static_cast<ai::DynamicQuest*>(obj);
            if (quest->GetQuestStatus() <= ai::DynamicQuest::STATUS_PROCESSING &&
                quest->GetTargetObjId() == enemyBelong)
            {
                return true;
            }
        }
        return false;
    }

    int CloneWndWithChildren(m3d::ui::Wnd const* srcWnd, m3d::ui::Wnd* dstWnd)
    {
        using namespace m3d::ui;
        if (srcWnd && dstWnd && !dstWnd->Valid())
        {
            if (dstWnd->Create(srcWnd->GetText(), srcWnd->GetStyle(), srcWnd->GetBounds(), srcWnd->GetId()) != 0)
            {
                dstWnd->SetStyle(srcWnd->GetStyle());
                dstWnd->SetText(srcWnd->GetText());
                dstWnd->SetId(srcWnd->GetId());
                dstWnd->SetName(srcWnd->GetName());
                dstWnd->SetBounds(srcWnd->GetBounds(), true);
                dstWnd->SetDefaultFont(srcWnd->GetDefaultFont());
                dstWnd->SetWrapMode(srcWnd->GetWrapMode());
                dstWnd->SetFormatMode(srcWnd->GetFormatMode());
                dstWnd->SetColor(srcWnd->GetColor());
                dstWnd->SetTextColor(srcWnd->GetTextColor());
                dstWnd->SetTextColorDisabled(srcWnd->GetTextColorDisabled());
                dstWnd->SetClientEdges(srcWnd->GetClientEdges());
                dstWnd->SetPane(srcWnd->GetPaneName());
                dstWnd->SetPaneFlags(srcWnd->GetPaneFlags());
                dstWnd->SetScrollPane(srcWnd->GetScrollPaneName());
                dstWnd->SetBackground(srcWnd->GetBackground());

                CStr tooltip;
                srcWnd->GetProperty(0x4000, &tooltip);
                dstWnd->SetProperty(0x4000, &tooltip);

                dstWnd->SetOnShowAnimation(srcWnd->GetOnShowAnimation());
                dstWnd->SetOnHideAnimation(srcWnd->GetOnHideAnimation());

                auto child = srcWnd->GetFirstChild();
                for (child = srcWnd->GetFirstChild(); child && child->IsKindOf(RT_CLASS_LOCAL(Wnd));
                     child = child->GetNextSibling())
                {
                    auto childWnd = dynamic_cast<Wnd*>(child);
                    auto newObjClass = child->GetClass();
                    auto newObj = m3d::g_Kernel->New(newObjClass);
                    auto newWnd = dynamic_cast<Wnd*>(newObj);
                    if (!newObj ||
                        newWnd->Create(
                            childWnd->GetText(), childWnd->GetStyle(), childWnd->GetBounds(), childWnd->GetId()) == 0)
                    {
                        break;
                    }
                    newWnd->SetStyle(childWnd->GetStyle());
                    newWnd->SetText(childWnd->GetText());
                    newWnd->SetId(childWnd->GetId());
                    newWnd->SetName(childWnd->GetName());
                    newWnd->SetBounds(childWnd->GetBounds(), true);
                    newWnd->SetDefaultFont(childWnd->GetDefaultFont());
                    newWnd->SetWrapMode(childWnd->GetWrapMode());
                    newWnd->SetFormatMode(childWnd->GetFormatMode());
                    newWnd->SetColor(childWnd->GetColor());
                    newWnd->SetTextColor(childWnd->GetTextColor());
                    newWnd->SetTextColorDisabled(childWnd->GetTextColorDisabled());
                    newWnd->SetClientEdges(childWnd->GetClientEdges());
                    newWnd->SetPane(childWnd->GetPaneName());
                    newWnd->SetPaneFlags(childWnd->GetPaneFlags());
                    newWnd->SetScrollPane(childWnd->GetScrollPaneName());
                    newWnd->SetBackground(childWnd->GetBackground());

                    childWnd->GetProperty(0x4000, &tooltip);
                    newWnd->SetProperty(0x4000, &tooltip);

                    newWnd->SetOnShowAnimation(childWnd->GetOnShowAnimation());
                    newWnd->SetOnHideAnimation(childWnd->GetOnHideAnimation());
                    if (newObjClass == RT_CLASS_LOCAL(ButtonWnd))
                    {
                        auto childButton = dynamic_cast<ButtonWnd*>(child);
                        auto newObjButton = dynamic_cast<ButtonWnd*>(newObj);
                        if (childButton->IsImaged())
                        {
                            newObjButton->SetImaged(
                                childButton->GetImageRegular(),
                                childButton->GetImageDown(),
                                childButton->GetImageIn(),
                                childButton->GetImageDisabled());
                        }
                        else
                        {
                            childButton->SetRegular();
                        }
                    }
                    else if (newObjClass == RT_CLASS_LOCAL(ImageWnd))
                    {
                        auto childImage = dynamic_cast<ImageWnd*>(child);
                        auto newObjImage = dynamic_cast<ImageWnd*>(newObj);
                        newObjImage->SetImage(childImage->GetImage());
                    }
                    dstWnd->AddChild(newObj);
                }
                return 1;
            }
        }
        return 0;
    }

    int GetWindowsSubDirs(CStr const& parentDirPath, retruxx::vector<CStr>& subDirs, CStr const& pattern)
    {
        subDirs.clear();
        auto attr = GetFileAttributesA(parentDirPath.c_str());
        if (attr != -1 && (attr & 0x10) != 0)
        {
            _finddata_t data;
            m3d::fs::CFileEnum fileEnum;
            if (!fileEnum.StartEnumeration(parentDirPath.c_str(), pattern.c_str(), &data))
            {
                return 1;
            }

            do
            {
                if ((data.attrib & 0x10) != 0)
                {
                    CStr dirName = data.name;
                    if (!dirName.empty() && dirName == "." || dirName == "..")
                    {
                        continue;
                    }
                    subDirs.push_back(dirName);
                }
            } while (fileEnum.GetNextFile(&data));
            return 1;
        }
        M3D_LOG_INFO("GetWindowsSubDirs error - dir \"" + parentDirPath + "\" does not exist");
        return 0;
    }

    int CreateWindowsDir(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr GetMapNameFromFileName(CStr const& fileName)
    {
        auto name = NameFromFileName(fileName);
        auto pos = name.find('.');
        auto res = name.substr(0, pos);
        return res;
    }

    CStr Color2Str(unsigned colorId)
    {
        auto const clr = m3d::ui::Wnd::GetGfxServer()->GetColor(colorId);
        std::ostringstream ss;
        ss << std::hex << clr;
        return CStr("@") + ss.str().c_str();
    }

    CStr ftoa(float fVal, int precision)
    {
        if (precision == -1)
        {
            precision = M3D_APP->m_pInterfaceManager->GetDefaultFloatPrecision();
        }
        if (precision < 0)
        {
            precision = 0;
        }
        else if (precision > 10)
        {
            precision = 10;
        }
        CStr str;
        str.format("%0.*f", precision, fVal);
        return str;
    }

    int GetSellPriceByObjId(int objId, int townId)
    {
        if (objId == -1 || townId == -1)
        {
            return -1;
        }
        auto* townObj = ai::theObjects->GetEntityByObjId(townId);
        auto* town = (townObj && townObj->IsKindOf(&ai::Town::m_classTown)) ? static_cast<ai::Town*>(townObj)
                                                                           : nullptr;
        auto* obj = ai::theObjects->GetEntityByObjId(objId);
        if (obj && !obj->IsKindOf(&ai::Obj::m_classObj))
        {
            obj = nullptr;
        }
        if (!town || !obj)
        {
            return -1;
        }
        ai::Workshop* workshop = town->GetWorkshopByObject(obj);
        return workshop ? static_cast<int>(workshop->GetObjectBuyPrice(obj)) : -2;
    }

    int GetBuyPriceByObjId(int objId, int townId)
    {
        if (objId == -1 || townId == -1)
        {
            return -1;
        }
        auto* townObj = ai::theObjects->GetEntityByObjId(townId);
        auto* town = (townObj && townObj->IsKindOf(&ai::Town::m_classTown)) ? static_cast<ai::Town*>(townObj)
                                                                           : nullptr;
        auto* obj = ai::theObjects->GetEntityByObjId(objId);
        if (obj && !obj->IsKindOf(&ai::Obj::m_classObj))
        {
            obj = nullptr;
        }
        if (!town || !obj)
        {
            return -1;
        }
        ai::Workshop* workshop = town->GetWorkshopByObject(obj);
        return workshop ? static_cast<int>(workshop->GetObjectSellPrice(obj)) : -3;
    }

    void RepairObj(ai::Obj* o)
    {
        // TODO(RVA 0x556AD0): the shipped helper dispatches to help::RepairVehicle /
        // help::RepairVehiclePart by kind; neither leaf helper is ported yet, so this
        // is a no-op for now (objects added back to a shop simply aren't auto-repaired).
        (void)o;
    }

    bool IsChildObjCompatibleWithVehicle(int objId, int vehicleId)
    {
        // TODO(RVA 0x5544C0): dispatches to help::IsVehiclePartCompatibleWithVehicle /
        // help::IsGadgetCompatibleWithVehicle by kind - the gadget leaf is not ported
        // yet. Reported as "not compatible" so shop slots simply don't get the
        // compatible-with-vehicle highlight until that is implemented.
        (void)objId;
        (void)vehicleId;
        return false;
    }

    void GetObjetsInRepositoryByResourceType(
        ai::GeomRepository const* repository,
        int resourceId,
        std::vector<int, std::allocator<int>>& objIds)
    {
        // RVA 0x5530B0
        objIds.clear();
        if (!repository || resourceId == -1)
        {
            return;
        }

        int const numItems = static_cast<int>(repository->GetNumItems());
        for (int i = 0; i < numItems; ++i)
        {
            ai::GeomRepositoryItem const item = repository->GetItem(i);
            ai::Obj* obj = ai::theObjects->GetEntityByObjId(item.GetObjId());
            if (!obj || !obj->IsKindOf(&ai::Obj::m_classObj))
            {
                continue;
            }

            ai::PrototypeInfo const* prototypeInfo = obj->GetPrototypeInfo();
            if (prototypeInfo && ai::theResourceManager->bResourceIsKindOf(prototypeInfo->m_resourceId, resourceId))
            {
                objIds.push_back(obj->GetId());
            }
        }
    }

    bool IsVehiclePartCompatibleWithVehicle(int vpId, int vehicleId)
    {
        // RVA 0x5531B0 - walks the vehicle prototype's part slots and reports
        // whether any still-attachable one accepts the part's resource.
        if (vpId == -1 || vehicleId == -1)
        {
            return false;
        }

        ai::Obj* vehicleObj = ai::theObjects->GetEntityByObjId(vehicleId);
        ai::Vehicle* vehicle = (vehicleObj && vehicleObj->IsKindOf(&ai::Vehicle::m_classVehicle))
                                   ? static_cast<ai::Vehicle*>(vehicleObj)
                                   : nullptr;

        ai::Obj* vpObj = ai::theObjects->GetEntityByObjId(vpId);
        ai::VehiclePart* vp = (vpObj && vpObj->IsKindOf(&ai::VehiclePart::m_classVehiclePart))
                                  ? static_cast<ai::VehiclePart*>(vpObj)
                                  : nullptr;

        if (!vehicle || !vp)
        {
            return false;
        }

        ai::PrototypeInfo const* vpPrototypeInfo = ai::thePrototypeManager->GetPrototypeInfo(vp->GetPrototypeId());
        if (!vpPrototypeInfo)
        {
            return false;
        }
        int const vpResourceId = vpPrototypeInfo->m_resourceId;

        auto const* vehiclePrototypeInfo =
            static_cast<ai::ComplexPhysicObjPrototypeInfo const*>(vehicle->GetPrototypeInfo());
        if (!vehiclePrototypeInfo)
        {
            return false;
        }

        auto const& allPartNames = vehiclePrototypeInfo->GetAllPartNames();
        for (int i = 0; i < static_cast<int>(allPartNames.size()); ++i)
        {
            CStr const& partName = allPartNames[i];
            if (!vehicle->CanPartBeAttached(partName))
            {
                continue;
            }
            auto const* partDescription = vehiclePrototypeInfo->GetPartDescriptionByName(partName);
            if (partDescription &&
                ai::theResourceManager->bResourceIsKindOf(vpResourceId, partDescription->GetPartResourceId()))
            {
                return true;
            }
        }
        return false;
    }

    void GetCompatibleVehiclePartsFromWorkshop(
        int workshopId,
        int vpResourceId,
        int vehicleId,
        std::vector<int, std::allocator<int>>& ids)
    {
        // RVA 0x553D70
        ids.clear();
        if (workshopId == -1 || vpResourceId == -1 || vehicleId == -1)
        {
            return;
        }

        ai::Obj* workshopObj = ai::theObjects->GetEntityByObjId(workshopId);
        if (!workshopObj || !workshopObj->IsKindOf(&ai::Workshop::m_classWorkshop))
        {
            return;
        }
        auto* workshop = static_cast<ai::Workshop*>(workshopObj);

        ai::Obj* vehicleObj = ai::theObjects->GetEntityByObjId(vehicleId);
        if (!vehicleObj || !vehicleObj->IsKindOf(&ai::Vehicle::m_classVehicle))
        {
            return;
        }

        if (!ai::theResourceManager->bResourceIsKindOf(
                vpResourceId, ai::theResourceManager->GetResourceId(CStr("VEHICLE_PART"))))
        {
            return;
        }

        ai::GeomRepository const* repository =
            workshop->GetRepositoryByType(ai::Workshop::GetRepositoryTypeByResourceId(vpResourceId));
        if (!repository)
        {
            return;
        }

        std::vector<int> allIds;
        GetObjetsInRepositoryByResourceType(repository, vpResourceId, allIds);
        for (int i = 0; i < static_cast<int>(allIds.size()); ++i)
        {
            if (IsVehiclePartCompatibleWithVehicle(allIds[i], vehicleId))
            {
                ids.push_back(allIds[i]);
            }
        }
    }

    void GetGunsForVehicle(int vehicleId, retruxx::vector<ai::Obj*>& guns)
    {
        using namespace ai;

        guns.clear();
        auto* obj = ai::theObjects->GetEntityByObjId(vehicleId);
        if (obj && IS_KIND_OF(obj, Vehicle))
        {
            auto* veh = RT_DYNCAST(obj, Vehicle);
            for (auto it = veh->begin(); it != veh->end(); ++it)
            {
                auto const& [name, part] = *it;
                if (part)
                {
                    auto const* protoInfo = part->GetPrototypeInfo();
                    if (protoInfo)
                    {
                        if (ai::theResourceManager->bResourceIsKindOf(
                                protoInfo->m_resourceId, theResourceManager->GetResourceId("GUN")))
                        {
                            guns.push_back(part);
                        }
                    }
                }
            }
        }
    }
    void GetAllMoveAnimations(retruxx::vector<ActionType>& moveAnimations)
    {
        moveAnimations.clear();
        moveAnimations.push_back(AT_BLOCK1);
        moveAnimations.push_back(AT_BLOCK2);
        moveAnimations.push_back(AT_DEATH1);
        moveAnimations.push_back(AT_DEATH2);
    }

    void GetAllStandAnimations(retruxx::vector<ActionType>& standAnimations)
    {
        standAnimations.clear();
        standAnimations.push_back(AT_STAND1);
        standAnimations.push_back(AT_STAND2);
        standAnimations.push_back(AT_MOVE1);
    }

    ActionType GetRandomMoveAnimation(m3d::AnimatedModel* model)
    {
        if (!model)
        {
            return AT_NUMTYPES;
        }
        retruxx::vector<ActionType> moveAnimations;
        GetAllMoveAnimations(moveAnimations);
        return moveAnimations[rand() % moveAnimations.size()];
    }

    ActionType GetRandomStandAnimation(m3d::AnimatedModel* model)
    {
        if (!model)
        {
            return AT_NUMTYPES;
        }
        retruxx::vector<ActionType> standAnimations;
        GetAllStandAnimations(standAnimations);
        return standAnimations[rand() % standAnimations.size()];
    }

    int GetScaledFontId(int patternFontId, float wantedFontSz)
    {
        auto* gfx = m3d::ui::Wnd::GetGfxServer();
        const m3d::ui::Font* patternFont = gfx->GetFontById(patternFontId);
        if (!patternFont)
        {
            return -1;
        }

        const CStr fontName = patternFont->m_nameShort;
        const m3d::ui::FontType type = patternFont->m_type;

        m3d::ui::FontParams params;
        if (type == m3d::ui::FONT_TYPE_SELFMAKING)
        {
            params.ttfParams.style = 1;
        }
        else
        {
            params.ttfParams.style = patternFont->m_style;
        }
        params.ttfParams.codePage = M3D_APP->m_codePage.CodePage;

        return gfx->GetFontId(fontName, wantedFontSz, type, params);
    }

    bool IsBoss(ai::Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    CStr GetClanAbbreviationByName(CStr const& clanName)
    {
        return clanName + "_abb";
    }
    CStr GetClanFullNameByName(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    CStr GetClanNameByBelong(int clanBelong)
    {
        return "Belong_" + CStr(clanBelong);
    }
    CStr GetKeysForImpulse(int impulseId)
    {
        // RVA 0x555980 - renders every key combination bound to `impulseId` as
        // ["Key1" + "Key2"], with alternative combinations separated by ", ".
        CStr strKeys;
        std::vector<std::vector<int>> const keySets = M3D_APP->m_pImpulses->GetKeysForImpulse(impulseId, 0);

        for (int i = 0; i < static_cast<int>(keySets.size()); ++i)
        {
            std::vector<int> const& keySet = keySets[i];
            for (int j = 0; j < static_cast<int>(keySet.size()); ++j)
            {
                CStr const keyName = M3D_APP->m_pImpulses->GetKeyNameById(keySet[j]);
                CStr const keyFullName = M3D_APP->GetStringByStringId0(keyName);
                strKeys += CStr("\"") + keyFullName + "\"";
                if (j < static_cast<int>(keySet.size()) - 1)
                {
                    strKeys += " + ";
                }
            }
            if (i < static_cast<int>(keySets.size()) - 1)
            {
                strKeys += ", ";
            }
        }
        return strKeys;
    }
    CStr CreateTooltipForImpulse(int impulseId)
    {
        // RVA 0x555D50
        return M3D_APP->GetStringByStringId0("KeySet") + ": " + GetKeysForImpulse(impulseId);
    }
    ai::Vehicle* GetPlayerVehicle()
    {
        // RVA 0x5513B0
        return ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr;
    }
    int GetPlayerVehicleId()
    {
        // RVA 0x5513D0
        ai::Vehicle const* vehicle = GetPlayerVehicle();
        return vehicle ? vehicle->GetId() : -1;
    }
    CStr GetServiceSymbols()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    CStr GetServiceSymbolsForVisualisation()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    CStr GetVehiclePartNameByResourceId(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    int RoundHealth(float val)
    {
        if (val <= 0.0)
            return 0;
        if (val >= 1.0)
            return (int)val;
        return 1;
    }

    void SetWndTextAlpha(m3d::ui::Wnd*, unsigned char)
    {
        // TODO: implement SetWndTextAlpha
        // RETRUXX_NOT_IMPLEMENTED;
    }

    bool WindowsDirExists(CStr const& dirPath)
    {
        DWORD const fileAttributes = GetFileAttributesA(dirPath.c_str());
        return fileAttributes != -1 && (fileAttributes & 0x10) != 0;
    }

    bool WindowsFileExists(CStr const& filePath)
    {
        DWORD const fileAttributes = GetFileAttributesA(filePath.c_str());
        return fileAttributes != -1 && (fileAttributes & 0x10) == 0;
    }

    bool IsWndValid(m3d::ui::Wnd const* w)
    {
        return w && M3D_APP->IsWndAlive(w, -1) && w->Valid();
    }

    int DestroyVehicle(int vehicleId)
    {
        // RVA 0x553F40. An unset id counts as success - there was nothing to destroy.
        if (vehicleId == -1)
        {
            return 1;
        }
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(vehicleId);
        if (!obj || !obj->IsKindOf(&ai::Vehicle::m_classVehicle))
        {
            return 0;
        }
        obj->Remove();
        return 1;
    }

    NavPoint::ObjectType GetNpObjectTypeByQuestType(help::QuestType questType)
    {
        // RVA 0x5557B0
        if (questType == QUESTTYPE_STATIC)
        {
            return NavPoint::OBJECT_TYPE_STATIC_QUEST;
        }
        if (questType == QUESTTYPE_DYNAMIC)
        {
            return NavPoint::OBJECT_TYPE_DYNAMIC_QUEST;
        }
        return NavPoint::OBJECT_TYPE_INVALID;
    }

    ai::Vehicle* CreateVehicleFromPrototype(int prototypeId)
    {
        // RVA 0x553ED0
        if (prototypeId == -1)
        {
            return nullptr;
        }
        int const objId = ai::theObjects->CreateNewObject(prototypeId, "VehicleToSell", -1, -1);
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(objId);
        return (obj && obj->IsKindOf(&ai::Vehicle::m_classVehicle)) ? static_cast<ai::Vehicle*>(obj) : nullptr;
    }

    void RemoveAllPartsFromVehicle(ai::Vehicle* vehicle)
    {
        // RVA 0x553FA0 - strips every part except the chassis, which the vehicle
        // cannot exist without.
        for (auto it = vehicle->begin(); it != vehicle->end();)
        {
            // Advance before touching the part: both calls below can erase the
            // entry we are standing on.
            auto const cur = it++;
            ai::VehiclePart* part = cur->second;
            if (!part || part->GetPartName() == CStr("CHASSIS"))
            {
                continue;
            }
            CStr const partName = cur->first;
            vehicle->SetPartByName(partName, nullptr, true);
            part->Remove();
        }
    }

    ai::Bar* GetBarWithBarmanForTown(ai::Town const* town)
    {
        if (!town)
        {
            return nullptr;
        }
        for (auto* building : town->GetBuildingByType(ai::BAR))
        {
            if (building && building->IsKindOf(&ai::Bar::m_classBar))
            {
                auto* bar = static_cast<ai::Bar*>(building);
                if (bar->bWithBarman())
                {
                    return bar;
                }
            }
        }
        return nullptr;
    }

    ai::Bar* GetBarWithoutBarmanForTown(ai::Town const* town)
    {
        if (!town)
        {
            return nullptr;
        }
        for (auto* building : town->GetBuildingByType(ai::BAR))
        {
            if (building && building->IsKindOf(&ai::Bar::m_classBar))
            {
                auto* bar = static_cast<ai::Bar*>(building);
                if (!bar->bWithBarman())
                {
                    return bar;
                }
            }
        }
        return nullptr;
    }

    ai::Building* GetShopForTown(ai::Town const* town)
    {
        if (!town)
        {
            return nullptr;
        }
        // Matches the shipped game (RVA 0x554C20): it fetches the SHOP buildings
        // but type-checks the first one against ai::Workshop.
        auto const buildings = town->GetBuildingByType(ai::SHOP);
        if (!buildings.empty() && buildings.front() && buildings.front()->IsKindOf(&ai::Workshop::m_classWorkshop))
        {
            return buildings.front();
        }
        return nullptr;
    }

    ai::Building* GetWorkshopForTown(ai::Town const* town)
    {
        if (!town)
        {
            return nullptr;
        }
        auto const buildings = town->GetBuildingByType(ai::WORKSHOP);
        if (!buildings.empty() && buildings.front() && buildings.front()->IsKindOf(&ai::Workshop::m_classWorkshop))
        {
            return buildings.front();
        }
        return nullptr;
    }

    PointBase<float> GetRelScreenPtByWorldPos(CVector const& worldPos)
    {
        // RVA 0x154350
        CVector const orgInv = M3D_RENDERER->MatGetOrgInv();
        CVector const relToCam{worldPos.x - orgInv.x, worldPos.y - orgInv.y, worldPos.z - orgInv.z};
        CVector const screenPt = M3D_RENDERER->Project(relToCam);

        PointBase<float> result{screenPt.x, screenPt.y};
        M3D_RENDERER->AbsToRel(result.x, result.y);
        return result;
    }

    ai::eTolerance GetObjTolerance(int objId)
    {
        if (!ai::thePlayer)
        {
            return ai::RS_MAX;
        }
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(objId);
        if (!obj)
        {
            return ai::RS_MAX;
        }
        return ai::pServer->CheckTolerance(ai::thePlayer->GetBelong(), obj->GetBelong());
    }

    bool CanGunFire(ai::Obj const* gun)
    {
        // NOTE: this helper's own implementation is not among the functions
        // decompiled for this pass (only its call sites are, which show it
        // takes the gun object and returns whether it can currently fire).
        // A hard RETRUXX_NOT_IMPLEMENTED stub is avoided here because this
        // runs every frame from WeaponInfoWnd::UpdateOnNewFrame while any
        // weapon-info UI is visible; as a safe placeholder this reports
        // "can fire" whenever a gun object exists.
        return gun != nullptr;
    }

    bool CanGunShotToSeenObj(ai::Obj const* gun)
    {
        // NOTE: same situation as CanGunFire above - this helper's own
        // implementation is not among the functions decompiled for this
        // pass, only call sites that show it takes the gun object and
        // returns whether it can currently hit the player's seen/locked
        // target. A safe placeholder ("can shoot" whenever a gun exists) is
        // used instead of a hard RETRUXX_NOT_IMPLEMENTED stub because this
        // is called every frame while weapon-group UI is visible.
        return gun != nullptr;
    }

    bool IsGunWithCharging(ai::Obj const* gun)
    {
        if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
        {
            return g->IsWithCharging();
        }
        if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
        {
            return cg->IsWithCharging();
        }
        return false;
    }

    unsigned int GetGunChargeSize(ai::Obj const* gun)
    {
        if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
        {
            return g->GetChargeSize();
        }
        if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
        {
            return cg->GetChargeSize();
        }
        return 0;
    }

    unsigned int GetGunShellsInCurrentCharge(ai::Obj const* gun)
    {
        if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
        {
            return g->GetShellsInCurrentCharge();
        }
        if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
        {
            return cg->GetShellsInCurrentCharge();
        }
        return 0;
    }

    unsigned int GetGunShellsInPool(ai::Obj const* gun)
    {
        if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
        {
            return g->GetShellsInPool();
        }
        if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
        {
            return cg->GetShellsInPool();
        }
        return 0;
    }

    float GetGunRechargingTime(ai::Obj const* gun)
    {
        if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
        {
            return g->GetRechargingTime();
        }
        if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
        {
            return cg->GetRechargingTime();
        }
        return 0.0f;
    }

    float GetGunCurrentRechargingTime(ai::Obj const* gun)
    {
        if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
        {
            return g->GetCurrentRechargingTime();
        }
        if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
        {
            return cg->GetCurrentRechargingTime();
        }
        return 0.0f;
    }

    ai::FiringTypes GetGunFiringType(ai::Obj const* gun)
    {
        if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
        {
            if (auto const* pi = g->GetPrototypeInfo())
            {
                return pi->m_firingType;
            }
        }
        else if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
        {
            if (auto const* pi = cg->GetPrototypeInfo())
            {
                return pi->GetFiringType();
            }
        }
        return ai::FT_NUM_FIRING_TYPES;
    }

    bool IsGunWithShellsPoolLimit(ai::Obj const* gun)
    {
        // RVA 0x5514A0
        if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
        {
            return g->IsWithShellsPoolLimit();
        }
        if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
        {
            return cg->IsWithShellsPoolLimit();
        }
        return false;
    }

    bool CanGunBeReloaded(ai::Obj const* gun)
    {
        // RVA 0x5538B0
        return IsGunWithCharging(gun) && IsGunWithShellsPoolLimit(gun);
    }

    int GetFuelPriceForOneUnit(int vehicleId, int townId)
    {
        // RVA 0x553940 - NOTE: the shipped body only checks that both ids resolve
        // and then returns a flat 1; there is no per-town fuel pricing.
        ai::Obj* vehicle = ai::theObjects->GetEntityByObjId(vehicleId);
        ai::Obj* town = ai::theObjects->GetEntityByObjId(townId);
        bool const bVehicle = vehicle && vehicle->IsKindOf(&ai::Vehicle::m_classVehicle);
        bool const bTown = town && town->IsKindOf(&ai::Town::m_classTown);
        return (bVehicle && bTown) ? 1 : -1;
    }

    unsigned int GetGunShellsPoolSize(ai::Obj const* gun)
    {
        // RVA 0x5516A0
        if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
        {
            return g->GetShellsPoolSize();
        }
        if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
        {
            return cg->GetShellsPoolSize();
        }
        return 0;
    }

    ai::DamageType GetGunDamageType(ai::Obj const* gun)
    {
        // RVA 0x5516E0
        if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
        {
            return g->GetDamageType();
        }
        if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
        {
            return cg->GetDamageType();
        }
        return ai::DAMAGE_NUM_TYPES;
    }

    float GetGunDamage(ai::Obj const* gun)
    {
        // RVA 0x551720
        if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
        {
            return g->GetDamage();
        }
        if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
        {
            return cg->GetDamage();
        }
        return 0.0f;
    }

    float GetGunDurability(ai::Obj const* gun)
    {
        // RVA 0x554EA0 - a plain gun reads the durability component directly,
        // a compound one asks the base class.
        if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
        {
            return g->Durability().value().get();
        }
        if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
        {
            return cg->GetDurability();
        }
        return 0.0f;
    }

    float GetGunMaxDurability(ai::Obj const* gun)
    {
        // RVA 0x554EE0
        if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
        {
            return g->Durability().maxValue().get();
        }
        if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
        {
            return cg->GetMaxDurability();
        }
        return 0.0f;
    }

    bool IsGunDurabilityEnoughForFiring(ai::Obj const* gun)
    {
        // RVA 0x554F20
        if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
        {
            return g->IsDurabilityEnoughForFiring();
        }
        if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
        {
            return cg->IsDurabilityEnoughForFiring();
        }
        return false;
    }

    float GetGunAccuracy(ai::Obj const* gun)
    {
        // RVA 0x5517B0 - compound guns are followed down to their first sub-part
        // until a bullet launcher turns up.
        ai::Obj const* current = gun;
        while (current)
        {
            if (auto const* bl = RT_DYNCAST(current, ai::BulletLauncher const))
            {
                return bl->GetAccuracyClamped();
            }
            auto const* cg = RT_DYNCAST(current, ai::CompoundGun const);
            if (!cg)
            {
                // A gun that is not a bullet launcher has no spread to report.
                return 100.0f;
            }
            current = cg->begin()->second.vp;
        }
        return 0.0f;
    }

    CStr DamageType2Str(ai::DamageType damageType)
    {
        // RVA 0x5513F0
        struct DamageType2StrEntry
        {
            ai::DamageType m_damageType;
            char const* m_damageName;
        };
        static DamageType2StrEntry const l_damageType2Str[] = {
            {ai::DAMAGE_PIERCING, "Piercing"},
            {ai::DAMAGE_BLAST, "Blast"},
            {ai::DAMAGE_ENERGY, "Energy"},
            {ai::DAMAGE_WATER, "Water"},
        };

        for (auto const& entry : l_damageType2Str)
        {
            if (entry.m_damageType == damageType)
            {
                return entry.m_damageName;
            }
        }
        return {};
    }

    int GetBaseBuyPriceByObjId(int objId)
    {
        // RVA 0x550D70 - the list price, with no town or workshop coefficients
        // applied.
        if (objId == -1)
        {
            return -1;
        }
        ai::Obj const* obj = ai::theObjects->GetEntityByObjId(objId);
        if (!obj || !obj->IsKindOf(&ai::Obj::m_classObj))
        {
            return -1;
        }
        return static_cast<int>(obj->GetPrice(nullptr));
    }

    ObjectOwnerType GetObjectOwnerType(int objId)
    {
        // RVA 0x550EA0 - while the trade window is up it is the authority on who
        // owns what; otherwise ownership follows the repository the object is in.
        if (objId == -1)
        {
            return OWNER_INVALID;
        }
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(objId);
        if (!obj)
        {
            return OWNER_INVALID;
        }

        ref_ptr<m3d::ui::Wnd> const wndZnayuKakProdat =
            M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_ZNAYU_KAK_PRODAT);
        if (wndZnayuKakProdat && wndZnayuKakProdat->IsKindOf(&ZnayuKakProdatWnd::m_classZnayuKakProdatWnd) &&
            wndZnayuKakProdat->IsChildOf(M3D_APP))
        {
            switch (static_cast<ZnayuKakProdatWnd const*>(wndZnayuKakProdat.get())->GetItemBelong(objId))
            {
            case ZnayuKakProdatWnd::BELONG_PLAYER:
                return OWNER_PLAYER;
            case ZnayuKakProdatWnd::BELONG_WORKSHOP:
                return OWNER_TOWN;
            default:
                return OWNER_INVALID;
            }
        }

        ai::Vehicle* playerVehicle = GetPlayerVehicle();
        ai::Vehicle* workshopVehicle = M3D_APP->m_pInterfaceManager->GetVehicleSellingInWorkshop();
        ai::Town* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();

        ai::GeomRepository const* parentRepository = obj->GetParentRepository();
        if (parentRepository)
        {
            if (playerVehicle && parentRepository == playerVehicle->GetRepository())
            {
                return OWNER_PLAYER;
            }
            if (town)
            {
                auto const& buildings = town->GetAllBuildings();
                for (int i = 0; i < static_cast<int>(buildings.size()); ++i)
                {
                    auto* building = buildings[i];
                    if (!building || !building->IsKindOf(&ai::Workshop::m_classWorkshop))
                    {
                        continue;
                    }
                    auto* workshop = static_cast<ai::Workshop*>(building);
                    for (int type = ai::WORKSHOP_GOODS; type < ai::WORKSHOP_NUM_TYPES; ++type)
                    {
                        if (parentRepository ==
                            workshop->GetRepositoryByType(static_cast<ai::WorkshopRepositoryType>(type)))
                        {
                            return OWNER_TOWN;
                        }
                    }
                }
                if (workshopVehicle && parentRepository == workshopVehicle->GetRepository())
                {
                    return OWNER_TOWN;
                }
                return OWNER_INVALID;
            }
            return OWNER_OTHER;
        }

        // Loose objects: a vehicle, part or gadget still counts as "other",
        // anything else has no owner at all.
        if (obj->IsKindOf(&ai::Vehicle::m_classVehicle) || obj->IsKindOf(&ai::VehiclePart::m_classVehiclePart) ||
            obj->IsKindOf(&ai::Gadget::m_classGadget))
        {
            return OWNER_OTHER;
        }
        return OWNER_INVALID;
    }

    int GetPriceSmart(int objId)
    {
        // RVA 0x551180
        if (objId == -1)
        {
            return -1;
        }

        ref_ptr<m3d::ui::Wnd> const wndZnayuKakProdat =
            M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_ZNAYU_KAK_PRODAT);
        if (wndZnayuKakProdat && wndZnayuKakProdat->IsKindOf(&ZnayuKakProdatWnd::m_classZnayuKakProdatWnd) &&
            wndZnayuKakProdat->IsChildOf(M3D_APP))
        {
            return static_cast<ZnayuKakProdatWnd const*>(wndZnayuKakProdat.get())->GetItemCost(objId);
        }

        ai::Town* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
        switch (GetObjectOwnerType(objId))
        {
        case OWNER_PLAYER:
            if (town)
            {
                return GetSellPriceByObjId(objId, town->GetId());
            }
            break;
        case OWNER_TOWN:
            return town ? GetBuyPriceByObjId(objId, town->GetId()) : GetBaseBuyPriceByObjId(objId);
        case OWNER_OTHER:
            break;
        default:
            return -1;
        }
        return ai::GetIntPrice(static_cast<float>(GetBaseBuyPriceByObjId(objId)) * 0.5f);
    }

    float Angle0To2Pi(float angle)
    {
        float const turns = static_cast<float>(static_cast<int>(angle * 0.15915494f));
        if (angle >= 0.0f)
        {
            return angle - turns * 6.2831855f;
        }
        return angle - turns * 6.2831855f + 6.2831855f;
    }

    float AngleMinusPiToPi(float angle)
    {
        float const turns = static_cast<float>(static_cast<int>(angle * 0.15915494f));
        if (angle > 3.1415927f)
        {
            return angle - turns * 6.2831855f - 6.2831855f;
        }
        if (angle < -3.1415927f)
        {
            return angle - turns * 6.2831855f + 6.2831855f;
        }
        return angle;
    }

    namespace
    {
        bool GetPropertyValFromObj(float& propertyVal, ai::Obj const* obj, int propertyId)
        {
            propertyVal = 0.0f;
            if (!obj || propertyId == -1)
            {
                return false;
            }
            m3d::AIParam const prop = obj->GetPropertyById(propertyId);
            if (prop.GetType() != m3d::AIPARAM_FLOAT)
            {
                return false;
            }
            propertyVal = prop.GetAsFloat();
            return true;
        }

        bool GetDefaultPropertyValFromObj(float& propertyVal, ai::Obj const* obj, int propertyId)
        {
            propertyVal = 0.0f;
            if (!obj || propertyId == -1)
            {
                return false;
            }
            m3d::AIParam const prop = obj->GetPropertyDefaultById(propertyId);
            if (prop.GetType() != m3d::AIPARAM_FLOAT)
            {
                return false;
            }
            propertyVal = prop.GetAsFloat();
            return true;
        }

        bool IsGadgetModificationApplicableToObjProperty(
            ai::GadgetPrototypeInfo::ModificationInfo const& modification,
            ai::Obj const* obj,
            int propertyId)
        {
            if (!obj || propertyId == -1 || modification.m_propertyName.empty())
            {
                return false;
            }
            if (!(obj->GetPropertyName(propertyId) == modification.m_propertyName))
            {
                return false;
            }
            ai::PrototypeInfo const* protoInfo = obj->GetPrototypeInfo();
            if (!protoInfo)
            {
                return false;
            }

            switch (modification.m_applierInfo.applierType)
            {
            case ai::GadgetPrototypeInfo::GA_VEHICLE:
                return obj->IsKindOf(&ai::Vehicle::m_classVehicle);
            case ai::GadgetPrototypeInfo::GA_OBJECT_BY_RESOURCE:
                return ai::theResourceManager->bResourceIsKindOf(
                    protoInfo->m_resourceId, modification.m_applierInfo.targetResourceId);
            case ai::GadgetPrototypeInfo::GA_GUN_BY_TYPE:
                if (ai::theResourceManager->bResourceIsKindOf(
                        protoInfo->m_resourceId, ai::theResourceManager->GetResourceId("GUN")))
                {
                    return GetGunFiringType(obj) == modification.m_applierInfo.targetFiringType;
                }
                return false;
            default:
                return false;
            }
        }

        // The objects a given vehicle property is actually stored on: most
        // chassis stats live on the chassis part, cabin-driven stats (speed,
        // torque, control) on the cabin, and the two "storage capacity"
        // properties are shared between the cabin and basket.
        void GetVehicleObjectsForProperty(
            ai::Vehicle const* vehicle,
            int propertyId,
            std::vector<ai::Obj const*>& objects)
        {
            objects.clear();
            if (!vehicle)
            {
                return;
            }
            switch (propertyId)
            {
            case 9:
            case 10:
            case 26:
            case 27:
                if (auto const* chassis = vehicle->GetChassis())
                {
                    objects.push_back(chassis);
                }
                break;
            case 12:
                objects.push_back(vehicle);
                break;
            case 19:
            case 20:
                if (auto const* cabin = vehicle->GetCabin())
                {
                    objects.push_back(cabin);
                    if (auto const* basket = vehicle->GetBasket())
                    {
                        objects.push_back(basket);
                    }
                }
                break;
            case 22:
            case 23:
            case 25:
                if (auto const* cabin = vehicle->GetCabin())
                {
                    objects.push_back(cabin);
                }
                break;
            default:
                break;
            }
        }
        bool GetBasePropertyValFromObjImpl(
            float& basePropertyVal,
            ai::Obj const* obj,
            ai::Vehicle const* vehicle,
            int propertyId)
        {
            basePropertyVal = 0.0f;
            if (!obj || !vehicle || propertyId == -1)
            {
                return false;
            }
            float value = 0.0f;
            if (!GetPropertyValFromObj(value, obj, propertyId))
            {
                return false;
            }
            float defaultValue = 0.0f;
            if (!GetDefaultPropertyValFromObj(defaultValue, obj, propertyId))
            {
                return false;
            }

            // Undo every installed gadget modification that applies to this
            // property, recovering the value the object would have without them.
            for (auto const& entry : vehicle->GetGadgets())
            {
                ai::Gadget const* gadget = entry.second;
                if (!gadget)
                {
                    continue;
                }
                auto const* gadgetPi = gadget->GetPrototypeInfo();
                if (!gadgetPi)
                {
                    continue;
                }
                for (auto const& modification : gadgetPi->GetModifications())
                {
                    if (!IsGadgetModificationApplicableToObjProperty(modification, obj, propertyId))
                    {
                        continue;
                    }
                    if (modification.m_modificationType == ai::GadgetPrototypeInfo::ModificationInfo::MULTIPLY)
                    {
                        value -= modification.m_value.GetAsFloat() * defaultValue;
                    }
                    else
                    {
                        value -= modification.m_value.GetAsFloat();
                    }
                }
            }
            basePropertyVal = value;
            return true;
        }
    }  // namespace

    float GetBasePropertyValFromObj(ai::Obj const* obj, ai::Vehicle const* vehicle, int propertyId)
    {
        float value = 0.0f;
        GetBasePropertyValFromObjImpl(value, obj, vehicle, propertyId);
        return value;
    }

    float GetBasePropertyValFromVehicle(ai::Vehicle const* vehicle, int propertyId)
    {
        if (!vehicle || propertyId == -1)
        {
            return 0.0f;
        }
        std::vector<ai::Obj const*> objects;
        GetVehicleObjectsForProperty(vehicle, propertyId, objects);

        float total = 0.0f;
        for (ai::Obj const* obj : objects)
        {
            float value = 0.0f;
            if (!obj || !GetBasePropertyValFromObjImpl(value, obj, vehicle, propertyId))
            {
                return 0.0f;
            }
            total += value;
        }
        return total;
    }
}  // namespace help
