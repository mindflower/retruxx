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
#include "retruxx/common.h"

namespace m3d
{
    class IGeneratedTexture;
    class mVideoPlayer;
    struct AuxImpulseInfo;
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
        friend class LightsServer;

    public:
        Application(const m3d::Application&);
        Application(const char* logName);
        virtual ~Application();
        void MiniDump();
        int init(HINSTANCE__* hInstance, HICON__* icon, const CStr& configName, HWND__* forcedWnd, const CStr& cmdLine);
        int run();
        void done();
        int OneFrame();
        int SwitchDisplayModes(HWND__* wnd, int dwWidth, int dwHeight, bool bFullScreen);
        bool CheckCommandLineParam(const char* param) const;
        virtual void setZoom(float zoom);
        virtual float getZoom();

    protected:
        m3d::CmdLine m_cmdLine;

    public:
        int SetFont(const CStr& fontName, float height, unsigned int flags, unsigned int cp);
        int SetFontSelfMaking(const CStr& fontName, float height);
        int DrawTextAbs(float x, float y, unsigned int dwColor, const CStr& strText, unsigned int dwFlags, int fid);
        int DrawTextAbsT(float sx, float sy, unsigned int dwColor, const CStr& str, unsigned int dwFlags, int fid);
        int DrawTextRel(float x, float y, unsigned int dwColor, const CStr& strText, unsigned int dwFlags, int fid);
        int DrawTextRelT(float x, float y, unsigned int dwColor, const CStr& strText, unsigned int dwFlags, int fid);

    private:
        int DrawStringRelClip(const m3d::ui::FormattedLine& fl, const m3d::ui::DrawInfo& di);

