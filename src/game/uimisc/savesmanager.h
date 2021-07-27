#pragma once

class SavesManager :  public m3d::Object
{
public:
    virtual ~SavesManager();
    int QuickLoad();
    CStr GetNewSaveDefaultName() const ;
    int MakeCurGameScreenshot();
    virtual class m3d::Object * Clone();
    CStr GetPathForTemporaryMaps() const ;
    CStr GetSaveFolderPathByFolderName(CStr const &) const ;
    int AutoSave(CStr const &);
    static struct m3d::Class * __fastcall GetBaseClass();
    class m3d::rend::TexHandle GetCurGameScreenshot() const ;
    CStr GetNewSaveFolderName() const ;
    static class m3d::Object * __fastcall CreateObject();
    int LastLoad();
    int LoadGame(CStr const &);
    int GameDataUpdate(void *,int);
    int QuickSave();
    struct SaveInfo const * GetSaveInfoByFolderName(CStr const &) const ;
    int Init();
    struct ConstantSaveInfo const & GetConstatntSaveInfo() const ;
    int SaveGame(CStr const &,CStr const &,bool);
    void Clear();
    virtual struct m3d::Class * GetClass() const ;
    int GetSaveFolderNames(class std::vector<CStr,class std::allocator<CStr> > &) const ;
    int DeleteSaveGame(CStr const &);
protected:
    int LoadInfos();
    SavesManager();
    SavesManager(class SavesManager const &);
    void CheckAndHandleDelayedQuickSave(bool);
    CStr GetNewAutoSaveFolderName() const ;
    CStr GetNewQuickSaveFolderName() const ;
    int SaveScreenshot(CStr const &);
    void OnCurProfileChanged();
    CStr GetFirstUsedSaveFolderName(class std::vector<CStr,class std::allocator<CStr> > const &) const ;
    void GetSaveFoldersByPattern(CStr const &,class std::vector<CStr,class std::allocator<CStr> > &) const ;
    CStr GetLastUsedSaveFolderName(class std::vector<CStr,class std::allocator<CStr> > const &) const ;
    void OnNewFrameForce();
    CStr GetAutoSaveName(CStr const &,CStr const &) const ;
    CStr GetQuickSaveName(CStr const &) const ;
    int AddSaveInfo(struct SaveInfo *);
    struct _FILETIME GetSaveFileModifyTime(CStr const &) const ;
    int SaveInfos(CStr const &,CStr const &);
private:
    std::pair<CStr,SaveInfo *>::pair<CStr,SaveInfo *>(CStr const &,SaveInfo * const &);
    SavesManager::ConstantSaveInfo m_constantSaveInfo;
    std::map<CStr,SavesManager::SaveInfo *> m_saves;
    m3d::rend::TexHandle m_curGameScreenshot;
    bool m_bDelayedQuickSave;
    unsigned int m_delayedQuickSaveFrame;
};
