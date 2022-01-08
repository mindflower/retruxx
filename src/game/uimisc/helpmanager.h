#pragma once

class HelpManager :  public m3d::Object
{
public:
    std::pair<enum HelpId const ,int>::pair<enum HelpId const ,int><enum HelpId,int>(struct std::pair<enum HelpId,int> const &);
    CStr GetHelpMsgByHelpId(CStr const &) const ;
    static CStr __fastcall Enumered2StrHelpId(enum HelpId);
    void ShowHelp(CStr const &,bool);
    std::allocator<enum HelpId>::allocator<enum HelpId>(class std::allocator<enum HelpId> const &);
    std::allocator<enum HelpId>::allocator<enum HelpId>();
    std::pair<enum HelpId,int>::pair<enum HelpId,int>(enum HelpId const &,int const &);
    static class m3d::Object * CreateObject();
    static enum HelpId __fastcall Str2EnumeredHelpId(CStr const &);
    void Clear();
    virtual struct m3d::Class * GetClass() const ;
    void ResetAutoHelp();
    static struct m3d::Class * GetBaseClass();
    void EnableAutoHelp(bool);
    virtual class m3d::Object * Clone();
    CStr GetHelpTitleByHelpId(CStr const &) const ;
    virtual ~HelpManager();
    bool IsAutoHelpEnabled() const ;
    int GameDataUpdate(void *,int);
protected:
    HelpManager();
    HelpManager(class HelpManager const &);
    void ShowHelpEnumered(enum HelpId,bool);
    void HideCurrentHelpWindow();
    void MarkHelpAsShown(enum HelpId);
    enum ProfileParam GetProfileParamIdByHelpId(enum HelpId) const ;
    int GetPredefinedWndGuiIdByHelpId(enum HelpId) const ;
    int GetCurHelpWndGuiId() const ;
    void GetAllHelpWndGuiIds(class std::vector<int,class std::allocator<int> > &) const ;
    void OnHelpImpulse();
    enum HelpId CalcHelpIdForCurrentInterfaceState() const ;
    bool IsHelpIdValidForCurrentInterfaceState(enum HelpId) const ;
    bool WasHelpShown(enum HelpId) const ;
    void OnHelpEvent(void *);
private:
    enum HelpId * __fastcall std::_Copy_backward_opt<enum HelpId *,enum HelpId *>(enum HelpId *,enum HelpId *,enum HelpId *,struct std::_Nonscalar_ptr_iterator_tag);
    void __fastcall std::_Construct<enum HelpId,enum HelpId>(enum HelpId *,enum HelpId const &);
    void __fastcall std::fill<enum HelpId *,enum HelpId>(enum HelpId *,enum HelpId *,enum HelpId const &);
    struct std::_Nonscalar_ptr_iterator_tag __fastcall std::_Ptr_cat<enum HelpId *,enum HelpId *>(enum HelpId * &,enum HelpId * &);
    void __fastcall std::_Destroy<enum HelpId>(enum HelpId *);
    enum HelpId * __fastcall std::_Allocate<enum HelpId>(unsigned int,enum HelpId *);
    enum HelpId * __fastcall std::copy_backward<enum HelpId *,enum HelpId *>(enum HelpId *,enum HelpId *,enum HelpId *);
    std::map<enum HelpManager::HelpId,int> m_predefinedHelpWindows;
    HelpManager::AuxInfo m_aif;
};