    public:
        int FormatText(retruxx::vector<m3d::ui::FormattedLine>& linesOfText, const PointBase<float>& at, const CStr& textIn, const m3d::ui::DrawInfo& di, m3d::TextWrapFlags wrapFlags, m3d::TextFormatFlags formatFlags);
        int DrawTextRelClip(const PointBase<float>& at, const CStr& str, const m3d::ui::DrawInfo& di, m3d::TextWrapFlags wrapFlag, m3d::TextFormatFlags formatFlag);
        int Render3DText0(const CVector& at, const CStr& str, unsigned int dwFlags, float scale);
        int GetTextExtent(const CStr& str, PointBase<float>& size, int fid, BoundsBase<float>* csz, int* minc, int* maxc, CStr* leftInvisibleSubstr, CStr* rightInvisibleSubstr);
        int FindLastColorInStr(const CStr& line);
        bool IsTextHieroglyphic(const CStr& text) const;
        int GetTextFit(const CStr& strText, PointBase<float>& size, float maxX, m3d::TextWrapFlags flags);
        void PutSprite2Abs(float x1, float y1, float tu1, float tv1, float x2, float y2, float tu2, float tv2, unsigned int c);
        void PutSprite2Abs(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float tu1, float tv1, float tu2, float tv2, float tu3, float tv3, float tu4, float tv4, float zval, unsigned int c);
        void PutSpriteAbs(float x1, float y1, float x2, float y2, unsigned int c);
        void PutSpriteAbs(float x, float y, float size2, unsigned int c);
        void PutSpriteAbs(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, unsigned int c);
        void PutSpriteRel(float, float, float, float, float, float, float, float, unsigned int);
        void PutSpriteRel(float x1, float y1, float x2, float y2, unsigned int c);
        void PutSpriteRel(float, float, float, unsigned int);
        void PutSprite2Rel(float x1, float y1, float tu1, float tv1, float x2, float y2, float tu2, float tv2, unsigned int c);
        void PutSpriteRelRot(float, float, float, unsigned int, float, float, float, float, float, float);
        void PutSpriteRelRot(float cx, float cy, float sx, float sy, unsigned int c, float angleVerts, float avx, float avy, float angleImage, float aix, float aiy);
        void PutSpriteAbsRot(float, float, float, unsigned int, float, float, float, float, float, float);
        void PutSpriteAbsRot(float cx, float cy, float sx, float sy, unsigned int c, float angleVerts, float avx, float avy, float angleImage, float aix, float aiy);
        m3d::rend::IRenderer* m_renderer = nullptr;
        snd::ISound* m_sound = nullptr;
        m3d::input::IInput* m_input = nullptr;
        m3d::Log* m_log = nullptr;
        CStr m_cfgName;
        static m3d::Application* g_pApp;
        void sysError(const CStr& whence, const CStr& assertion);
        void panic();
        void texGenEnqueue(m3d::IGeneratedTexture* tex);
        void texGenProcess(m3d::IGeneratedTexture* tex);
        void EnqueueMessage(int msg, int param0, int param1, int p2, int p3, const CStr& param4, const m3d::AIParam& param5);
        int ImmediateMessage(int msg, int param0, int param1, int p2, int p3, const CStr& param4, const m3d::AIParam& param5);
        // retruxx adaptation: the shipped build packed window and object pointers into param0 and
        // param1; these carry them in Event::m_ptrEv, the int slots stay as they are.
        void EnqueueMessage(int msg, void* ptr0, int param1, int p2, int p3, const CStr& param4, const m3d::AIParam& param5);
        void EnqueueMessage(int msg, void* ptr0, void* ptr1, int p2, int p3, const CStr& param4, const m3d::AIParam& param5);
        int ImmediateMessage(int msg, void* ptr0, int param1, int p2, int p3, const CStr& param4, const m3d::AIParam& param5);
        int ImmediateMessage(int msg, void* ptr0, void* ptr1, int p2, int p3, const CStr& param4, const m3d::AIParam& param5);
        m3d::Event MakeMessageEvent(int msg, int param0, int param1, int p2, int p3, const CStr& param4, const m3d::AIParam& param5);
        void SetKeyboardFocus(m3d::IEventHandler* entity);
        int GetMouseX() const;
        int GetMouseY() const;
        void SetMouseXy(float absX, float absY);
        virtual float GetMouseSensitivity() const;
        virtual void SetMouseSensitivity(float sensitivity);
        virtual bool IsMouseYAxisFlipped() const;
        virtual void SetMouseYAxisFlipped(bool bFlip);
        virtual bool IsMouseXAxisFlipped() const;
        virtual void SetMouseXAxisFlipped(bool bFlip);
        void TexSoften(m3d::rend::TexHandle dstTex, m3d::rend::TexHandle srcTex, int numJitters, float jitterDist, unsigned int clr);
        void DrawLine(const CVector2& from, const CVector2& to, unsigned int color);
        void DrawLine(const CVector& from, const CVector& to, unsigned int color);
        void DrawBoundingRadius(const CVector& org, float r, unsigned int clr);
        void DrawCross(const CVector& org, float size, unsigned int clr);
        void DrawTri(CVector* tri, unsigned int clr);
        void DrawQuad(CVector* tri, unsigned int clr);
        void DrawWireRect(const CVector2& from, const CVector2& to, unsigned int color);
        void DrawWireRectRel(const CVector2& from, const CVector2& to, unsigned int color);
        void StartQuads(m3d::rend::VertexType vt);
        void FinishQuads();
        void RenderQuadXyzwct1(float x, float y, float sz, unsigned int clr);
        m3d::rend::VertexXYZWCT1* RenderQuadXyzwct1GetNextPtr();
        m3d::rend::VertexXYZCT1* RenderQuadXyzct1GetNextPtr();
        void SetFlushQuadsShader(m3d::rend::IEffect* shader);
        virtual void OnAfterDeviceReset() override;
        virtual void OnBeforeDeviceReset() override;
        virtual const char* GetCallbackName() const override;
        const CStr& GetStartupFolder() const;
        const CStr& GetImageName() const;
        int StartExclusiveMsgLoop();
        int FinishExclusiveMsgLoop();
        int HeightmapToNormalmap(unsigned short* src, unsigned int* dst, int sx, int sy);
        virtual bool GetPostEffectParam(const CStr& effectName, float& effParam);
        virtual bool SetPostEffectParam(const CStr& effectName, float effParam);
        virtual bool AddPostEffect(const CStr& effectName, float effParam);
        virtual bool KillPostEffect(const CStr& effectName);
        virtual void ReloadPostEffects();

