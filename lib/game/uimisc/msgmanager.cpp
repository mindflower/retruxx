#include "msgmanager.h"

#include "config.h"
#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"
#include "game/m3dgame.h"
#include <client.h>
#include <core/ref_ptr.h>
#include <core/scoped_ptr.h>
#include <cstdlib>
#include <file/fileserver.h>
#include <file/filestream.h>

#include "level.h"
#include "world.h"

RT_CLASS_EXPORT_METHOD_DEFINE(MsgManager, ShowMsgBox)
{
    auto manager = (MsgManager*)context->asObject(0, "MsgManager");
    auto msg = context->asInt(1);
    auto pause = context->asBool(2);
    auto res = manager->ShowMsgBox(msg, pause);
    context->pushInt(res);
    return 1;
}

RT_CLASS_EXPORTS_BEGIN(MsgManager)
RT_CLASS_EXPORT(MsgManager, m3d::METHOD, ShowMsgBox, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MsgManager);

namespace
{
    struct Str2MsgTypeEntry
    {
        CStr m_str;
        MsgInfo::MsgType m_msgType;
    };

    // Mirrors the shipped game's anonymous-namespace l_str2MsgType table
    // (RVA 0x5692C0); an unmatched string maps to MSGTYPE_SCROLL.
    Str2MsgTypeEntry const l_str2MsgType[2] = {
        {CStr("NORMAL"), MsgInfo::MSGTYPE_NORMAL},
        {CStr("SCROLL"), MsgInfo::MSGTYPE_SCROLL},
    };
}  // namespace

MsgInfo::MsgInfo()
{
    m_id = -1;
    m_image.SetInvalid();
    m_imageUpOverlay.SetInvalid();
    m_imageDownOverlay.SetInvalid();
    m_time = -1;
    m_mbFlags = m3d::ui::MBX_OK;
    m_modelSkin = 0;
    m_modelCfg = 0;
    m_modelSlot = 0;
    m_bModelAutosized = false;
    m_msgType = MSGTYPE_NORMAL;
    m_fontSize = -1.0f;
    m_scrollSpeed = 1.0f;
}

MsgInfo::MsgInfo(MsgInfo const& rhs) :
    m_id(rhs.m_id),
    m_msg(rhs.m_msg),
    m_imageFileName(rhs.m_imageFileName),
    m_imageUpOverlayFileName(rhs.m_imageUpOverlayFileName),
    m_imageDownOverlayFileName(rhs.m_imageDownOverlayFileName),
    m_image(rhs.m_image),
    m_imageUpOverlay(rhs.m_imageUpOverlay),
    m_imageDownOverlay(rhs.m_imageDownOverlay),
    m_soundFileName(rhs.m_soundFileName),
    m_time(rhs.m_time),
    m_mbFlags(rhs.m_mbFlags),
    m_modelName(rhs.m_modelName),
    m_modelSkin(rhs.m_modelSkin),
    m_modelCfg(rhs.m_modelCfg),
    m_modelSlot(rhs.m_modelSlot),
    m_bModelAutosized(rhs.m_bModelAutosized),
    m_msgType(rhs.m_msgType),
    m_fontSize(rhs.m_fontSize),
    m_scrollSpeed(rhs.m_scrollSpeed)
{
    // The shipped binary never emits a MsgInfo copy constructor - MsgManager only
    // ever stores MsgInfo pointers. Provided for completeness; the copied texture
    // handles are shared, so a copy must not outlive the original (both
    // destructors call ReleaseTexture on the same handles).
}

MsgInfo::~MsgInfo()
{
    M3D_RENDERER->ReleaseTexture(m_image);
    M3D_RENDERER->ReleaseTexture(m_imageUpOverlay);
    M3D_RENDERER->ReleaseTexture(m_imageDownOverlay);
}

int MsgInfo::GetId() const
{
    return m_id;
}

int MsgInfo::GetParentId() const
{
    // MsgInfo carries no parent hierarchy in this build (no member and no shipped
    // code); -1 matches the "no parent" convention used elsewhere.
    return -1;
}

CStr const& MsgInfo::GetMsg() const
{
    return m_msg;
}

CStr const& MsgInfo::GetImageFileName() const
{
    return m_imageFileName;
}

CStr const& MsgInfo::GetImageUpOverlayFileName() const
{
    return m_imageUpOverlayFileName;
}

CStr const& MsgInfo::GetImageDownOverlayFileName() const
{
    return m_imageDownOverlayFileName;
}

m3d::rend::TexHandle MsgInfo::GetImage() const
{
    return m_image;
}

m3d::rend::TexHandle MsgInfo::GetImageUpOverlay() const
{
    return m_imageUpOverlay;
}

m3d::rend::TexHandle MsgInfo::GetImageDownOverlay() const
{
    return m_imageDownOverlay;
}

CStr const& MsgInfo::GetSoundFileName() const
{
    return m_soundFileName;
}

int MsgInfo::GetTime() const
{
    return m_time;
}

