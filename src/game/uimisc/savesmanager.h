#pragma once
#include <map>
#include <core/aiparam.h>
#include <core/clazz.h>
#include <math/point2d.h>
#include <renderer/i_renderer.h>

class SavesManager :  public m3d::Object
{
public:
    class ConstantSaveInfo
    {
    public:
        ConstantSaveInfo();

    private:
        CStr m_saveFolderName;
        CStr m_infoFileName;
        CStr m_quickSavePrefix;
        CStr m_autoSavePrefix;
        CStr m_quickSaveNameStrId;
        CStr m_autoSaveNameStrId;
        CStr m_screenshotFileName;
        CStr m_mapsDirName;
        PointBase<int> m_screenshotSz;
        int m_maxQuickSavesNum;
        int m_maxAutoSavesNum;
    };

    class SaveInfo
    {
    public:
        SaveInfo();

    private:
        CStr m_folderName;
        CStr m_saveName;
        m3d::AIParam m_gameTime;
        CStr m_levelName;
        _FILETIME m_saveModifyLocalTime;
    };

public:
    virtual ~SavesManager();
    int QuickLoad();
    CStr GetNewSaveDefaultName() const ;
    int MakeCurGameScreenshot();
    virtual m3d::Object * Clone();
    CStr GetPathForTemporaryMaps() const ;
    CStr GetSaveFolderPathByFolderName(CStr const &) const ;
    int AutoSave(CStr const &);
    static m3d::Class * __fastcall GetBaseClass();
    m3d::rend::TexHandle GetCurGameScreenshot() const ;
    CStr GetNewSaveFolderName() const ;
    static m3d::Object * __fastcall CreateObject();
    int LastLoad();
    int LoadGame(CStr const &);
    int GameDataUpdate(void *,int);
    int QuickSave();
    SaveInfo const * GetSaveInfoByFolderName(CStr const &) const ;
    int Init();
    ConstantSaveInfo const & GetConstatntSaveInfo() const ;
    int SaveGame(CStr const &,CStr const &,bool);
    void Clear();
    virtual m3d::Class * GetClass() const ;
    int GetSaveFolderNames(std::vector<CStr> &) const ;
    int DeleteSaveGame(CStr const &);

protected:
    int LoadInfos();
    SavesManager();
    SavesManager(SavesManager const &);
    void CheckAndHandleDelayedQuickSave(bool);
    CStr GetNewAutoSaveFolderName() const ;
    CStr GetNewQuickSaveFolderName() const ;
    int SaveScreenshot(CStr const &);
    void OnCurProfileChanged();
    CStr GetFirstUsedSaveFolderName(std::vector<CStr> const &) const ;
    void GetSaveFoldersByPattern(CStr const &,std::vector<CStr> &) const ;
    CStr GetLastUsedSaveFolderName(std::vector<CStr> const &) const ;
    void OnNewFrameForce();
    CStr GetAutoSaveName(CStr const &,CStr const &) const ;
    CStr GetQuickSaveName(CStr const &) const ;
    int AddSaveInfo(SaveInfo *);
    struct _FILETIME GetSaveFileModifyTime(CStr const &) const ;
    int SaveInfos(CStr const &,CStr const &);

private:
    ConstantSaveInfo m_constantSaveInfo;
    std::map<CStr,SaveInfo *> m_saves;
    m3d::rend::TexHandle m_curGameScreenshot;
    bool m_bDelayedQuickSave;
    unsigned int m_delayedQuickSaveFrame;
};
