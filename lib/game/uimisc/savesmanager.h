#pragma once
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

    public:
        CStr m_saveFolderName = "saves";
        CStr m_infoFileName = "SaveInfo.xml";
        CStr m_quickSavePrefix = "quick_";
        CStr m_autoSavePrefix = "auto_";
        CStr m_quickSaveNameStrId = "QuickSave";
        CStr m_autoSaveNameStrId = "AutoSave";
        CStr m_screenshotFileName = "screenshot.tga";
        CStr m_mapsDirName = "maps";
        PointBase<int> m_screenshotSz{256, 128};
        int m_maxQuickSavesNum = 5;
        int m_maxAutoSavesNum = 1;
    };

    struct SaveInfo
    {
        SaveInfo();

        /* 0x0000 */ CStr m_folderName;
        /* 0x000c */ CStr m_saveName;
        /* 0x0018 */ m3d::AIParam m_gameTime;
        /* 0x0034 */ CStr m_levelName;
        /* 0x0040 */ _FILETIME m_saveModifyLocalTime;
    }; /* size: 0x0048 */

public:
    virtual ~SavesManager();
    int QuickLoad();
    CStr GetNewSaveDefaultName() const ;
    int MakeCurGameScreenshot();
    virtual m3d::Object * Clone();
    CStr GetPathForTemporaryMaps() const ;
    CStr GetSaveFolderPathByFolderName(CStr const &) const ;
    int AutoSave(CStr const &);
    static m3d::Class * GetBaseClass();
    m3d::rend::TexHandle GetCurGameScreenshot() const ;
    CStr GetNewSaveFolderName() const ;
    static m3d::Object * CreateObject();
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
    int GetSaveFolderNames(retruxx::vector<CStr> &) const ;
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
    CStr GetFirstUsedSaveFolderName(retruxx::vector<CStr> const &) const ;
    void GetSaveFoldersByPattern(CStr const &,retruxx::vector<CStr> &) const ;
    CStr GetLastUsedSaveFolderName(retruxx::vector<CStr> const &) const ;
    void OnNewFrameForce();
    CStr GetAutoSaveName(CStr const &,CStr const &) const ;
    CStr GetQuickSaveName(CStr const &) const ;
    int AddSaveInfo(SaveInfo *);
    struct _FILETIME GetSaveFileModifyTime(CStr const &) const ;
    int SaveInfos(CStr const &,CStr const &);

public:
    RT_CLASS_DECLARE(SavesManager);

private:
    ConstantSaveInfo m_constantSaveInfo;
    retruxx::map<CStr,SaveInfo *> m_saves;
    m3d::rend::TexHandle m_curGameScreenshot;
    bool m_bDelayedQuickSave = false;
    unsigned int m_delayedQuickSaveFrame = 0;
};