m3d::ui::MbFlags MsgInfo::GetMbFlags() const
{
    return m_mbFlags;
}

CStr const& MsgInfo::GetModelName() const
{
    return m_modelName;
}

unsigned int MsgInfo::GetModelSkin() const
{
    return m_modelSkin;
}

unsigned int MsgInfo::GetModelCfg() const
{
    return m_modelCfg;
}

int MsgInfo::GetModelSlot() const
{
    return m_modelSlot;
}

bool MsgInfo::IsModelAutosized() const
{
    return m_bModelAutosized;
}

MsgInfo::MsgType MsgInfo::GetMsgType() const
{
    return m_msgType;
}

float MsgInfo::GetFontSize() const
{
    return m_fontSize;
}

float MsgInfo::GetScrollSpeed() const
{
    return m_scrollSpeed;
}

MsgInfo::MsgType MsgInfo::Str2MsgType(CStr const& str) const
{
    for (auto const& entry : l_str2MsgType)
    {
        if (!CStr::my_strcmp(entry.m_str.c_str(), str.c_str()))
        {
            return entry.m_msgType;
        }
    }
    return MSGTYPE_SCROLL;
}

void MsgInfo::AddImage(CStr const& fileName, m3d::rend::TexHandle& tex)
{
    M3D_RENDERER->ReleaseTexture(tex);
    if (fileName.empty())
    {
        return;
    }

    tex = M3D_RENDERER->AddTexture(fileName, 4);
    if (!tex.IsValid())
    {
        M3D_LOG_ERR(
            "MsgInfo::AddImage error - cannot add texture '" + fileName + "' for MsgInfo id = '" + CStr(m_id) + "'");
    }
    else
    {
        M3D_RENDERER->SetTextureParameter(tex, m3d::rend::TM_WRAP_S, 3);
        M3D_RENDERER->SetTextureParameter(tex, m3d::rend::TM_WRAP_T, 3);
    }
}

int MsgInfo::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    if (!xmlFile || !xmlNode)
    {
        M3D_LOG_INFO("MsgInfo::LoadFromXml error - invalid params");
        return 0;
    }

    if (xmlNode->IsEmpty())
    {
        M3D_LOG_INFO("MsgInfo::LoadFromXml error - invalid id");
        return 0;
    }
    char const* idAttr = xmlNode->GetAttribute("id");
    int const id = idAttr ? atoi(idAttr) : -1;
    if (!idAttr || id == -1)
    {
        M3D_LOG_INFO("MsgInfo::LoadFromXml error - invalid id");
        return 0;
    }
    m_id = id;

    CStr sMsg;
    m3d::SafeStrAttrib(sMsg, xmlNode, "value");
    m_msg = sMsg;

    m3d::SafeStrAttrib(m_imageFileName, xmlNode, "image");
    AddImage(m_imageFileName, m_image);
    m3d::SafeStrAttrib(m_imageUpOverlayFileName, xmlNode, "upOverlay");
    AddImage(m_imageUpOverlayFileName, m_imageUpOverlay);
    m3d::SafeStrAttrib(m_imageDownOverlayFileName, xmlNode, "downOverlay");
    AddImage(m_imageDownOverlayFileName, m_imageDownOverlay);

    m3d::SafeStrAttrib(m_soundFileName, xmlNode, "sound");
    m3d::SafeIntAttrib(m_time, xmlNode, "time");

    int numButtons = m_mbFlags;
    m3d::SafeIntAttrib(numButtons, xmlNode, "numButtons");
    if (numButtons == 1)
    {
        m_mbFlags = m3d::ui::MBX_OK;
    }
    else if (numButtons == 2)
    {
        m_mbFlags = m3d::ui::MBX_YES_NO;
    }
    else if (numButtons == 3)
    {
        m_mbFlags = m3d::ui::MBX_YES_NO_CANCEL;
    }
    else
    {
        M3D_LOG_INFO("Invalid number of buttons message " + CStr(m_id));
    }

    m3d::SafeStrAttrib(m_modelName, xmlNode, "modelName");
    m3d::SafeUintAttrib(m_modelSkin, xmlNode, "modelSkin");
    m3d::SafeUintAttrib(m_modelCfg, xmlNode, "modelCfg");
    m3d::SafeIntAttrib(m_modelSlot, xmlNode, "modelSlot");
    m3d::SafeBoolAttrib(m_bModelAutosized, xmlNode, "modelAutosized");

    CStr strMsgType;
    m3d::SafeStrAttrib(strMsgType, xmlNode, "msgType");
    if (!strMsgType.empty())
    {
        m_msgType = Str2MsgType(strMsgType);
    }

    m3d::SafeFloatAttrib(m_fontSize, xmlNode, "fontSize");
    m3d::SafeFloatAttrib(m_scrollSpeed, xmlNode, "scrollSpeed");
    return 1;
}

