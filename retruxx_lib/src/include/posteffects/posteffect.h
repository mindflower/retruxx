#pragma once
#include <core/stringm3d.h>

class PostEffectManager;
class PostEffectModel;

class PostEffect
{
public:
    PostEffect(PostEffectModel* model, float effParam, PostEffectManager* parent, int nSeq, const CStr& seqName, bool isBeKilling);
    void DoFrame();
    bool FindByName(const CStr name);

private:
    void ApplyDynamic();
    bool CheckExit();
    bool CheckExitAnd();
    bool CheckExitOr();
    void UnRegistedMe();
    /* 0x0000 */ unsigned int m_startTime;
    /* 0x0004 */ PostEffectManager* m_parent;
    /* 0x0008 */ PostEffectModel* m_model;
    /* 0x000c */ int m_sequenceNumb;
    /* 0x0010 */ CStr m_sequenceName;
}; /* size: 0x001c */
