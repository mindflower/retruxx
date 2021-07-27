#pragma once

class RepliesManager :  public m3d::Object
{
public:
    void SetConditionValue(int);
    int Init();
    class Reply * GetFirstFitReply(class std::vector<CStr,class std::allocator<CStr> > const &,enum Reply::Role,class ai::Npc *);
    virtual struct m3d::Class * GetClass() const ;
    virtual ~RepliesManager();
    class std::vector<class Reply *,class std::allocator<class Reply *> > GetNextPlayerReplies(class Reply const *,class ai::Npc *);
    class std::vector<CStr,class std::allocator<CStr> > GetAllHelloReplyNamesForDynamicNpc();
    void Clear();
    class Reply * GetPlayerRequestForDynamicQuest(class ai::Npc *,class ai::DynamicQuest const *);
    int ExecuteScript(CStr const &,class ai::Npc *) const ;
    int LoadFromXml(CStr const &);
    class Reply * GetReplyByName(CStr const &) const ;
    virtual class m3d::Object * Clone();
    class std::vector<class Reply *,class std::allocator<class Reply *> > GetAllFitReplies(class std::vector<CStr,class std::allocator<CStr> > const &,enum Reply::Role,class ai::Npc *);
    static class m3d::Object * __fastcall CreateObject();
    class Reply * GetRandomFitReply(class std::vector<CStr,class std::allocator<CStr> > const &,enum Reply::Role,class ai::Npc *);
    class Reply * GetNextNpcReply(class Reply const *,class ai::Npc *);
    static struct m3d::Class * __fastcall GetBaseClass();
    class Reply * GetFirstNpcReply(class ai::Npc *);
protected:
    RepliesManager(class RepliesManager const &);
    RepliesManager();
private:
    std::map<CStr,Reply *> m_replies;
    bool m_isInited;
    int m_lastScriptConditionValue;
};