int MsgManager::ShowMsgBox(int msgId, bool pause)
{
    // RVA 0x569FA0: looks the id up in the level map then the global map (that
    // is exactly GetMsgInfo), and additionally treats a stored null entry as
    // "not found".
    MsgInfo const* info = GetMsgInfo(msgId);
    if (!info)
    {
        M3D_LOG_INFO(
            "MsgManager::ShowMsgBox error - MsgInfo for msg with id " + CStr(msgId) + " not found");
        // The shipped code returns -1; MbRetCodes has no error member, so
        // callers read any non-positive value as "no dialog shown".
        return -1;
    }
    // Caption is deliberately empty in the binary (it passes ""); the box shows
    // only the MsgInfo text with its configured button set.
    return M3D_APP->RunMsgBoxDlg(CStr(), info->GetMsg(), info->GetMbFlags(), pause);
}

int MsgManager::Init(bool bGlobal)
{
    Clear(bGlobal);

    CStr fileName;
    if (bGlobal)
    {
        fileName = M3D_ENGINE_CFG.m_ui_pathToStrings.GetS();
    }
    else
    {
        fileName =
            m3d::pClient->GetWorld().m_level->GetFullPathNameA(m3d::pClient->GetWorld().m_level->m_dialogStrings);
    }
    return LoadFromXml(fileName, bGlobal);
}

void MsgManager::Clear(bool bGlobal)
{
    for (auto& info : m_levelMsgs)
    {
        delete info.second;
    }
    m_levelMsgs.clear();

    if (bGlobal)
    {
        for (auto& info : m_globalMsgs)
        {
            delete info.second;
        }
        m_globalMsgs.clear();
    }
}

m3d::Class* MsgManager::GetClass() const
{
    return RT_CLASS_LOCAL(MsgManager);
}

m3d::Class* MsgManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

MsgInfo const* MsgManager::GetMsgInfo(int msgId) const
{
    auto it = m_levelMsgs.find(msgId);
    if (it != m_levelMsgs.end())
    {
        return it->second;
    }

    auto globalIt = m_globalMsgs.find(msgId);
    if (globalIt != m_globalMsgs.end())
    {
        return globalIt->second;
    }
    return nullptr;
}

m3d::Object* MsgManager::CreateObject()
{
    return new MsgManager;
}

MsgManager::~MsgManager()
{
    // RVA 0x569510: clears both maps (deleting every owned MsgInfo) before the
    // std::map members tear their now-empty trees down.
    Clear(true);
}

m3d::Object* MsgManager::Clone()
{
    // RVA 0x568A20 allocates a MsgManager and runs the copy ctor, which (see
    // below) yields an empty manager - a clone must be Init()'d again.
    return new MsgManager(*this);
}

int MsgManager::AddMsg(MsgInfo* msgInfo, bool bGlobal)
{
    // RVA 0x569DD0
    if (!msgInfo)
    {
        return 0;
    }
    int const id = msgInfo->GetId();
    if (id == -1)
    {
        return 0;
    }

    auto& msgs = bGlobal ? m_globalMsgs : m_levelMsgs;
    auto it = msgs.find(id);
    if (it != msgs.end())
    {
        M3D_LOG_INFO(
            "MsgManager::AddMsg warning - msg with id " + CStr(id) + " already exists and wiil be replaced");
        delete it->second;
        it->second = nullptr;
        msgs.erase(it);
    }
    msgs[id] = msgInfo;
    return 1;
}

MsgManager::MsgManager(MsgManager const&)
{
    // RVA 0x5694B0 is byte-identical to the default ctor: it builds the Object
    // base and two empty maps and never touches the source. Clone() relies on
    // this - a copied MsgManager carries no messages.
}

MsgManager::MsgManager()
{
}

int MsgManager::LoadFromXml(CStr const& fileName, bool bGlobal)
{
    // RVA 0x5697B0
    scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
    if (!stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
    {
        M3D_LOG_INFO("Can't open file " + fileName + " for read.");
        return 0;
    }

    ref_ptr xmlFile = m3d::g_Kernel->CreateXmlFile();
    if (!xmlFile->Read(*stream))
    {
        M3D_LOG_INFO("Error: cannot parse " + fileName + " (" + CStr(xmlFile->GetError()) + ") ");
        return 0;
    }
    stream->Close();

    ref_ptr rootNode = xmlFile->CreateNode();
    xmlFile->GetFirstChild(rootNode, "resource");
    if (rootNode->IsEmpty())
    {
        M3D_LOG_INFO("MsgManager::LoadFromXml error - cannot find root node resource");
        return 0;
    }

    int res = 1;
    ref_ptr node = xmlFile->CreateNode();
    for (rootNode->GetFirstChild(node, "string"); !node->IsEmpty(); node->GetNextSibling(node, "string"))
    {
        MsgInfo* info = new MsgInfo();
        if (!info->LoadFromXml(xmlFile, node) || !AddMsg(info, bGlobal))
        {
            // AddMsg took ownership only on success; drop the failed one.
            delete info;
            res = 0;
        }
    }
    if (!res)
    {
        M3D_LOG_INFO("MsgManager inited with errors");
    }
    return res;
}
