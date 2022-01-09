#pragma once
#include <list>
#include <map>
#include <vector>
#include <core/clazz.h>
#include <core/console/cvar.h>
#include <math/vector.h>
#include <math/quaternion.h>

class CCamera;

namespace m3d
{

    class CameraPathState
    {
    public:
        CameraPathState(CameraPathState const&);
        CameraPathState(CVector const&, Quaternion const&, float, float, float);
        void SaveToXmlRuntime(cmn::XmlFile*, cmn::XmlNode*) const;
        void LoadFromXmlRuntime(cmn::XmlFile*, cmn::XmlNode const*);

    private:
        CVector m_point;
        Quaternion m_rotation;
        float m_zoom;
        float m_speed;
        float m_flyTime;
    };

    class CameraPath
    {
    public:
        void insert(int, CameraPathState const&);
        unsigned int size() const;
        void GetCameraForTime(float, CVector&, Quaternion&, float&) const;
        bool empty() const;
        void CalcFlyTimes(unsigned int, bool);
        void push_back(CameraPathState const&);
        void MovePoint(int, CameraPathState const&);
        void CalcFullLength(unsigned int);
        float GetFullLength() const;
        void SaveToXmlRuntime(cmn::XmlFile*, cmn::XmlNode*) const;
        void LoadFromXml(cmn::XmlFile*, cmn::XmlNode const*);
        void SetFullTime(float);
        void InitByStates(std::vector<CameraPathState, std::allocator<CameraPathState> > const&);
        void clear();
        void RemovePoint(int);
        void LoadFromXmlRuntime(cmn::XmlFile*, cmn::XmlNode const*);
        CameraPathState& operator[](unsigned int);
        CameraPathState const& operator[](unsigned int) const;
        float GetFullTime() const;

    private:
        float _CalcSplineSegmentLength(unsigned int, unsigned int) const;
        void _DeFix();
        void _Fix();

    private:
        std::vector<CameraPathState> m_cameraPathStates;
        float m_fullLength = 0.0;
        float m_fullTime = 1.0;
    };

    enum CinematicState
    {
        CINEMATIC_ENTER_FADE_OUT = 0x0,
        CINEMATIC_ENTER_FADE_IN = 0x1,
        CINEMATIC_IS_PLAYING = 0x2,
        CINEMATIC_EXIT_FADE_OUT = 0x3,
        CINEMATIC_EXIT_FADE_IN = 0x4,
        CINEMATIC_NOT_INITED = 0x5,
        CINEMATIC_IS_HANGING = 0x6,
        CINEMATIC_KNOCKING_AT_DOOR = 0x7,
    };

    enum CinematicType
    {
        CINEMATIC_OFF = 0x0,
        CINEMATIC_PLAY_PATH = 0x1,
        CINEMATIC_FLY_AROUND = 0x2,
    };
    class CinematicItem
    {
    private:
        int m_flags = 0;
        bool m_bLookTo = false;
        int m_idToLookAt = -1;
        CVector m_pointToLookAt{0.0, 0.0, 0.0};
        int m_baseId = -1;
        bool m_bRelativeRotations = false;
        bool m_bRelativePoints = false;
        m3d::CinematicType m_playType = CINEMATIC_OFF;
        bool m_bWaitWhenStop = false;
        m3d::CameraPath m_cameraPath;
        CStr m_cameraPathName;
        float m_finalPhi = 0.0;
        float m_finalTheta = 0.0;
        float m_finalRadius = 0.0;
        float m_startPhi = 0.0;
        float m_startTheta = 0.0;
        float m_startRadius = 0.0;
        bool m_bLerpFromPreviousItem = false;
    };

    class Cinematic :  public Object
    {
    public:
        bool SkipCinematic();
        bool RenderDebugInfo() const ;
        bool bMustBeNextCinematic() const ;
        void SetCurrentDebugPointNum(int);
        float GetFadePeriodForState(CinematicState) const ;
        virtual Class * GetClass() const ;
        void SetRelativePoints(bool);
        virtual void SaveToXml(cmn::XmlFile *,cmn::XmlNode *) const ;
        virtual ~Cinematic();
        void SetCameraStates(std::vector<CameraPathState,std::allocator<CameraPathState> > const &);
        void SetDebugMode(bool);
        bool GetDebugMode() const ;
        void SetPathFromPos(CVector const &,Quaternion const &,char const *);
        void DumpCurrentPath(CStr const &);
        bool InPlay() const ;
        CinematicType GetPlayType() const ;
        void SetLookTo(bool);
        void UpdateCameraRotation(CCamera &);
        void InsertPointToCurrentPath(CVector const &,Quaternion const &,float,float);
        bool Load(char const *);
        int GetFlags() const ;
        void SetFlags(int);
        void PlayFromPoint(float,int);
        void SetAim(CVector const &);
        void SetAimToID(int);
        CStr GetNextFlyPathName() const ;
        void AddPointToCurrentPath(CVector const &,Quaternion const &,float,float);
        void Play(float);
        static Class * GetBaseClass();
        void Stop();
        void RemoveCurrentDebugPoint();
        void FlyAround(float,float,float,float,CVector const &,char const *);
        float GetTimeToTheEnd() const ;
        static Object * CreateObject();
        void SetFromPos(CVector const &,Quaternion const &);
        void SetRelativeRotations(bool);
        void SetWaitWhenStop(bool);
        void Update(CCamera &,float);
        void SetLerpFromPreviousItem(bool);
        CinematicItem const & GetCurItem() const ;
        bool bCanUpdate() const ;
        void StartCinematic();
        void SetFolder(char const *);
        char const * GetFolder() const ;
        virtual void LoadFromXml(cmn::XmlFile *,cmn::XmlNode const *);
        void SetBaseToId(int);
        virtual Object * Clone();
        bool SetPath(char const *);
        bool bWaitWhenStop() const ;
        CameraPath const & GetPathByName(CStr const &) const ;
        void LoadDefaults();
        void MoveCurrentDebugPoint(CVector const &,Quaternion const &,float);

    protected:
        Cinematic();

    public:
        RT_CLASS_DECLARE(Cinematic);

    public:
        CinematicState m_state = CINEMATIC_NOT_INITED;
        CVar m_fadePeriod;

    private:
        CVector _GetPointToLookAt() const ;
        CameraPathState _GetPathState(float) const ;
        CVector _GetBasePoint() const ;
        Quaternion _GetBaseRotation() const ;
        bool _TakeNextCinematicItem();
        void _PushCinematicItem();
        bool _bIsFirstItemPlayingNow() const ;

    private:
        int m_playTime = 0;
        int m_fadeStartTime = 0;
        bool m_bWasSkipped = false;
        bool m_bWasSkippedInEnterFadeOut = false;
        std::list<CinematicItem> m_cinematicItems;
        CinematicItem m_curItem;
        CStr m_folder;
        float m_curTime = 0.0;
        std::map<CStr,CameraPath> m_paths;
        bool m_bDebugMode = false;
        int m_curDebugPointNum = -1;
        unsigned int m_numConsecutiveItemPlayingNow = 0;
    };
}
