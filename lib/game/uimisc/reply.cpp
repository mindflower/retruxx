#include "reply.h"

#include <cstdlib>
#include <cstring>

#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <core/scoped_ptr.h>

#include <file/fileserver.h>
#include <file/filestream.h>
#include <script/scriptserver.h>

#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>

#include <server/dynamicquestmanager.h>
#include <server/utils.h>
#include <server/objects/dynamicquest.h>
#include <server/objects/npc.h>

namespace
{
    // RVA 0x177F10 - "PLAYER" / "NPC" -> Reply::Role, anything else -> ROLE_INVALID.
    Reply::Role Str2Role(CStr const& str)
    {
        struct
        {
            Reply::Role m_role;
            char const* m_name;
        } const l_str2Role[2] = {
            {Reply::ROLE_PLAYER, "PLAYER"},
            {Reply::ROLE_NPC, "NPC"},
        };
        for (auto const& entry : l_str2Role)
        {
            if (std::strcmp(entry.m_name, str.c_str()) == 0)
            {
                return entry.m_role;
            }
        }
        return Reply::ROLE_INVALID;
    }

    // RVA 0x177FC0 - in-place replace every single quote with a double quote so
    // the fragment is safe to embed inside a Lua string.
    void ReplaceQuates(CStr& str)
    {
        int const len = str.length();
        for (int i = 0; i < len; ++i)
        {
            if (str[i] == '\'')
            {
                str[i] = '"';
            }
        }
    }

    // RVA 0x178000 - wrap a bare condition expression into a Lua statement that
    // reports the outcome back through RepliesManager:SetConditionValue.
    CStr FixScriptCondition(CStr const& script)
    {
        if (script.empty())
        {
            return script;
        }
        return CStr("if (") + script +
            ") then RepliesManager:SetConditionValue( 1 ) else RepliesManager:SetConditionValue( 0 ) end";
    }

    // RVA 0x178140 - "(source @ line): errText description" for a script error.
    CStr GetFormattedScriptErrorDesc(m3d::eScriptError err)
    {
        CStr desc;
        if (err != m3d::SUCCESS)
        {
            m3d::ScriptServer& scriptServer = m3d::g_Kernel->GetScriptServer();
            char const* errorDescString = scriptServer.getErrorDescString(err);
            m3d::auxScriptErrorDesc const& lastError = scriptServer.getLastErrorDesc();
            desc = CStr("(") + lastError.sourceString + " @ " + CStr(lastError.lineNumber) + "): " + errorDescString +
                " " + lastError.descriptionString;
        }
        return desc;
    }

    // RVA 0x178310 - dynamic-quest QuestType -> its resource-name token.
    CStr DQuestType2Str(ai::DynamicQuestManager::QuestType type)
    {
        struct
        {
            ai::DynamicQuestManager::QuestType m_type;
            char const* m_str;
        } const l_type2str[5] = {
            {ai::DynamicQuestManager::TYPE_DESTROY, "Destroy"},
            {ai::DynamicQuestManager::TYPE_REACH, "Reach"},
            {ai::DynamicQuestManager::TYPE_CONVOY, "Convoy"},
            {ai::DynamicQuestManager::TYPE_PEACE, "Peace"},
            {ai::DynamicQuestManager::TYPE_HUNT, "Hunt"},
        };
        for (auto const& entry : l_type2str)
        {
            if (entry.m_type == type)
            {
                return CStr(entry.m_str);
            }
        }
        return CStr();
    }
}  // namespace

// ===========================================================================
//  Reply
// ===========================================================================

Reply::Reply()
{
    // RVA 0x1783A0
    m_role = ROLE_INVALID;
}

Reply::Reply(Reply const& rhs) :
    m_name(rhs.m_name),
    m_text(rhs.m_text),
    m_role(rhs.m_role),
    m_scriptCondition(rhs.m_scriptCondition),
    m_scriptResult(rhs.m_scriptResult),
    m_nextReplies(rhs.m_nextReplies)
{
}

Reply::~Reply()
{
    // RVA 0x1783E0 - the CStr and vector members clean up after themselves.
}

void Reply::SetName(CStr const& name)
{
    // RVA 0x178490
    m_name = name;
}

CStr const& Reply::GetName() const
{
    // RVA 0x1784A0
    return m_name;
}

