#pragma once

class CreditsWnd :  public m3d::ui::ModalWnd
{
public:
    virtual ~CreditsWnd();
    virtual struct m3d::Class * GetClass() const ;
    virtual class m3d::Object * Clone();
    static struct m3d::Class * GetBaseClass();
    static class m3d::Object * CreateObject();
protected:
    virtual int OnAfterRemoveFromWndStation();
    void StartShowingPages();
    void UpdateStartTime();
    CreditsWnd();
    CreditsWnd(class CreditsWnd const &);
    void UpdateBg();
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    void UpdatePortrait();
    void Exit();
    void DoShowPage();
    void PlaySwitchSound();
    void DoBreak();
    void StopSwitchSound();
    void CheckAndShowCurrentPage();
    void UpdateText();
    bool IsCurPageIdValid() const ;
    int PlaySoundA(int,bool);
    bool IsTimeEnded() const ;
    void StopSound(int);
    void LoadPageInfo();
    virtual int OnBeforeAddToWndStation();
    void StopAllSoundsAndMusic();
    void UpdateControls();
    void ClearPages();
    void UpdatePointer();
    void PlayMusic();
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    virtual int GameDataUpdate(void *,int);
    void StopMusic();
    void Reset();
    void UpdatePortraitAnimation();
    void OnNewFrame();
    virtual int GameDataSetup();
    bool NeedExit() const ;
    void CheckAndShowNextPage();
    void PlayAmbientSound();
    void StopAmbientSound();
private:
    std::_Copy_backward_opt<PageInfo * *,PageInfo * *>(PageInfo * *,PageInfo * *,PageInfo * *,std::_Scalar_ptr_iterator_tag);
    std::_Ptr_cat<PageInfo>(PageInfo * *,PageInfo * *);
    std::copy_backward<PageInfo * *,PageInfo * *>(PageInfo * *,PageInfo * *,PageInfo * *);
    std::fill<PageInfo * *,PageInfo *>(PageInfo * *,PageInfo * *,PageInfo * const &);
    std::_Allocate<PageInfo *>(uint,PageInfo * *);
    std::_Uninit_copy<PageInfo *,PageInfo *>(PageInfo * *,PageInfo * *,PageInfo * *,std::allocator<PageInfo *> &,std::_Scalar_ptr_iterator_tag);
    std::allocator<PageInfo *>::allocator<PageInfo *>();
    std::allocator<PageInfo *>::allocator<PageInfo *>();
    m3d::ui::Wnd *m_wndText;
    ItemModelWnd *m_wndPortrait;
    m3d::rend::TexHandle m_texBgShow;
    m3d::rend::TexHandle m_texBgBreak;
    float m_breakTime;
    int m_showSoundTableId;
    int m_breakSoundTableId;
    int m_ambientSoundTableId;
    int m_musicTableId;
    int m_switchSoundChannelId;
    int m_ambientSoundChannelId;
    int m_musicChannelId;
    std::vector<CreditsWnd::PageInfo *> m_pageInfos;
    int m_curPageId;
    bool m_bInBreak;
    CreditsWnd::Pointer m_pointer;
    float m_startTime;
    m3d::CVar m_cvPathToPageInfo;
    CreditsWnd::AuxInfo m_aif;
};