    protected:
        virtual int InitMedia() = 0;
        virtual int DoneMedia() = 0;
        virtual int Render(bool) = 0;
        virtual int FrameMove() = 0;
        virtual int NewFrame() = 0;
        void RunBenchmark();
        int createInput();
        void doneInput();
        LONG_PTR m_dwWindowStyle;
        tagRECT m_rcWindowBounds;
        tagRECT m_rcWindowClient;
        CStr m_frameStats;
        unsigned long m_frameFillRate;
        char* m_strWindowTitle = nullptr;

    public:
        char* GetWindowTitle() const;

    protected:
        CStr m_startupFolder;
        CStr m_imageName;
        int m_appNeedToRedraw = false;

    public:
        int AppActive() const;
        CStr& GetFrameStats();

    protected:
        int m_screenShotPending = 0;
        int m_screenShotPendingAlways = 0;
        virtual void HandleCommand(int cmdID, const m3d::CConsoleParams& params) override;
        virtual bool HandleCVar(const m3d::CVar* cvar, const m3d::CConsoleParams& params) override;
        virtual int HandleEvent(const m3d::Event& ev) override;

    private:
        void* m_procTexNewTextureNotifyEvent = nullptr;
        void* m_procTexShutdownEvent = nullptr;
        void* m_procTexThreadHandle = nullptr;
        unsigned long m_procTexThreadId;
        static unsigned long __stdcall texGenThread(void* context);
        char Padding_309[4];
        m3d::Event m_eventsQueue[5000];
        int m_eventsQueueHead = 0;
        int m_eventsQueueTail = 0;
        m3d::IEventHandler* m_focusKbdEntity = nullptr;
        int m_mouseX = 100;
        int m_mouseY = 100;
        float m_mouseSensitivity = 1.0;
        bool m_bMouseYAxisFlipped = false;
        bool m_bMouseXAxisFlipped = false;
        char Padding_310[2];
        retruxx::vector<m3d::IEventHandler*, retruxx::allocator<m3d::IEventHandler*> > m_allEventHandlers;
        bool GetEvent(m3d::Event& event, bool removeFromQueue);
        void EnqueueEvent(const m3d::Event& event);

        struct CFontType
        {
            CStr m_fontId;
            m3d::rend::TexHandle m_tex;
            unsigned int m_glyphHeight;
            float m_scale;
            unsigned int m_fontFlags;
            unsigned int m_texSzX;
            unsigned int m_texSzY;
            float m_tcs[4][224];
            _ABC m_abc[224];
            _ABC GetAbcWidth(unsigned char);
            float GetCharWidthAdvanced(unsigned char);
            float GetCharWidthA(unsigned char);
            float GetCharHeight(unsigned char);
        };

    private:
        void enterFontRender();
        void finishFontRender();
        int AdjustWindowForChange(HWND__* hWnd, bool bFullScreen);
        virtual unsigned long GetStyleForRenderWindow(bool bFullScreen) const;
        int processWinMessages();
        int FillEngineMessages();
        int createRenderer();
        void doneRenderer();
        int createSound();
        void doneSound();
        int createProcTexThread();
        void doneProcTexThread();
        int createSprite();
        void doneSprite();
        int createUi();
        void doneUi();
        HINSTANCE m_hInputDll;
        HINSTANCE m_hRenderDll;
        HINSTANCE m_hSoundDll;
        bool m_breakLoop = false;
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        char Padding_311[3];
        int m_isAppActive = 0;
        int CheckAndLogPlatform();
        int64_t GetCpuFrequency(unsigned int uiMeasureMSecs);
        unsigned int m_prevBtnsMask = 0;
        int m_prevJoystickBtnsMask = 0;
        int m_prevJoystickAxisInclination[12] = {0};
        m3d::rend::VertexXYZWCT1 m_pointsVertsWct1[4000];
        m3d::rend::VertexXYZCT1 m_pointsVertsCt1[4000];
        int m_numPointsVerts;
        m3d::rend::IbHandle m_pointsVertsIb;
        m3d::rend::VbHandle m_pointsVertsVb;
        m3d::rend::VertexType m_vt;
        void* m_sourceVerts;
        int m_pointsVertsSz;
        unsigned int m_frameClearColor;
        m3d::rend::IEffect* m_flushQuadsShader = nullptr;
        int64_t m_cpuSpeed;
        void FlushQuads();
        void CreateQuadsIb();
        void RegisterConsoleCommands();

