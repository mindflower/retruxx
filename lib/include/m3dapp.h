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
    public:
        Application(const m3d::Application&);
        Application(const char* logName);
        virtual ~Application() /* 0x00 */;
        void MiniDump();
        int init(HINSTANCE__* hInstance, HICON__* icon, const CStr& configName, HWND__* forcedWnd, const CStr& cmdLine);
        int run();
        void done();
        int OneFrame();
        int SwitchDisplayModes(HWND__* wnd, int dwWidth, int dwHeight, bool bFullScreen);
        bool CheckCommandLineParam(const char* param) const;
        virtual void setZoom(float zoom) /* 0x08 */;
        virtual float getZoom() /* 0x0c */;

    protected:
        /* 0x02e0 */ m3d::CmdLine m_cmdLine;

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
        /* 0x02fc */ m3d::rend::IRenderer* m_renderer = nullptr;
        /* 0x0300 */ snd::ISound* m_sound = nullptr;
        /* 0x0304 */ m3d::input::IInput* m_input = nullptr;
        /* 0x0308 */ m3d::Log* m_log = nullptr;
        /* 0x030c */ CStr m_cfgName;
        static m3d::Application* g_pApp;
        void sysError(const CStr& whence, const CStr& assertion);
        void panic();
        void texGenEnqueue(m3d::IGeneratedTexture* tex);
        void texGenProcess(m3d::IGeneratedTexture* tex);
        void EnqueueMessage(int msg, int param0, int param1, int p2, int p3, const CStr& param4, const m3d::AIParam& param5);
        int ImmediateMessage(int msg, int param0, int param1, int p2, int p3, const CStr& param4, const m3d::AIParam& param5);
        void SetKeyboardFocus(m3d::IEventHandler* entity);
        int GetMouseX() const;
        int GetMouseY() const;
        void SetMouseXy(float absX, float absY);
        virtual float GetMouseSensitivity() const /* 0x10 */;
        virtual void SetMouseSensitivity(float sensitivity) /* 0x14 */;
        virtual bool IsMouseYAxisFlipped() const /* 0x18 */;
        virtual void SetMouseYAxisFlipped(bool bFlip) /* 0x1c */;
        virtual bool IsMouseXAxisFlipped() const /* 0x20 */;
        virtual void SetMouseXAxisFlipped(bool bFlip) /* 0x24 */;
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
        virtual void OnAfterDeviceReset() override /* 0x08 */;
        virtual void OnBeforeDeviceReset() override /* 0x04 */;
        virtual const char* GetCallbackName() const override /* 0x00 */;
        const CStr& GetStartupFolder() const;
        const CStr& GetImageName() const;
        int StartExclusiveMsgLoop();
        int FinishExclusiveMsgLoop();
        int HeightmapToNormalmap(unsigned short* src, unsigned int* dst, int sx, int sy);
        virtual bool GetPostEffectParam(const CStr& effectName, float& effParam) /* 0x28 */;
        virtual bool SetPostEffectParam(const CStr& effectName, float effParam) /* 0x2c */;
        virtual bool AddPostEffect(const CStr& effectName, float effParam) /* 0x30 */;
        virtual bool KillPostEffect(const CStr& effectName) /* 0x34 */;
        virtual void ReloadPostEffects() /* 0x38 */;

    protected:
        virtual int InitMedia() = 0 /* 0x3c */;
        virtual int DoneMedia() = 0 /* 0x40 */;
        virtual int Render(bool) = 0 /* 0x44 */;
        virtual int FrameMove() = 0 /* 0x48 */;
        virtual int NewFrame() = 0 /* 0x4c */;
        void RunBenchmark();
        int createInput();
        void doneInput();
        /* 0x0318 */ unsigned long m_dwWindowStyle;
        /* 0x031c */ tagRECT m_rcWindowBounds;
        /* 0x032c */ tagRECT m_rcWindowClient;
        /* 0x033c */ CStr m_frameStats;
        /* 0x0348 */ unsigned long m_frameFillRate;
        /* 0x034c */ char* m_strWindowTitle = nullptr;

    public:
        char* GetWindowTitle() const;

    protected:
        /* 0x0350 */ CStr m_startupFolder;
        /* 0x035c */ CStr m_imageName;
        /* 0x0368 */ int m_appNeedToRedraw = false;

    public:
        int AppActive() const;
        CStr& GetFrameStats();

    protected:
        /* 0x036c */ int m_screenShotPending = 0;
        /* 0x0370 */ int m_screenShotPendingAlways = 0;
        virtual void HandleCommand(int cmdID, const m3d::CConsoleParams& params) override /* 0x00 */;
        virtual bool HandleCVar(const m3d::CVar* cvar, const m3d::CConsoleParams& params) override /* 0x04 */;
        virtual int HandleEvent(const m3d::Event& ev) override /* 0x04 */;

    private:
        /* 0x0374 */ void* m_procTexNewTextureNotifyEvent = nullptr;
        /* 0x0378 */ void* m_procTexShutdownEvent = nullptr;
        /* 0x037c */ void* m_procTexThreadHandle = nullptr;
        /* 0x0380 */ unsigned long m_procTexThreadId;
        static unsigned long __stdcall texGenThread(void* context);
        /* 0x0384 */ char Padding_309[4];
        /* 0x0388 */ m3d::Event m_eventsQueue[5000];
        /* 0x581c8 */ int m_eventsQueueHead = 0;
        /* 0x581cc */ int m_eventsQueueTail = 0;
        /* 0x581d0 */ m3d::IEventHandler* m_focusKbdEntity = nullptr;
        /* 0x581d4 */ int m_mouseX = 100;
        /* 0x581d8 */ int m_mouseY = 100;
        /* 0x581dc */ float m_mouseSensitivity = 1.0;
        /* 0x581e0 */ bool m_bMouseYAxisFlipped = false;
        /* 0x581e1 */ bool m_bMouseXAxisFlipped = false;
        /* 0x581e2 */ char Padding_310[2];
        /* 0x581e4 */ retruxx::vector<m3d::IEventHandler*, retruxx::allocator<m3d::IEventHandler*> > m_allEventHandlers;
        bool GetEvent(m3d::Event& event, bool removeFromQueue);
        void EnqueueEvent(const m3d::Event& event);

        struct CFontType
        {
            /* 0x0000 */ CStr m_fontId;
            /* 0x000c */ m3d::rend::TexHandle m_tex;
            /* 0x0010 */ unsigned int m_glyphHeight;
            /* 0x0014 */ float m_scale;
            /* 0x0018 */ unsigned int m_fontFlags;
            /* 0x001c */ unsigned int m_texSzX;
            /* 0x0020 */ unsigned int m_texSzY;
            /* 0x0024 */ float m_tcs[4][224];
            /* 0x0e24 */ _ABC m_abc[224];
            _ABC GetAbcWidth(unsigned char);
            float GetCharWidthAdvanced(unsigned char);
            float GetCharWidthA(unsigned char);
            float GetCharHeight(unsigned char);
        }; /* size: 0x18a4 */

    private:
        void enterFontRender();
        void finishFontRender();
        int AdjustWindowForChange(HWND__* hWnd, bool bFullScreen);
        virtual unsigned long GetStyleForRenderWindow(bool bFullScreen) const /* 0x50 */;
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
        /* 0x581f4 */ HINSTANCE m_hInputDll;
        /* 0x581f8 */ HINSTANCE m_hRenderDll;
        /* 0x581fc */ HINSTANCE m_hSoundDll;
        /* 0x58200 */ bool m_breakLoop = false;
        static long __stdcall WndProc(HWND__* hWnd, unsigned int uMsg, unsigned int wParam, long lParam);
        long MsgProc(HWND__* hWnd, unsigned int uMsg, unsigned int wParam, long lParam);
        /* 0x58201 */ char Padding_311[3];
        /* 0x58204 */ int m_isAppActive = 0;
        int CheckAndLogPlatform();
        int64_t GetCpuFrequency(unsigned int uiMeasureMSecs);
        /* 0x58208 */ unsigned int m_prevBtnsMask = 0;
        /* 0x5820c */ int m_prevJoystickBtnsMask = 0;
        /* 0x58210 */ int m_prevJoystickAxisInclination[12] = {0};
        /* 0x58240 */ m3d::rend::VertexXYZWCT1 m_pointsVertsWct1[4000];
        /* 0x737c0 */ m3d::rend::VertexXYZCT1 m_pointsVertsCt1[4000];
        /* 0x8aec0 */ int m_numPointsVerts;
        /* 0x8aec4 */ m3d::rend::IbHandle m_pointsVertsIb;
        /* 0x8aec8 */ m3d::rend::VbHandle m_pointsVertsVb;
        /* 0x8aecc */ m3d::rend::VertexType m_vt;
        /* 0x8aed0 */ void* m_sourceVerts;
        /* 0x8aed4 */ int m_pointsVertsSz;
        /* 0x8aed8 */ unsigned int m_frameClearColor;
        /* 0x8aedc */ m3d::rend::IEffect* m_flushQuadsShader = nullptr;
        /* 0x8aee0 */ int64_t m_cpuSpeed;
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
        /* 0x8aee8 */ m3d::DataServer* m_serverStaticModels = nullptr;
        /* 0x8aeec */ m3d::DataServer* m_serverAnimatedModels = nullptr;
        /* 0x8aef0 */ m3d::DataServer* m_serverLights = nullptr;
        /* 0x8aef4 */ m3d::DataServer* m_serverSprites = nullptr;
        /* 0x8aef8 */ m3d::DataServer* m_serverParticles = nullptr;
        /* 0x8aefc */ m3d::DataServer* m_serverSound = nullptr;
        /* 0x8af00 */ m3d::DataServer* m_serverMusic = nullptr;
        /* 0x8af04 */ m3d::DataServer* m_serverLines = nullptr;
        /* 0x8af08 */ m3d::DataServer* m_serverProjectors = nullptr;
        /* 0x8af0c */ m3d::DataServer* m_serverDecals = nullptr;
        void SetCodepage();

    public:
        /* 0x8af10 */ _cpinfoexA m_codePage;
        /* 0x8b02c */ _cpinfoexA m_unicodeCodePage;

    protected:
        /* 0x8b148 */ m3d::rend::TexHandle m_texSplash;
        /* 0x8b14c */ m3d::rend::TexHandle m_texSplash1;
        /* 0x8b150 */ m3d::rend::TexHandle m_texProgr;
        /* 0x8b154 */ m3d::rend::TexHandle m_texLogo;
        /* 0x8b158 */ m3d::rend::TexHandle m_defLoadPicture;
        /* 0x8b15c */ m3d::rend::TexHandle m_mainMenuLevelLoadBg;

        struct LoadScreenInfo
        {
            /* 0x0000 */ CStr m_name;
            /* 0x000c */ CStr m_diz0;
            /* 0x0018 */ CStr m_diz1;
            /* 0x0024 */ CStr m_diz2;
            /* 0x0030 */ m3d::rend::TexHandle m_picture;
            LoadScreenInfo(const m3d::Application::LoadScreenInfo&);
            LoadScreenInfo();
            ~LoadScreenInfo();
        }; /* size: 0x0034 */

    protected:
        /* 0x8b160 */ m3d::Application::LoadScreenInfo m_loadScreenInfo;

    public:
        void SetFrameClearColor(unsigned int color);
        virtual void PutSplash(int proc, const char* text) /* 0x54 */;
        static void PutSplashCallBack(int proc, void* data);
        void DrawLogo();
        void PutSplashMainMenuLevelLoad(int proc, void* data);
        static void PutSplashCallBackQuiet(int, void*);
        int64_t GetMeasuredCpuFrequency() const;
        /* 0x8b194 */ m3d::CameraController* m_cameraController;
        /* 0x8b198 */ retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> > m_cachedSoundIDs;
        /* 0x8b1a4 */ bool m_stopallsounds;
        /* 0x8b1a5 */ bool m_pauseAllowed;
        /* 0x8b1a6 */ char Padding_312[2];
        /* 0x8b1a8 */ int m_timeFromLevelLoaded;
        /* 0x8b1ac */ m3d::CIntHash<int> m_belongToIdxHash;
        m3d::CameraController* GetCameraController();
        float GetOnScreenSize(const CVector& o, float radius);
        bool StartPlayingMusic(const char* musicName, bool bDoLoop, bool bImmediate);
        void StopPlayingMusic();
        virtual void Pause() /* 0x58 */;
        virtual void UnPause() /* 0x5c */;

    protected:
        /* 0x8b1d4 */ m3d::ProfilerStack m_profilerStack;
        /* 0x8b1e8 */ m3d::DbgCounterStack m_counterStack;
        /* 0x8b210 */ unsigned int m_profiler_OneFrame;
        /* 0x8b214 */ unsigned int m_profiler_Render;
        /* 0x8b218 */ unsigned int m_profiler_UiRender;
        /* 0x8b21c */ bool m_bDrawStats = false;
        /* 0x8b21d */ bool m_bDrawMemoryStats = false;
        /* 0x8b21e */ bool m_bDrawCounters = false;
        /* 0x8b21f */ bool m_bDrawGraph = false;

    public:
        m3d::ProfilerStack& GetProfilerStack();
        m3d::DbgCounterStack& GetDbgCounterStack();
        m3d::Profiler* CreateProfiler(const char* name);
        m3d::DbgCounter* CreateCounter(const char* name);
        CStr GetClassDesc(m3d::Class* cls) const;
        CStr GetNativeFuncDesc(const char* funcName) const;
        int SaveExportDescToFile(m3d::fs::FileStream& outputFile, bool skipEmpty) const;

    protected:
        /* 0x8b220 */ bool m_waitForAnykey = false;
        /* 0x8b221 */ bool m_waitNetworkServer = false;
        /* 0x8b222 */ bool m_enginePlayingVideo = false;
        /* 0x8b223 */ bool m_isRenderingAllowed = true;
        /* 0x8b224 */ bool m_isConsoleAllowed = false;
        /* 0x8b225 */ bool m_bShowRenderStats = false;
        /* 0x8b226 */ bool m_bShowDeviceMemStats = false;

    public:
        void WaitForAnykey();
        void ForbidRendering();
        void AllowRendering();
        /* 0x8b227 */ char Padding_313;
        /* 0x8b228 */ m3d::mVideoPlayer* M3dVideoPlayer;
        void ClearViewportToBlack();
        /* 0x8b22c */ bool m_bDoNotLoadMainmenuLevel;
        bool DoNotLoadMainMenuLevel();

        struct DetailSettings
        {
            /* 0x0000 */ bool m_dsShadows;
            /* 0x0001 */ char Padding_308[3];
            /* 0x0004 */ float m_lsViewDistanceDivider;
            /* 0x0008 */ int m_lsWaterMethod;
            /* 0x000c */ float m_NPatchLevel;
            void SaveGameSettings();
            void RestoreGameSettings();
            void SetMenuLevelSettings();
            DetailSettings();
        }; /* size: 0x0010 */

        /* 0x8b22d */ char Padding_314[3];

    public:
        /* 0x8b230 */ m3d::Application::DetailSettings m_detailSettings;
        /* 0x8b240 */ m3d::IImpulse* m_pImpulses;
        virtual int GetCurGameMode() /* 0x60 */;
        virtual int GetCurDifficultyLevel() const /* 0x64 */;
        virtual bool bIsMousePointing() const /* 0x68 */;
        virtual bool GetMouseHitPoint(CVector& hitPoint, m3d::SgNode*& sgNode) /* 0x6c */;
        virtual int OnChangeMode(const m3d::AuxImpulseInfo& impInfo) /* 0x70 */;

    protected:
        virtual int InitImpulses() /* 0x74 */;
        virtual void DoneImpulses() /* 0x78 */;
        virtual int OnLoosingFocus() override /* 0x00 */;
        void ProcessAllEvents();
        void DiscardAllEvents();

    public:
        int EnableDXCursor(bool bEnable);
        bool IsDXCursorEnabled() const;
        HWND__* GetRenderWindow() const;

    private:
        /* 0x8b244 */ bool m_bDXCursorEnabled = true;

        struct MouseInfo
        {
            MouseInfo(const m3d::Application::MouseInfo&);
            MouseInfo();
            void SetUpForCurPos(const PointBase<int>& curPos);
            void ResetDelta();
            const PointBase<int>& GetDeltaDuringGameFrame() const;
            const PointBase<int>& GetLastPos() const;
            /* 0x0000 */ PointBase<int> m_deltaDuringGameFrame;
            /* 0x0008 */ PointBase<int> m_lastPos;
        }; /* size: 0x0010 */

        /* 0x8b245 */ char Padding_315[3];

    private:
        /* 0x8b248 */ m3d::Application::MouseInfo m_mouseInfo;
        /* 0x8b258 */ HWND m_renderWindow = NULL;
        /* 0x8b25c */ m3d::IConHandler* m_soundConHandler = nullptr;

    protected:
        void CaptureAndClipSystemCursor(bool bState);
        void ShowSystemCursor(bool bShow);

    public:
        /* 0x8b260 */ bool m_bGuiWasHiddenBeforeCinematic = false;
        /* 0x8b261 */ char Padding_316[3];
        /* 0x8b264 */ m3d::Cinematic* m_cinematic;
        virtual int HandleCinematic(float dT) /* 0x7c */;
        /* 0x8b268 */ CCamera m_curCamera;
        virtual void LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) /* 0x80 */;
        virtual void SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const /* 0x84 */;
        virtual void ChangeLanguage() /* 0x88 */;
        unsigned long GetLocale() const;
        virtual m3d::ui::MbRetCodes RunMsgBoxDlg(const CStr& caption, const CStr& message, unsigned int flags, bool bPause) /* 0x8c */;
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classApplication;
    }; /* size: 0x8b290 */

    static_assert(sizeof(Application) == 0x8b290);
}

#define M3D_APP m3d::Application::g_pApp
#define M3D_RENDERER m3d::Application::g_pApp->m_renderer
