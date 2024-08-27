#pragma once
#include <core/stringm3d.h>

class PostEffectManager;
class PostEffectModel;

class PostEffect
{
public:
    PostEffect(PostEffectModel*, float, PostEffectManager*, int, CStr const&, bool);
    void DoFrame();
    bool FindByName(CStr);
    ~PostEffect();

private:
    void UnRegistedMe();
    bool CheckExitAnd();
    //std::mem_fun_t<void, PostEffect>::mem_fun_t<void, PostEffect>(void (*)());
    bool CheckExitOr();
    bool CheckExit();
    //std::mem_fun<void, PostEffect>(void (*)());
    void ApplyDynamic();

private:
    unsigned int m_startTime;
    PostEffectManager* m_parent;
    PostEffectModel* m_model;
    int m_sequenceNumb;
    CStr m_sequenceName;
};