    public:
        m3d::DataServer& GetStaticModelsServer();
        m3d::DataServer& GetAnimatedModelsServer();
        m3d::DataServer& GetLightsServer();
        m3d::DataServer& GetSpritesServer();
        m3d::DataServer& GetParticlesServer();
        m3d::DataServer& GetSoundServer();
        m3d::DataServer& GetMusicServer();
        m3d::DataServer& GetLinesServer();
        m3d::DataServer& GetProjectorsServer();
        m3d::DataServer& GetDecalsServer();
        bool SaveUsedOnlyServers(const CStr& filename, retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& usedAnimatedHandles);
        bool SaveServers(const CStr& filename);
        bool LoadServers(const CStr& filename, bool bQuiet);
        void PostLoadServers();

    protected:
        m3d::DataServer* m_serverStaticModels = nullptr;
        m3d::DataServer* m_serverAnimatedModels = nullptr;
        m3d::DataServer* m_serverLights = nullptr;
        m3d::DataServer* m_serverSprites = nullptr;
        m3d::DataServer* m_serverParticles = nullptr;
        m3d::DataServer* m_serverSound = nullptr;
        m3d::DataServer* m_serverMusic = nullptr;
        m3d::DataServer* m_serverLines = nullptr;
        m3d::DataServer* m_serverProjectors = nullptr;
        m3d::DataServer* m_serverDecals = nullptr;
        void SetCodepage();

    public:
        _cpinfoexA m_codePage;
        _cpinfoexA m_unicodeCodePage;

    protected:
        m3d::rend::TexHandle m_texSplash;
        m3d::rend::TexHandle m_texSplash1;
        m3d::rend::TexHandle m_texProgr;
        m3d::rend::TexHandle m_texLogo;
        m3d::rend::TexHandle m_defLoadPicture;
        m3d::rend::TexHandle m_mainMenuLevelLoadBg;

        struct LoadScreenInfo
        {
            CStr m_name;
            CStr m_diz0;
            CStr m_diz1;
            CStr m_diz2;
            m3d::rend::TexHandle m_picture;
            LoadScreenInfo(const m3d::Application::LoadScreenInfo&);
            LoadScreenInfo();
            ~LoadScreenInfo();
        };

    protected:
        m3d::Application::LoadScreenInfo m_loadScreenInfo;

    public:
        void SetFrameClearColor(unsigned int color);
        virtual void PutSplash(int proc, const char* text);
        static void PutSplashCallBack(int proc, void* data);
        void DrawLogo();
        void PutSplashMainMenuLevelLoad(int proc, void* data);
        static void PutSplashCallBackQuiet(int, void*);
        int64_t GetMeasuredCpuFrequency() const;
        m3d::CameraController* m_cameraController;
        retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> > m_cachedSoundIDs;
        bool m_stopallsounds;
        bool m_pauseAllowed;
        char Padding_312[2];
        int m_timeFromLevelLoaded;
        m3d::CIntHash<int> m_belongToIdxHash;
        m3d::CameraController* GetCameraController();
        float GetOnScreenSize(const CVector& o, float radius);
        bool StartPlayingMusic(const char* musicName, bool bDoLoop, bool bImmediate);
        void StopPlayingMusic();
        virtual void Pause();
        virtual void UnPause();

