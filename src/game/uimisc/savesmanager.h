#pragma once

class SavesManager :  public m3d::Object
{
public:
    virtual ~SavesManager();
    int QuickLoad();
    class CStr GetNewSaveDefaultName() const ;
    int MakeCurGameScreenshot();
    virtual class m3d::Object * Clone();
    class CStr GetPathForTemporaryMaps() const ;
    class CStr GetSaveFolderPathByFolderName(class CStr const &) const ;
    int AutoSave(class CStr const &);
    static struct m3d::Class * __fastcall GetBaseClass();
    class m3d::rend::TexHandle GetCurGameScreenshot() const ;
    class CStr GetNewSaveFolderName() const ;
    static class m3d::Object * __fastcall CreateObject();
    int LastLoad();
    int LoadGame(class CStr const &);
    int GameDataUpdate(void *,int);
    int QuickSave();
    struct SaveInfo const * GetSaveInfoByFolderName(class CStr const &) const ;
    int Init();
    struct ConstantSaveInfo const & GetConstatntSaveInfo() const ;
    int SaveGame(class CStr const &,class CStr const &,bool);
    void Clear();
    virtual struct m3d::Class * GetClass() const ;
    int GetSaveFolderNames(class std::vector<class CStr,class std::allocator<class CStr> > &) const ;
    int DeleteSaveGame(class CStr const &);
protected:
    int LoadInfos();
    SavesManager();
    SavesManager(class SavesManager const &);
    void CheckAndHandleDelayedQuickSave(bool);
    class CStr GetNewAutoSaveFolderName() const ;
    class CStr GetNewQuickSaveFolderName() const ;
    int SaveScreenshot(class CStr const &);
    void OnCurProfileChanged();
    class CStr GetFirstUsedSaveFolderName(class std::vector<class CStr,class std::allocator<class CStr> > const &) const ;
    void GetSaveFoldersByPattern(class CStr const &,class std::vector<class CStr,class std::allocator<class CStr> > &) const ;
    class CStr GetLastUsedSaveFolderName(class std::vector<class CStr,class std::allocator<class CStr> > const &) const ;
    void OnNewFrameForce();
    class CStr GetAutoSaveName(class CStr const &,class CStr const &) const ;
    class CStr GetQuickSaveName(class CStr const &) const ;
    int AddSaveInfo(struct SaveInfo *);
    struct _FILETIME GetSaveFileModifyTime(class CStr const &) const ;
    int SaveInfos(class CStr const &,class CStr const &);
private:
    std::pair<CStr,SaveInfo *>::pair<CStr,SaveInfo *>(CStr const &,SaveInfo * const &);
    SavesManager::ConstantSaveInfo m_constantSaveInfo;
    std::map<CStr,SavesManager::SaveInfo *> m_saves;
    m3d::rend::TexHandle m_curGameScreenshot;
    bool m_bDelayedQuickSave;
    unsigned int m_delayedQuickSaveFrame;
};
