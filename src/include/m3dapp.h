#pragma once
#include <i_event.h>
#include <windows.h>
#include <core/cmdline.h>
#include <core/containers.h>
#include <core/debugcounter.h>
#include <core/profilerstack.h>
#include <core/stringm3d.h>
#include <engine/console/console_internal.h>
#include <math/camera.h>
#include <renderer/i_renderer.h>
#include <renderer/i_renderer_vertex.h>
#include <ui/msgbox.h>
#include <ui/ui_srv.h>
#include <ui/wndstation.h>

namespace m3d
{
    class IGeneratedTexture;
    class mVideoPlayer;
    class AuxImpulseInfo;
    class SgNode;
    class Log;

    namespace fs
    {
        class FileStream;
    }

    class DataServer;
    class CameraController;
    class Cinematic;
    class IImpulse;

    class Application :
        public IConHandler,
        public IEventHandler,
        public IDeviceResetCallback,
        public ui::WndStation
    {
    public:
        class DetailSettings
        {
        public:
            void RestoreGameSettings();
            void SaveGameSettings();
            void SetMenuLevelSettings();
            DetailSettings();

        private:
            bool m_dsShadows;
            float m_lsViewDistanceDivider;
            int m_lsWaterMethod;
            float m_NPatchLevel;
        };

        class LoadScreenInfo
        {
        public:
            ~LoadScreenInfo();
            LoadScreenInfo();

        private:
            CStr m_name;
            CStr m_diz0;
            CStr m_diz1;
            CStr m_diz2;
            rend::TexHandle m_picture;
        };

        class MouseInfo
        {
        public:
            void ResetDelta();
            PointBase<int> const& GetDeltaDuringGameFrame() const;
            void SetUpForCurPos(PointBase<int> const& curPos);
            MouseInfo();
            PointBase<int> const& GetLastPos() const;

        private:
            PointBase<int> m_deltaDuringGameFrame;
            PointBase<int> m_lastPos;
        };

    public:
        static Application* g_pApp;

    public:
        DataServer & GetSoundServer();
        void texGenProcess(IGeneratedTexture *);
        int EnableDXCursor(bool);
        void DrawTri(CVector *,unsigned int);
        void ForbidRendering();
        virtual Class * GetClass() const ;
        DataServer & GetStaticModelsServer();
        void DrawCross(CVector const &,float,unsigned int);
        CStr const & GetImageName() const ;
        CameraController * GetCameraController();
        virtual int GetCurDifficultyLevel() const ;
        char * GetWindowTitle() const ;
        void MiniDump();
        void EnqueueMessage(int,int,int,int,int,CStr const &,AIParam const &);
        void DrawLogo();
        float GetOnScreenSize(CVector const &,float);
        virtual int HandleCinematic(float);
        static void __fastcall PutSplashCallBack(int,void *);
        void AllowRendering();
        bool StartPlayingMusic(char const *,bool,bool);
        DataServer & GetProjectorsServer();
        bool CheckCommandLineParam(char const *) const ;
        unsigned long GetLocale() const ;
        void DrawQuad(CVector *,unsigned int);
        void PutSpriteRel(float,float,float,float,unsigned int);
        virtual void SaveToXml(cmn::XmlFile *,cmn::XmlNode *) const ;
        virtual ~Application();
        bool IsDXCursorEnabled() const ;
        void TexSoften(rend::TexHandle,rend::TexHandle,int,float,unsigned int);
        bool IsTextHieroglyphic(CStr const &) const ;
        CStr GetNativeFuncDesc(char const *) const ;
        void StopPlayingMusic();
        virtual void SetMouseSensitivity(float);
        virtual float getZoom();
        bool LoadServers(CStr const &,bool);
        virtual bool SetPostEffectParam(CStr const &,float);
        virtual void ChangeLanguage();
        void DrawWireRect(CVector2 const &,CVector2 const &,unsigned int);
        void PostLoadServers();
        virtual void OnBeforeDeviceReset();
        void FinishQuads();
        virtual void setZoom(float);
        void PutSpriteRelRot(float,float,float,float,unsigned int,float,float,float,float,float,float);
        virtual void SetMouseXAxisFlipped(bool);
        virtual void SetMouseYAxisFlipped(bool);
        int init(HINSTANCE,HICON,CStr const &,HWND,CStr const &);
        void SetFlushQuadsShader(rend::IEffect *);
        int AppActive() const ;
        rend::VertexXYZCT1 * RenderQuadXyzct1GetNextPtr();
        DbgCounterStack & GetDbgCounterStack();
        virtual float GetMouseSensitivity() const ;
        virtual void LoadFromXml(cmn::XmlFile *,cmn::XmlNode const *);
        int SwitchDisplayModes(HWND__ *,int,int,bool);
        void done();
        virtual void OnAfterDeviceReset();
        virtual void ReloadPostEffects();
        virtual bool GetPostEffectParam(CStr const &,float &);
        int GetTextFit(CStr const &,PointBase<float> &,float,TextWrapFlags);
        int StartExclusiveMsgLoop();
        DataServer & GetSpritesServer();
        void panic();
        bool SaveServers(CStr const &);
        virtual bool GetMouseHitPoint(CVector &,SgNode * &);
        void StartQuads(rend::VertexType);
        int GetTextExtent(CStr const &,PointBase<float> &,int,BoundsBase<float> *,int *,int *,CStr *,CStr *);
        virtual bool IsMouseYAxisFlipped() const ;
        int DrawTextRelT(float,float,unsigned int,CStr const &,unsigned int,int);
        int FormatText(std::vector<ui::FormattedLine,std::allocator<ui::FormattedLine> > &,PointBase<float> const &,CStr const &,ui::DrawInfo const &,TextWrapFlags,TextFormatFlags);
        void WaitForAnykey();
        void SetFrameClearColor(unsigned int);
        int DrawTextAbs(float,float,unsigned int,CStr const &,unsigned int,int);
        Application(char const *logName);
        void sysError(CStr const &,CStr const &);
        int FinishExclusiveMsgLoop();
        virtual void UnPause();
        int ImmediateMessage(int,int,int,int,int,CStr const &,AIParam const &);
        virtual bool IsMouseXAxisFlipped() const ;
        static Class * __fastcall GetBaseClass();
        int OneFrame();
        void PutSpriteAbsRot(float,float,float,float,unsigned int,float,float,float,float,float,float);
        Profiler * CreateProfiler(char const *);
        void PutSprite2Rel(float,float,float,float,float,float,float,float,unsigned int);
        void SetKeyboardFocus(IEventHandler *);
        virtual int OnChangeMode(AuxImpulseInfo const &);
        void DrawWireRectRel(CVector2 const &,CVector2 const &,unsigned int);
        DataServer & GetLightsServer();
        int run();
        ProfilerStack & GetProfilerStack();
        int SetFont(CStr const &,float,unsigned int,unsigned int);
        bool SaveUsedOnlyServers(CStr const &,std::set<int,std::less<int>,std::allocator<int> > &);
        int Render3DText0(CVector const &,CStr const &,unsigned int,float);
        rend::VertexXYZWCT1 * RenderQuadXyzwct1GetNextPtr();
        virtual bool AddPostEffect(CStr const &,float);
        int FindLastColorInStr(CStr const &);
        void PutSplashMainMenuLevelLoad(int,void *);
        void RenderQuadXyzwct1(float,float,float,unsigned int);
        void DrawBoundingRadius(CVector const &,float,unsigned int);
        DataServer & GetMusicServer();
        void SetMouseXy(float,float);
        virtual int GetCurGameMode();
        int HeightmapToNormalmap(unsigned short *,unsigned int *,int,int);
        int SetFontSelfMaking(CStr const &,float);
        int GetMouseY() const ;
        int GetMouseX() const ;
        virtual void PutSplash(int,char const *);
        int DrawTextRel(float,float,unsigned int,CStr const &,unsigned int,int);
        HWND GetRenderWindow() const ;
        void PutSprite2Abs(float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,unsigned int);
        void PutSprite2Abs(float,float,float,float,float,float,float,float,unsigned int);
        virtual char const * GetCallbackName() const ;
        virtual ui::MbRetCodes RunMsgBoxDlg(CStr const &,CStr const &,unsigned int,bool);
        int DrawTextAbsT(float,float,unsigned int,CStr const &,unsigned int,int);
        int SaveExportDescToFile(fs::FileStream &,bool) const ;
        void DrawLine(CVector2 const &,CVector2 const &,unsigned int);
        void DrawLine(CVector const &,CVector const &,unsigned int);
        virtual void Pause();
        CStr GetClassDesc(Class *) const ;
        void ClearViewportToBlack();
        __int64 GetMeasuredCpuFrequency() const ;
        CStr const & GetStartupFolder() const ;
        void PutSpriteAbs(float,float,float,unsigned int);
        void PutSpriteAbs(float,float,float,float,unsigned int);
        void PutSpriteAbs(float,float,float,float,float,float,float,float,unsigned int);
        DataServer & GetAnimatedModelsServer();
        virtual bool KillPostEffect(CStr const &);
        DataServer & GetDecalsServer();
        int DrawTextRelClip(PointBase<float> const &,CStr const &,ui::DrawInfo const &,TextWrapFlags,TextFormatFlags);
        DataServer & GetParticlesServer();
        virtual bool bIsMousePointing() const ;
        DbgCounter * CreateCounter(char const *);
        void texGenEnqueue(IGeneratedTexture *);

    protected:
        int createInput();
        virtual void HandleCommand(int,CConsoleParams const &);
        void ProcessAllEvents();
        void doneInput();
        virtual int HandleEvent(Event const &);
        void CaptureAndClipSystemCursor(bool);
        virtual bool HandleCVar(CVar const *,CConsoleParams const &);
        void DiscardAllEvents();
        void SetCodepage();
        void RunBenchmark();
        void ShowSystemCursor(bool);
        virtual int OnLoosingFocus();
        virtual int InitImpulses();
        virtual void DoneImpulses();

    private:
        __int64 GetCpuFrequency(unsigned int);
        void FlushQuads();
        void CreateQuadsIb();
        int createRenderer();
        static long __stdcall WndProc(HWND,unsigned int,unsigned int,long);
        void doneRenderer();
        void doneUi();
        void RegisterConsoleCommands();
        void EnqueueEvent(Event const &);
        int processWinMessages();
        //RunBenchmark();
        bool GetEvent(Event &,bool);
        int createSprite();
        void finishFontRender();
        int DrawStringRelClip(ui::FormattedLine const &,ui::DrawInfo const &);
        int createProcTexThread();
        int createUi();
        static unsigned long __stdcall texGenThread(void *);
        int CheckAndLogPlatform();
        long MsgProc(HWND,unsigned int,unsigned int,long);
        int AdjustWindowForChange(HWND,bool);
        int createSound();
        void doneSprite();
        int FillEngineMessages();
        void doneSound();
        void doneProcTexThread();
        void enterFontRender();
        virtual unsigned long GetStyleForRenderWindow(bool) const ;

        virtual int InitMedia() = 0;
        virtual int NewFrame() = 0;
        virtual int FrameMove() = 0;
        virtual int Render(bool) = 0;

    public:
        snd::ISound* m_sound;
        rend::IRenderer* m_renderer;
        input::IInput* m_input;

    protected:
        CCamera m_curCamera;
        IImpulse* m_pImpulses;

    private:
        CmdLine m_cmdLine;
        Log *m_log;
        CStr m_cfgName;
        unsigned int m_dwWindowStyle;
        RECT m_rcWindowBounds;
        RECT m_rcWindowClient;
        CStr m_frameStats;
        unsigned int m_frameFillRate;
        char *m_strWindowTitle;
        CStr m_startupFolder;
        CStr m_imageName;
        int m_appNeedToRedraw;
        int m_screenShotPending;
        int m_screenShotPendingAlways;
        void *m_procTexNewTextureNotifyEvent;
        void *m_procTexShutdownEvent;
        void *m_procTexThreadHandle;
        unsigned int m_procTexThreadId;
        Event m_eventsQueue[5000];
        int m_eventsQueueHead;
        int m_eventsQueueTail;
        IEventHandler *m_focusKbdEntity;
        int m_mouseX;
        int m_mouseY;
        float m_mouseSensitivity;
        bool m_bMouseYAxisFlipped;
        bool m_bMouseXAxisFlipped;
        std::vector<IEventHandler *> m_allEventHandlers;
        HINSTANCE m_hInputDll;
        HINSTANCE m_hRenderDll;
        HINSTANCE m_hSoundDll;
        bool m_breakLoop;
        int m_isAppActive;
        unsigned int m_prevBtnsMask;
        int m_prevJoystickBtnsMask;
        int m_prevJoystickAxisInclination[12];
        rend::VertexXYZWCT1 m_pointsVertsWct1[4000];
        rend::VertexXYZCT1 m_pointsVertsCt1[4000];
        int m_numPointsVerts;
        rend::IbHandle m_pointsVertsIb;
        rend::VbHandle m_pointsVertsVb;
        rend::VertexType m_vt;
        void *m_sourceVerts;
        int m_pointsVertsSz;
        unsigned int m_frameClearColor;
        rend::IEffect *m_flushQuadsShader;
        __int64 m_cpuSpeed;
        DataServer *m_serverStaticModels;
        DataServer *m_serverAnimatedModels;
        DataServer *m_serverLights;
        DataServer *m_serverSprites;
        DataServer *m_serverParticles;
        DataServer *m_serverSound;
        DataServer *m_serverMusic;
        DataServer *m_serverLines;
        DataServer *m_serverProjectors;
        DataServer *m_serverDecals;
        _cpinfoexA m_codePage;
        _cpinfoexA m_unicodeCodePage;
        rend::TexHandle m_texSplash;
        rend::TexHandle m_texSplash1;
        rend::TexHandle m_texProgr;
        rend::TexHandle m_texLogo;
        rend::TexHandle m_defLoadPicture;
        rend::TexHandle m_mainMenuLevelLoadBg;
        LoadScreenInfo m_loadScreenInfo;
        CameraController *m_cameraController;
        std::set<CStr> m_cachedSoundIDs;
        bool m_stopallsounds;
        bool m_pauseAllowed;
        int m_timeFromLevelLoaded;
        CIntHash<int> m_belongToIdxHash;
        ProfilerStack m_profilerStack;
        DbgCounterStack m_counterStack;
        unsigned int m_profiler_OneFrame;
        unsigned int m_profiler_Render;
        unsigned int m_profiler_UiRender;
        bool m_bDrawStats;
        bool m_bDrawMemoryStats;
        bool m_bDrawCounters;
        bool m_bDrawGraph;
        bool m_waitForAnykey;
        bool m_waitNetworkServer;
        bool m_enginePlayingVideo;
        bool m_isRenderingAllowed;
        bool m_isConsoleAllowed;
        bool m_bShowRenderStats;
        bool m_bShowDeviceMemStats;
        mVideoPlayer *M3dVideoPlayer;
        bool m_bDoNotLoadMainmenuLevel;
        DetailSettings m_detailSettings;
        bool m_bDXCursorEnabled;
        MouseInfo m_mouseInfo;
        HWND m_renderWindow;
        IConHandler *m_soundConHandler;
        bool m_bGuiWasHiddenBeforeCinematic;
        Cinematic *m_cinematic;
    }; 
}
