#include "guihelper.h"
#include <client.h>
#include <level.h>
#include <world.h>

#include "core/kernel.h"
#include "core/log.h"
#include "file/fileenum.h"
#include "file/fileserver.h"
#include "ui/ui.h"
#include "ui/button.h"
#include "ui/image.h"
#include "ui/ui_srv.h"

#include <sstream>

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
             if (auto * level = m3d::pClient->GetWorld().m_level; level != nullptr)
             {
                 return level->GetLevelName();
             }
         }
         return {};
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
                for (child = srcWnd->GetFirstChild(); child && child->IsKindOf(RT_CLASS_LOCAL(Wnd)); child = child->GetNextSibling())
                {
                    auto childWnd = dynamic_cast<Wnd*>(child);
                    auto newObjClass = child->GetClass();
                    auto newObj = m3d::g_Kernel->New(newObjClass);
                    auto newWnd = dynamic_cast<Wnd*>(newObj);
                    if (!newObj || newWnd->Create(childWnd->GetText(), childWnd->GetStyle(), childWnd->GetBounds(), childWnd->GetId()) == 0)
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
                            newObjButton->SetImaged(childButton->GetImageRegular(), childButton->GetImageDown(), childButton->GetImageIn(), childButton->GetImageDisabled());
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
}
