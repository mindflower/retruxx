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
        CameraPath();
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
        float m_fullLength;
        float m_fullTime;
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
        int m_flags;
        bool m_bLookTo;
        int m_idToLookAt;
        CVector m_pointToLookAt;
        int m_baseId;
        bool m_bRelativeRotations;
        bool m_bRelativePoints;
        m3d::CinematicType m_playType;
        bool m_bWaitWhenStop;
        m3d::CameraPath m_cameraPath;
        CStr m_cameraPathName;
        float m_finalPhi;
        float m_finalTheta;
        float m_finalRadius;
        float m_startPhi;
        float m_startTheta;
        float m_startRadius;
        bool m_bLerpFromPreviousItem;
    };

    class Cinematic :  public Object
    {
    public:
        RT_CLASS_DECLARE(Cinematic);

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
        static Class * __fastcall GetBaseClass();
        void Stop();
        void RemoveCurrentDebugPoint();
        void FlyAround(float,float,float,float,CVector const &,char const *);
        float GetTimeToTheEnd() const ;
        static Object * __fastcall CreateObject();
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
        Cinematic(Cinematic const &);
        Cinematic();

    public:
        CinematicState m_state;
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
        int m_playTime;
        int m_fadeStartTime;
        bool m_bWasSkipped;
        bool m_bWasSkippedInEnterFadeOut;
        std::list<CinematicItem> m_cinematicItems;
        CinematicItem m_curItem;
        CStr m_folder;
        float m_curTime;
        std::map<CStr,CameraPath> m_paths;
        bool m_bDebugMode;
        int m_curDebugPointNum;
        unsigned int m_numConsecutiveItemPlayingNow;
    };
}
