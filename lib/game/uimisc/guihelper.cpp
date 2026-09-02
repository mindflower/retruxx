#include "guihelper.h"
#include <client.h>
#include <level.h>
#include <world.h>

#include "core/kernel.h"
#include "core/log.h"
#include "file/fileenum.h"
#include "file/fileserver.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/physicbodies/vehiclepart.h"
#include "ui/ui.h"
#include "ui/button.h"
#include "ui/font.h"
#include "ui/image.h"
#include "ui/ui_srv.h"
#include <m3dapp.h>

#include <sstream>
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

    UnifyQuestStatus GetQuestUnifyStatusByQuestId(help::QuestType /*questType*/, int /*questId*/)
    {
        // TODO: implement (ExMachina 1.02 NoCD RVA 0x1555b0).
        RETRUXX_NOT_IMPLEMENTED;
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
    ActionType GetRandomMoveAnimation(m3d::AnimatedModel*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
    CStr GetKeysForImpulse(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    int DestroyVehicle(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::Bar* GetBarWithBarmanForTown(ai::Town const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::Bar* GetBarWithoutBarmanForTown(ai::Town const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace help