    protected:
        m3d::ProfilerStack m_profilerStack;
        m3d::DbgCounterStack m_counterStack;
        unsigned int m_profiler_OneFrame;
        unsigned int m_profiler_Render;
        unsigned int m_profiler_UiRender;
        bool m_bDrawStats = false;
        bool m_bDrawMemoryStats = false;
        bool m_bDrawCounters = false;
        bool m_bDrawGraph = false;

    public:
        m3d::ProfilerStack& GetProfilerStack();
        m3d::DbgCounterStack& GetDbgCounterStack();
        m3d::Profiler* CreateProfiler(const char* name);
        m3d::DbgCounter* CreateCounter(const char* name);
        CStr GetClassDesc(m3d::Class* cls) const;
        CStr GetNativeFuncDesc(const char* funcName) const;
        int SaveExportDescToFile(m3d::fs::FileStream& outputFile, bool skipEmpty) const;

    protected:
        bool m_waitForAnykey = false;
        bool m_waitNetworkServer = false;
        bool m_enginePlayingVideo = false;
        bool m_isRenderingAllowed = true;
        bool m_isConsoleAllowed = false;
        bool m_bShowRenderStats = false;
        bool m_bShowDeviceMemStats = false;

    public:
        void WaitForAnykey();
        void ForbidRendering();
        void AllowRendering();
        char Padding_313;
        m3d::mVideoPlayer* M3dVideoPlayer;
        void ClearViewportToBlack();
        bool m_bDoNotLoadMainmenuLevel;
        bool DoNotLoadMainMenuLevel();

        struct DetailSettings
        {
            bool m_dsShadows;
            char Padding_308[3];
            float m_lsViewDistanceDivider;
            int m_lsWaterMethod;
            float m_NPatchLevel;
            void SaveGameSettings();
            void RestoreGameSettings();
            void SetMenuLevelSettings();
            DetailSettings();
        };

        char Padding_314[3];

    public:
        m3d::Application::DetailSettings m_detailSettings;
        m3d::IImpulse* m_pImpulses;
        virtual int GetCurGameMode();
        virtual int GetCurDifficultyLevel() const;
        virtual bool bIsMousePointing() const;
        virtual bool GetMouseHitPoint(CVector& hitPoint, m3d::SgNode*& sgNode);
        virtual int OnChangeMode(const m3d::AuxImpulseInfo& impInfo);

    protected:
        virtual int InitImpulses();
        virtual void DoneImpulses();
        virtual int OnLoosingFocus() override;
        void ProcessAllEvents();
        void DiscardAllEvents();

    public:
        int EnableDXCursor(bool bEnable);
        bool IsDXCursorEnabled() const;
        HWND__* GetRenderWindow() const;

    private:
        bool m_bDXCursorEnabled = true;

        struct MouseInfo
        {
            MouseInfo(const m3d::Application::MouseInfo&);
            MouseInfo();
            void SetUpForCurPos(const PointBase<int>& curPos);
            void ResetDelta();
            const PointBase<int>& GetDeltaDuringGameFrame() const;
            const PointBase<int>& GetLastPos() const;
            PointBase<int> m_deltaDuringGameFrame;
            PointBase<int> m_lastPos;
        };

        char Padding_315[3];

    private:
        m3d::Application::MouseInfo m_mouseInfo;
        HWND m_renderWindow = NULL;
        m3d::IConHandler* m_soundConHandler = nullptr;

    protected:
        void CaptureAndClipSystemCursor(bool bState);
        void ShowSystemCursor(bool bShow);

    public:
        bool m_bGuiWasHiddenBeforeCinematic = false;
        char Padding_316[3];
        m3d::Cinematic* m_cinematic;
        virtual int HandleCinematic(float dT);
        CCamera m_curCamera;
        virtual void LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        virtual void SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        virtual void ChangeLanguage();
        unsigned long GetLocale() const;
        virtual m3d::ui::MbRetCodes RunMsgBoxDlg(const CStr& caption, const CStr& message, unsigned int flags, bool bPause);
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classApplication;
    };
}

#define M3D_APP m3d::Application::g_pApp
#define M3D_RENDERER m3d::Application::g_pApp->m_renderer