void Reply::SetText(CStr const text)
{
    // RVA 0x1784B0
    m_text = text;
}

CStr const& Reply::GetText() const
{
    // RVA 0x1784E0
    return m_text;
}

void Reply::SetRole(Reply::Role role)
{
    // RVA 0x1784F0
    m_role = role;
}

Reply::Role Reply::GetRole() const
{
    // RVA 0x178500
    return m_role;
}

void Reply::SetScriptCondition(CStr const& script)
{
    // RVA 0x178510
    m_scriptCondition = script;
}

CStr const& Reply::GetScriptCondition() const
{
    // RVA 0x178520
    return m_scriptCondition;
}

void Reply::SetScriptResult(CStr const& script)
{
    // RVA 0x178530
    m_scriptResult = script;
}

CStr const& Reply::GetScriptResult() const
{
    // RVA 0x178540
    return m_scriptResult;
}

void Reply::SetNextDialogs(std::vector<CStr> const& nextReplies)
{
    // RVA 0x178550
    m_nextReplies = nextReplies;
}

std::vector<CStr> const& Reply::GetNextReplies() const
{
    // RVA 0x178560
    return m_nextReplies;
}

int Reply::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    // RVA 0x178570
    if (!xmlFile)
    {
        M3D_LOG_INFO("Reply: error loading - invalid xml file");
        return 0;
    }
    if (!xmlNode || xmlNode->IsEmpty())
    {
        M3D_LOG_INFO("Reply: error loading - invalid node");
        return 0;
    }

    m3d::SafeStrAttrib(m_name, xmlNode, "name");
    if (m_name.empty())
    {
        M3D_LOG_INFO("Reply: error loading - empty name");
        return 0;
    }

    CStr cText;
    m3d::SafeStrAttrib(cText, xmlNode, "text");
    m_text = cText;

    CStr strRole;
    m3d::SafeStrAttrib(strRole, xmlNode, "role");
    m_role = Str2Role(strRole);
    if (m_role == ROLE_INVALID)
    {
        M3D_LOG_INFO("Reply: error loading - invalid role");
        return 0;
    }

    m3d::SafeStrAttrib(m_scriptCondition, xmlNode, "scriptCondition");
    ReplaceQuates(m_scriptCondition);
    m3d::SafeStrAttrib(m_scriptResult, xmlNode, "scriptResult");
    ReplaceQuates(m_scriptResult);

    CStr strNextReplies;
    m3d::SafeStrAttrib(strNextReplies, xmlNode, "nextReplies");
    ai::StrToStringVector(strNextReplies, m_nextReplies);
    return 1;
}

// ===========================================================================
//  RepliesManager - script exports
// ===========================================================================

RT_CLASS_EXPORT_METHOD_DEFINE(RepliesManager, SetConditionValue)
{
    auto* mgr = static_cast<RepliesManager*>(context->asObject(0, "RepliesManager"));
    mgr->SetConditionValue(context->asInt(1));
    return 0;
}

RT_CLASS_EXPORT_METHOD_DEFINE(RepliesManager, Init)
{
    auto* mgr = static_cast<RepliesManager*>(context->asObject(0, "RepliesManager"));
    context->pushInt(mgr->Init());
    return 1;
}

RT_CLASS_EXPORTS_BEGIN(RepliesManager)
RT_CLASS_EXPORT(RepliesManager, m3d::METHOD, SetConditionValue, "", "", "")
RT_CLASS_EXPORT(RepliesManager, m3d::METHOD, Init, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RepliesManager);

// ===========================================================================
//  RepliesManager
// ===========================================================================

RepliesManager::RepliesManager()
{
    // RVA 0x1788C0 - empty map, not yet inited.
}

RepliesManager::RepliesManager(RepliesManager const&) : RepliesManager()
{
    // Matches the original: the copy ctor default-constructs the base and an
    // empty reply map; nothing is copied. Reached only via Clone().
}

RepliesManager::~RepliesManager()
{
    // RVA 0x1789B0
    Clear();
}

m3d::Object* RepliesManager::Clone()
{
    // RVA 0x177EB0
    return new RepliesManager(*this);
}

m3d::Object* RepliesManager::CreateObject()
{
    // RVA 0x178860
    return new RepliesManager;
}

m3d::Class* RepliesManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

m3d::Class* RepliesManager::GetClass() const
{
    // RVA 0x178850
    return RT_CLASS_LOCAL(RepliesManager);
}

