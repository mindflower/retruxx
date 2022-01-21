#pragma once
#include <map>
#include <vector>
#include <core/clazz.h>

namespace ai
{
    class DynamicQuest;
    class Npc;
}

class Reply
{
public:
    enum Role
    {
        ROLE_PLAYER = 0x0,
        ROLE_NPC = 0x1,
        ROLE_INVALID = 0x2,
    };
public:
    int LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    CStr const& GetScriptResult() const;
    Role GetRole() const;
    Reply();
    std::vector<CStr> const& GetNextReplies() const;
    void SetScriptResult(CStr const&);
    void SetRole(Role);
    void SetScriptCondition(CStr const&);
    CStr const& GetText() const;
    void SetName(CStr const&);
    CStr const& GetScriptCondition() const;
    ~Reply();
    void SetNextDialogs(std::vector<CStr> const&);
    void SetText(CStr);
    CStr const& GetName() const;

private:
    CStr m_name;
    CStr m_text;
    Role m_role;
    CStr m_scriptCondition;
    CStr m_scriptResult;
    std::vector<CStr> m_nextReplies;
};

class RepliesManager :  public m3d::Object
{
public:
    void SetConditionValue(int);
    int Init();
    Reply * GetFirstFitReply(std::vector<CStr> const &, Reply::Role,ai::Npc *);
    virtual m3d::Class * GetClass() const ;
    virtual ~RepliesManager();
    std::vector<Reply *> GetNextPlayerReplies(Reply const *,ai::Npc *);
    std::vector<CStr> GetAllHelloReplyNamesForDynamicNpc();
    void Clear();
    Reply * GetPlayerRequestForDynamicQuest(ai::Npc *,ai::DynamicQuest const *);
    int ExecuteScript(CStr const &,ai::Npc *) const ;
    int LoadFromXml(CStr const &);
    Reply * GetReplyByName(CStr const &) const ;
    virtual m3d::Object * Clone();
    std::vector<Reply *> GetAllFitReplies(std::vector<CStr> const &,Reply::Role,ai::Npc *);
    static m3d::Object * CreateObject();
    Reply * GetRandomFitReply(std::vector<CStr,std::allocator<CStr> > const &,Reply::Role,ai::Npc *);
    Reply * GetNextNpcReply(Reply const *,ai::Npc *);
    static m3d::Class * GetBaseClass();
    Reply * GetFirstNpcReply(ai::Npc *);

protected:
    RepliesManager(RepliesManager const &);
    RepliesManager();

public:
    RT_CLASS_DECLARE(RepliesManager);

private:
    std::map<CStr,Reply *> m_replies;
    bool m_isInited;
    int m_lastScriptConditionValue;
};
