#pragma once
#include <core/stringm3d.h>

class PostEffectManager;
class PostEffectModel;

class PostEffect
{
public:
    PostEffect(
        PostEffectModel* model,
        float effParam,
        PostEffectManager* parent,
        int nSeq,
        CStr const& seqName,
        bool isBeKilling);
    void DoFrame();
    bool FindByName(CStr const name);

private:
    void ApplyDynamic();
    bool CheckExit();
    bool CheckExitAnd();
    bool CheckExitOr();
    void UnRegistedMe();
    unsigned int m_startTime;
    PostEffectManager* m_parent;
    PostEffectModel* m_model;
    int m_sequenceNumb;
    CStr m_sequenceName;
};
