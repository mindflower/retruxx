#pragma once
#include "uimanager/truxxuimanager.h"
#include <m3dapp.h>
#include <core/console/cvar.h>
#include <server/objects/base/objcontainer.h>
#include <ui/msgbox.h>

class PostEffectManager;
class ProfileManager;

namespace ai {
    class PhysicObj;
}

namespace m3d
{
    class RadioEngine;
    class BlockMusicManager;
    class TownMusicManager;
    class AuxImpulseInfo;
}

enum HackedMusicType
{
    HACKMUSIC_MENU = 0x0,
    HACKMUSIC_GAME = 0x1,
    HACKMUSIC_BAR = 0x2,
    HACKMUSIC_CUSTOM = 0x3,
    HACKMUSIC_LAST = 0x4,
};

class CMiracle3d :  public m3d::Application
{
public:
    class CurGameMode
    {
    public:
        GameState Get() const;
        void Set(GameState);

    private:
        GameState m_mode = GS_GAME;

    };
    class Player
    {
    public:
        void LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        void SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;

    public:
        CameraModes m_cameraMode = CM_FOLLOWMODE;
        CVector m_gameLookAt;
        Quaternion m_lastobjQuat{0.0, 0.0, 0.0, 1.0};
        int m_elapsedtime = 0;
        float m_desiredDistance;
    };

public:
    virtual int OnChangeMode(m3d::AuxImpulseInfo const &);
    void SkipCinematicMessage();
    int OnGameDrag(m3d::AuxImpulseInfo const &);
    float GetMinTimeScale() const ;
    int OnFinishVideoPlaying();
    void SetMinTimeScale(float);
    bool LoadSavedGame(CStr const &);
    virtual bool GetMouseHitPoint(CVector &,m3d::SgNode * &);
    int OnFinishIntroVideoPlaying();
    int GameDone();
    int OnFlyMouse(m3d::AuxImpulseInfo const &);
    int GameInit();
    virtual bool GetCursorShow() const ;
    virtual bool GetCursorShow0() const ;
    virtual void SetCursorShow(bool);
    int CinematicInit();
    void PlayHackedMusic(HackedMusicType,bool);
    bool CinematicFade();
    static m3d::Class* GetBaseClass();
    CMiracle3d();
    m3d::TownMusicManager * GetTownMusicManager();
    virtual bool AddPostEffect(CStr const &,float);
    virtual int HandleCinematic(float);
    static m3d::Object * CreateObject();
    void OnChangeProfile();
    void CleanMainMenuLevel();
    void BeginModalDlg(bool);
    virtual m3d::ui::MbRetCodes RunMsgBoxDlg(CStr const &,CStr const &,unsigned int,bool);
    int CleanLevel(bool,bool);
    int OnGameZoom(m3d::AuxImpulseInfo const &);
    virtual void PutSplash(int,char const *);
    virtual bool bIsMousePointing() const ;
    void ClearSomeGameElementsBeforeModal();
    virtual void ChangeLanguage();
    virtual float getZoom();
    int CinematicClear();
    virtual void OnBeforeDeviceReset();
    void CinematicInterrupt();
    virtual void setZoom(float);
    virtual void OnAfterDeviceReset();
    int OnSkipCinematic(m3d::AuxImpulseInfo const &);
    void RenderAsBackground(bool);
    void UpdateCameraPosition(ai::PhysicObj *);
    float GetMaxTimeScale() const ;
    void SetMaxTimeScale(float);
    virtual int OnObtainingFocus();
    int LoadLevel(CStr const &,CStr const &,bool,bool,bool,m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *,ai::ObjContainer::eSAVE_TYPES);
    void FullSystyemAndUserUnpause();
    virtual bool GetPostEffectParam(CStr const &,float &);
    virtual bool SetPostEffectParam(CStr const &,float);
    void StartMainMenu();
    virtual void SetMouseSensitivity(float);
    void UpdateCinematicCameraRotation();
    void SetCurHackedMusicType(HackedMusicType);
    HackedMusicType GetCurHackedMusicType() const ;
    int ValidateCameraOrigin(bool);
    virtual void LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
    virtual void Pause();
    int OnGameSwitchCamera(m3d::AuxImpulseInfo const &);
    int OnGameMouse(m3d::AuxImpulseInfo const &);
    virtual void SaveToXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
    void EndModalDlg();
    bool CanLaunchIfaceWindow();
    virtual void UnPause();
    virtual int GetCurDifficultyLevel() const ;
    int LoadMainMenuLevel();
    int OnDebug(m3d::AuxImpulseInfo const &);
    void UnPauseSound();
    bool LoadMapFromConsole(m3d::CConsoleParams const &,bool);
    int StartPlayingVideo(char const *,int (CMiracle3d::*)());
    virtual ~CMiracle3d();
    float GetNormalTimeScale() const ;
    void SetNormalTimeScale(float);
    virtual m3d::Class* GetClass() const ;
    void PauseSound();
    bool SaveGame(CStr const &,bool);
    bool LoadMap(CStr const &,bool,m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *,ai::ObjContainer::eSAVE_TYPES);
    int ValidateCameraAngles();
    virtual char const * GetCallbackName() const ;
    virtual bool KillPostEffect(CStr const& effectName);
    virtual m3d::ui::Wnd * CaptureMouse(m3d::ui::Wnd *);
    int CollideCamera(CVector &,float &,CVector const &,CVector const &);
    int OnSkipCinematicMessage(m3d::AuxImpulseInfo const &);
    virtual void ReloadPostEffects();
    m3d::BlockMusicManager * GetBlockMusicManager();
    bool IsRenderAsBackground() const ;
    virtual int GetCurGameMode();
    virtual void SetMouseYAxisFlipped(bool bFlip);
    virtual void SetMouseXAxisFlipped(bool bFlip);
    ProfileManager * GetProfileManager() const ;

protected:
    virtual int OnEvent(m3d::Event const &);
    virtual int AddChild(m3d::Object *);
    virtual int RemoveChildForce(m3d::Object *);
    virtual int Render(bool);
    virtual int RemoveChild(m3d::Object *);
    void EmergencyRedrawAllObjs();
    virtual int DoneMedia();
    int Controls(double,double);
    virtual int InitMedia();
    float GetMeanHigh(float,float);
    virtual int FrameMove();
    virtual void HandleCommand(int,m3d::CConsoleParams const &);
    virtual bool HandleCVar(m3d::CVar const *,m3d::CConsoleParams const &);
    virtual int NewFrame();
    float GetMaxHigh(float,float);

public:
    RT_CLASS_DECLARE(CMiracle3d);

public:
    ITruxxUiManager* m_pInterfaceManager;
    Player m_player;
    CVector m_hitPoint;

private:
    float getFov() const ;
    virtual void setFov(float value);
    void RegisterConsoleCommands();
    virtual int InitImpulses();
    void InitBackgroundTexture();
    void initZoom();
    int CreateInterfaceManager();
    void DrawBackground();
    void CaptureBackground();

private:
    CVector m_oldPositionValue;
    int m_elapsedtime;
    int m_numModals;
    bool m_noclip;
    CVector m_flyCamTurn;
    CVector m_flyCamMove;
    float m_gameCameraRho;
    CVector m_gameSlideAuto;
    bool m_srvKeys[10];
    bool m_paused;
    float m_saveTimeScale;
    bool m_userPaused;
    PointBase<float> m_dragHit;
    std::vector<CStr> m_musicNames;
    HackedMusicType m_hackedMusicType;
    bool m_bMustStartNewMusic;
    m3d::BlockMusicManager *m_blockMusicManager;
    m3d::TownMusicManager *m_townMusicManager;
    m3d::RadioEngine *m_radioEngine;
    //int (__thiscall *m_onFinishVideoPlaying)(CMiracle3d *this);
    m3d::CVar m_minDist;
    m3d::CVar m_maxDist;
    m3d::CVar m_cameraHeight;
    m3d::CVar m_collideCameraRadius;
    m3d::CVar m_smoothCameraRadius;
    m3d::CVar m_cameraSpeed;
    m3d::CVar m_maxAngle;
    m3d::CVar m_minAngle;
    m3d::CVar m_fov;
    float m_maxTimeScale;
    float m_minTimeScale;
    float m_normalTimeScale;
    bool m_gameInited;
    m3d::CVar m_cvSoundDebug;
    bool zoomInited;
    float m_Fov0;
    PostEffectManager *m_postEffect;
    bool m_playingVideo;
    ProfileManager *m_profileManager;
    CurGameMode m_curGameMode;
    bool m_bBackgroundTextureIsValid;
    bool m_bRenderAsBackground;
    m3d::rend::TexHandle m_backgroundTexture;
};
