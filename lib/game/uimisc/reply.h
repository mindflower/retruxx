#pragma once
#include <map>
#include <vector>
#include <core/clazz.h>

namespace ai
{
    class DynamicQuest;
    class Npc;
}  // namespace ai

class Reply
{
public:
    enum Role
    {
        ROLE_PLAYER = 0,
        ROLE_NPC = 1,
        ROLE_INVALID = 2,
    };

    using NamesVector = std::vector<CStr, std::allocator<CStr>>;

public:
    Reply(Reply const&);
    Reply();
    ~Reply();
    void SetName(CStr const& name);
    CStr const& GetName() const;
    void SetText(CStr const text);
    CStr const& GetText() const;
    void SetRole(Reply::Role role);
    Reply::Role GetRole() const;
    void SetScriptCondition(CStr const& script);
    CStr const& GetScriptCondition() const;
    void SetScriptResult(CStr const& script);
    CStr const& GetScriptResult() const;
    void SetNextDialogs(std::vector<CStr, std::allocator<CStr>> const& nextReplies);
    std::vector<CStr, std::allocator<CStr>> const& GetNextReplies() const;
    int LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);

private:
    CStr m_name;
    CStr m_text;
    Reply::Role m_role;
    CStr m_scriptCondition;
    CStr m_scriptResult;
    std::vector<CStr, std::allocator<CStr>> m_nextReplies;
};

class RepliesManager : public m3d::Object
{
public:
    using RepliesVector = std::vector<Reply*, std::allocator<Reply*>>;

public:
    int Init();
    void Clear();
    Reply* GetReplyByName(CStr const& name) const;
    Reply* GetFirstNpcReply(ai::Npc* forNpc);
    Reply* GetNextNpcReply(Reply const* playerReply, ai::Npc* curNpc);
    std::vector<Reply*, std::allocator<Reply*>> GetNextPlayerReplies(Reply const* npcReply, ai::Npc* curNpc);
    Reply* GetPlayerRequestForDynamicQuest(ai::Npc* npc, ai::DynamicQuest const* dQuest);
    int ExecuteScript(CStr const& script, ai::Npc* curNpc) const;
    void SetConditionValue(int value);

protected:
    RepliesManager();
    RepliesManager(RepliesManager const& rhs);

public:
    virtual ~RepliesManager() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classRepliesManager;

    using ReplyMap = std::map<CStr, Reply*, std::less<CStr>, std::allocator<std::pair<CStr const, Reply*>>>;
    using ReplyPair = std::pair<CStr, Reply*>;

public:
    int LoadFromXml(CStr const& fileName);
    Reply* GetFirstFitReply(
        std::vector<CStr, std::allocator<CStr>> const& possibleReplyNames,
        Reply::Role role,
        ai::Npc* curNpc);
    std::vector<Reply*, std::allocator<Reply*>> GetAllFitReplies(
        std::vector<CStr, std::allocator<CStr>> const& possibleReplyNames,
        Reply::Role role,
        ai::Npc* curNpc);
    Reply* GetRandomFitReply(
        std::vector<CStr, std::allocator<CStr>> const& possibleReplyNames,
        Reply::Role role,
        ai::Npc* curNpc);
    std::vector<CStr, std::allocator<CStr>> GetAllHelloReplyNamesForDynamicNpc();
    std::map<CStr, Reply*, std::less<CStr>, std::allocator<std::pair<CStr const, Reply*>>> m_replies;
    bool m_isInited = false;
    char Padding_203[3];
    int m_lastScriptConditionValue;
};
