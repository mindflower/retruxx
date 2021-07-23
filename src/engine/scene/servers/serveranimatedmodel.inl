#pragma once

class DynamicModel
{
public:
    DynamicModel();
    void _releaseImpostorShit();
    void _createImpostorShit();
    ~DynamicModel();
protected:
private:
    std::_Destroy<auxEffectDesc>(auxEffectDesc *);
    std::_Copy_backward_opt<auxEffectDesc *,auxEffectDesc *>(auxEffectDesc *,auxEffectDesc *,auxEffectDesc *,std::_Nonscalar_ptr_iterator_tag);
    std::copy<auxEffectDesc *,auxEffectDesc *>(auxEffectDesc *,auxEffectDesc *,auxEffectDesc *);
    std::allocator<auxEffectDesc>::allocator<auxEffectDesc>();
    std::allocator<auxEffectDesc>::allocator<auxEffectDesc>();
    std::copy_backward<auxEffectDesc *,auxEffectDesc *>(auxEffectDesc *,auxEffectDesc *,auxEffectDesc *);
    std::_Allocate<auxEffectDesc>(uint,auxEffectDesc *);
    std::_Construct<auxEffectDesc,auxEffectDesc>(auxEffectDesc *,auxEffectDesc const &);
    std::fill<auxEffectDesc *,auxEffectDesc>(auxEffectDesc *,auxEffectDesc *,auxEffectDesc const &);
    std::_Ptr_cat<auxEffectDesc *,auxEffectDesc *>(auxEffectDesc * &,auxEffectDesc * &);
    std::_Copy_opt<auxEffectDesc *,auxEffectDesc *>(auxEffectDesc *,auxEffectDesc *,auxEffectDesc *,std::_Nonscalar_ptr_iterator_tag);
    CStr m_soundIds[32];
    int m_soundsLooped[32];
    m3d::AnimatedModel *m_mdl[5];
    unsigned __int8 m_numLods;
    unsigned __int8 m_curLod;
    bool m_useImpostors;
    m3d::rend::TexHandle m_impostorTex;
    m3d::rend::VbHandle m_impostorVb;
    m3d::rend::IbHandle m_impostorIb;
    unsigned __int16 m_numVerts;
    unsigned __int16 m_numTris;
    unsigned __int16 m_numIndices;
    float m_impostorDisplacement;
    DynamicModel::auxActionEffectsDesc m_effects[32];
};

class ModelEffectList
{
public:
    ModelEffectList(struct DynamicModel *);
    void adjustModelEffects(class m3d::SgNode *,class std::vector<enum ActionType,class std::allocator<enum ActionType> > const &);
    void adjustModelEffects(class m3d::SgNode *,enum ActionType);
    ~ModelEffectList();
protected:
private:
    std::_Ptr_cat<tEffect *,tEffect *>(tEffect * &,tEffect * &);
    std::_Advance<tEffect *,int>(tEffect * &,int,std::random_access_iterator_tag);
    std::_Iter_cat<tEffect *>(tEffect * const &);
    std::advance<tEffect *,int>(tEffect * &,int);
    std::get_temporary_buffer<tEffect>(int);
    std::_Copy_opt<tEffect *,tEffect *>(tEffect *,tEffect *,tEffect *,std::_Nonscalar_ptr_iterator_tag);
    std::fill<tEffect *,tEffect>(tEffect *,tEffect *,tEffect const &);
    std::const_mem_fun_ref_t<bool,tEffect>::const_mem_fun_ref_t<bool,tEffect>(bool (tEffect::*)());
    std::pair<tEffect *,int>::pair<tEffect *,int>(tEffect * const &,int const &);
    std::_Destroy<tEffect>(tEffect *);
    std::mem_fun_ref<bool,tEffect>(bool (tEffect::*)());
    std::_Allocate<tEffect>(uint,tEffect *);
    std::_Temp_iterator<tEffect>::_Temp_iterator<tEffect>(std::_Temp_iterator<tEffect> const &);
    std::_Temp_iterator<tEffect>::_Temp_iterator<tEffect>(std::_Temp_iterator<tEffect> const &);
    std::return_temporary_buffer<tEffect>(tEffect *);
    std::allocator<tEffect>::allocator<tEffect>();
    std::allocator<tEffect>::allocator<tEffect>();
    std::copy_backward<tEffect *,tEffect *>(tEffect *,tEffect *,tEffect *);
    std::_Temp_iterator<tEffect>::~_Temp_iterator<tEffect>();
    std::copy<tEffect *,tEffect *>(tEffect *,tEffect *,tEffect *);
    void adjustModelEffects(class m3d::SgNode *,class std::vector<struct tEffect,class std::allocator<struct tEffect> > &);
    std::_Construct<tEffect,tEffect>(tEffect *,tEffect const &);
    std::_Copy_backward_opt<tEffect *,tEffect *>(tEffect *,tEffect *,tEffect *,std::_Nonscalar_ptr_iterator_tag);
    std::swap<tEffect>(tEffect &,tEffect &);
    std::vector<ModelEffectList::tEffect> m_curEffectList;
    DynamicModel *m_dynModel;
};

namespace ModelEffectList
{
    struct tEffect
    {
        DynamicModel::auxEffectDesc *m_desc;
        m3d::SgNode *m_effectNode;
    public:
        bool IsValid() const ;
    protected:
    private:
        std::const_mem_fun_ref_t<bool,tEffect>::const_mem_fun_ref_t<bool,tEffect>(bool (*)());
        std::mem_fun_ref<bool,tEffect>(bool (*)());
    };
}

namespace DynamicModel
{
    class auxActionEffectsDesc
    {
    public:
        auxActionEffectsDesc();
    protected:
    private:
        std::vector<auxEffectDesc> lpEffects;
        int startAttackFrame;
        int endAttackFrame;
        int skinNum;
        int cfgNum;
    };
}

namespace ModelEffectList
{
    class SortPred
    {
    public:
        bool operator()(tEffect const &,tEffect const &);
    protected:
    private:
    };
}
