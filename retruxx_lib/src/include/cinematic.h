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
    public:
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

    class Cinematic : public m3d::Object
    {
    protected:
        Cinematic();
        Cinematic(const m3d::Cinematic& cinematic);

    public:
        virtual  ~Cinematic() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x04 */;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x34 */;
        static m3d::Class m_classCinematic;
        /* 0x0034 */ int m_playTime;
        /* 0x0038 */ int m_fadeStartTime;
        /* 0x003c */ m3d::CinematicState m_state;
        /* 0x0040 */ bool m_bWasSkipped;
        /* 0x0041 */ bool m_bWasSkippedInEnterFadeOut;
        /* 0x0042 */ char Padding_205[2];
        m3d::CVar m_fadePeriod;

        using CinematicItems = std::list<m3d::CinematicItem, std::allocator<m3d::CinematicItem> >;

    public:
        std::list<m3d::CinematicItem, std::allocator<m3d::CinematicItem> > m_cinematicItems;
        m3d::CinematicItem m_curItem;
        bool Load(const char* FileName);
        bool InPlay() const;
        void LoadDefaults();
        void StartCinematic();
        bool SkipCinematic();
        void Play(float playTime);
        void PlayFromPoint(float playTime, int pointNum);
        void FlyAround(float phi, float theta, float radius, float playTime, const CVector& curPos, const char* flyPathName);
        CStr GetNextFlyPathName() const;
        void Stop();
        void Update(CCamera& cam, float dT);
        void UpdateCameraRotation(CCamera& cam);
        void SetAimToID(int objId);
        void SetAim(const CVector& lookAt);
        void SetRelativePoints(bool value);
        void SetRelativeRotations(bool value);
        void SetLookTo(bool value);
        void SetBaseToId(int objId);
        int GetFlags() const;
        void SetFlags(int flags);
        void SetFromPos(const CVector& pos, const Quaternion& rotation);
        bool SetPath(const char* pathName);
        void SetPathFromPos(const CVector& pos, const Quaternion& rotation, const char* pathName);
        m3d::CinematicType GetPlayType() const;
        bool bWaitWhenStop() const;
        void SetWaitWhenStop(bool wait);
        void SetLerpFromPreviousItem(bool bLerp);
        const char* GetFolder() const;
        void SetFolder(const char* Folder);
        float GetTimeToTheEnd() const;
        float GetFadePeriodForState(m3d::CinematicState state) const;
        bool bCanUpdate() const;
        bool bMustBeNextCinematic() const;
        void SetCameraStates(const std::vector<m3d::CameraPathState, std::allocator<m3d::CameraPathState> >& states);
        virtual void LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) /* 0x3c */;
        virtual void SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const /* 0x40 */;
        void DumpCurrentPath(const CStr& filename);
        bool GetDebugMode() const;
        void SetDebugMode(bool bDebug);
        void AddPointToCurrentPath(const CVector& point, const Quaternion& rot, float zoom, float speed);
        void InsertPointToCurrentPath(const CVector& point, const Quaternion& rot, float zoom, float speed);
        void SetCurrentDebugPointNum(int pointNum);
        void MoveCurrentDebugPoint(const CVector& point, const Quaternion& rot, float zoom);
        void RemoveCurrentDebugPoint();
        const m3d::CameraPath& GetPathByName(const CStr& pathName) const;
        bool RenderDebugInfo() const;
        const m3d::CinematicItem& GetCurItem() const;

    private:
        CStr m_folder;
        /* 0x00f0 */ float m_curTime;

        using PathMap = std::map<CStr, m3d::CameraPath, std::less<CStr>, std::allocator<std::pair<CStr const, m3d::CameraPath> > >;

    private:
        std::map<CStr, m3d::CameraPath, std::less<CStr>, std::allocator<std::pair<CStr const, m3d::CameraPath> > > m_paths;
        /* 0x0100 */ bool m_bDebugMode;
        /* 0x0101 */ char Padding_206[3];
        /* 0x0104 */ int m_curDebugPointNum;
        /* 0x0108 */ unsigned int m_numConsecutiveItemPlayingNow;
        CVector _GetPointToLookAt() const;
        CVector _GetBasePoint() const;
        Quaternion _GetBaseRotation() const;
        m3d::CameraPathState _GetPathState(float curTime) const;
        bool _TakeNextCinematicItem();
        void _PushCinematicItem();
        bool _bIsFirstItemPlayingNow() const;
    }; /* size: 0x010c */
}