void RepliesManager::SetConditionValue(int value)
{
    // RVA 0x179650
    m_lastScriptConditionValue = value;
}

int RepliesManager::Init()
{
    // RVA 0x178A30
    M3D_APP->m_pInterfaceManager->LaunchEvent(101, GUI_EVENT_CUSTOM, nullptr);

    if (m_isInited)
    {
        Clear();
    }

    int res = LoadFromXml(M3D_APP->m_pInterfaceManager->GetPathToDialogsFileGlobal()) & 1;
    res &= LoadFromXml(M3D_APP->m_pInterfaceManager->GetPathToDynamicDialogsFileGlobal());

    m_isInited = true;
    if (res)
    {
        M3D_LOG_INFO("RepliesManager was inited successfully");
    }
    else
    {
        M3D_LOG_ERR("RepliesManager was inited with errors");
    }
    return 1;
}

void RepliesManager::Clear()
{
    // RVA 0x178B80
    for (auto const& kv : m_replies)
    {
        delete kv.second;
    }
    m_replies.clear();
    m_isInited = false;
}

int RepliesManager::LoadFromXml(CStr const& fileName)
{
    // RVA 0x178C30
    scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
    if (!stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
    {
        // A missing file is not fatal - the dynamic-dialogs file is optional.
        M3D_LOG_INFO("Can't open file " + fileName + " for read.");
        return 1;
    }

    ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
    if (!xmlFile->Read(*stream))
    {
        stream->Close();
        M3D_LOG_INFO("Error: cannot parse " + fileName + " (" + CStr(xmlFile->GetError()) + ") ");
        return 0;
    }
    stream->Close();

    ref_ptr rootNode = xmlFile->CreateNode();
    xmlFile->GetFirstChild(rootNode, "DialogsResource");
    if (rootNode->IsEmpty())
    {
        M3D_LOG_INFO("RepliesManager: file " + fileName + " is empty");
        return 1;
    }

    ref_ptr replyNode = xmlFile->CreateNode();
    for (rootNode->GetFirstChild(replyNode, "Reply"); !replyNode->IsEmpty();
         replyNode->GetNextSibling(replyNode, "Reply"))
    {
        auto* reply = new Reply();
        if (!reply->LoadFromXml(xmlFile, replyNode))
        {
            delete reply;
            continue;
        }

        if (m_replies.find(reply->GetName()) == m_replies.end())
        {
            m_replies[reply->GetName()] = reply;
        }
        else
        {
            M3D_LOG_INFO(
                "RepliesManager::LoadFromXml warning - duplicate reply names (" + reply->GetName() +
                "). Second reply with that name would be ignored.");
            delete reply;
        }
    }

    M3D_LOG_INFO("Replies were loaded successfully from file " + fileName);
    return 1;
}

Reply* RepliesManager::GetReplyByName(CStr const& name) const
{
    // RVA 0x179450
    auto it = m_replies.find(name);
    return it != m_replies.end() ? it->second : nullptr;
}

Reply* RepliesManager::GetFirstNpcReply(ai::Npc* forNpc)
{
    // RVA 0x179660
    if (!forNpc)
    {
        return nullptr;
    }

    std::vector<CStr> helloReplies = forNpc->GetHelloReplyNames();
    if (forNpc->GetNpcType() != ai::Npc::NPC_BARMAN)
    {
        return GetFirstFitReply(helloReplies, Reply::ROLE_NPC, forNpc);
    }

    if (helloReplies.empty())
    {
        helloReplies = GetAllHelloReplyNamesForDynamicNpc();
    }
    return GetRandomFitReply(helloReplies, Reply::ROLE_NPC, forNpc);
}

Reply* RepliesManager::GetNextNpcReply(Reply const* playerReply, ai::Npc* curNpc)
{
    // RVA 0x1794A0
    if (!playerReply || playerReply->GetRole() != Reply::ROLE_PLAYER)
    {
        return nullptr;
    }
    return GetFirstFitReply(playerReply->GetNextReplies(), Reply::ROLE_NPC, curNpc);
}

std::vector<Reply*> RepliesManager::GetNextPlayerReplies(Reply const* npcReply, ai::Npc* curNpc)
{
    // RVA 0x1794C0
    std::vector<Reply*> nextReplies;
    if (npcReply && npcReply->GetRole() == Reply::ROLE_NPC)
    {
        nextReplies = GetAllFitReplies(npcReply->GetNextReplies(), Reply::ROLE_PLAYER, curNpc);
    }
    return nextReplies;
}

Reply* RepliesManager::GetFirstFitReply(std::vector<CStr> const& possibleReplyNames, Reply::Role role, ai::Npc* curNpc)
{
    // RVA 0x179710
    for (CStr const& name : possibleReplyNames)
    {
        Reply* reply = GetReplyByName(name);
        if (!reply || reply->GetRole() != role)
        {
            continue;
        }

        m_lastScriptConditionValue = 1;
        if (!ExecuteScript(FixScriptCondition(reply->GetScriptCondition()), curNpc))
        {
            M3D_LOG_INFO(CStr("Fail to execute condition script for NPC reply: ") + reply->GetName());
        }
        if (m_lastScriptConditionValue)
        {
            return reply;
        }
    }
    return nullptr;
}

std::vector<Reply*>
    RepliesManager::GetAllFitReplies(std::vector<CStr> const& possibleReplyNames, Reply::Role role, ai::Npc* curNpc)
{
    // RVA 0x179900
    std::vector<Reply*> nextReplies;
    for (CStr const& name : possibleReplyNames)
    {
        Reply* reply = GetReplyByName(name);
        if (!reply || reply->GetRole() != role)
        {
            continue;
        }

        m_lastScriptConditionValue = 1;
        if (!ExecuteScript(FixScriptCondition(reply->GetScriptCondition()), curNpc))
        {
            M3D_LOG_INFO(CStr("Fail to execute condition script for NPC reply: ") + reply->GetName());
        }
        if (m_lastScriptConditionValue)
        {
            nextReplies.push_back(reply);
        }
    }
    return nextReplies;
}

Reply* RepliesManager::GetRandomFitReply(std::vector<CStr> const& possibleReplyNames, Reply::Role role, ai::Npc* curNpc)
{
    // RVA 0x179D40
    std::vector<Reply*> fitReplies = GetAllFitReplies(possibleReplyNames, role, curNpc);
    if (fitReplies.empty())
    {
        return nullptr;
    }
    return fitReplies[std::rand() % fitReplies.size()];
}

std::vector<CStr> RepliesManager::GetAllHelloReplyNamesForDynamicNpc()
{
    // RVA 0x179B30 - collect "Dynamic00", "Dynamic01", ... while replies exist.
    std::vector<CStr> replyNames;
    CStr const strDynamic = "Dynamic";
    for (int i = 0;; ++i)
    {
        CStr strNum;
        strNum.format("%02d", i);
        CStr const name = strDynamic + strNum;
        if (!GetReplyByName(name))
        {
            break;
        }
        replyNames.push_back(name);
    }
    return replyNames;
}

int RepliesManager::ExecuteScript(CStr const& script, ai::Npc* curNpc) const
{
    // RVA 0x179510
    if (script.empty())
    {
        return 1;
    }

    ai::Npc::SetCurrentNpc(curNpc);
    m3d::eScriptError const err = m3d::g_Kernel->GetScriptServer().execute(script.c_str(), "ConversationWnd");
    ai::Npc::SetCurrentNpc(nullptr);

    if (err == m3d::SUCCESS)
    {
        return 1;
    }

    M3D_LOG_INFO("RepliesManager: error execute script - " + GetFormattedScriptErrorDesc(err));
    return 0;
}

Reply* RepliesManager::GetPlayerRequestForDynamicQuest(ai::Npc* npc, ai::DynamicQuest const* dQuest)
{
    // RVA 0x179DB0
    if (!npc || !dQuest)
    {
        return nullptr;
    }

    Reply* firstNpcReply = GetFirstNpcReply(npc);
    if (!firstNpcReply)
    {
        return nullptr;
    }

    CStr const strType = DQuestType2Str(dQuest->GetQuestType());
    std::vector<CStr> replyNames;
    for (int i = 0;; ++i)
    {
        CStr strNum;
        strNum.format("%02d", i);
        CStr const name = firstNpcReply->GetName() + "_" + strType + strNum;
        if (!GetReplyByName(name))
        {
            break;
        }
        replyNames.push_back(name);
    }

    return GetRandomFitReply(replyNames, Reply::ROLE_PLAYER, npc);
}
