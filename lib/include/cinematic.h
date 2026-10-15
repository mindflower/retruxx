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

    struct CameraPathState
    {
        CVector m_point;
        Quaternion m_rotation;
        float m_zoom;
        float m_speed;
        float m_flyTime;
        CameraPathState(CVector const& point, Quaternion const& rotation, float zoom, float flyTime, float speed);
        CameraPathState();
        void LoadFromXmlRuntime(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        void SaveToXmlRuntime(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
    };

    class CameraPath
    {
        // Cinematic's point editing re-fixes the path around its own changes.
        friend class Cinematic;

    public:
        static m3d::CameraPath const m_emptyPath;
        CameraPath(std::vector<m3d::CameraPathState, std::allocator<m3d::CameraPathState>> const&);
        CameraPath();
        void LoadFromXmlRuntime(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        void SaveToXmlRuntime(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        void LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        void CalcFlyTimes(unsigned int pointNum, bool recalcFullLength);
        void CalcFullLength(unsigned int pointNum);
        void GetCameraForTime(float curTime, CVector& pos, Quaternion& rot, float& zoom) const;
        void InitByStates(std::vector<m3d::CameraPathState, std::allocator<m3d::CameraPathState>> const& states);
        std::vector<m3d::CameraPathState, std::allocator<m3d::CameraPathState>>& GetCameraPathStates();
        std::vector<m3d::CameraPathState, std::allocator<m3d::CameraPathState>> const& GetCameraPathStates() const;
        void clear();
        bool empty() const;
        std::size_t size() const;
        void push_back(m3d::CameraPathState const& state);
        void insert(int pointNum, m3d::CameraPathState const& state);
        m3d::CameraPathState const& operator[](unsigned int index) const;
        m3d::CameraPathState& operator[](unsigned int index);
        void MovePoint(int pointNum, m3d::CameraPathState const& state);
        void RemovePoint(int pointNum);
        float GetFullTime() const;
        void SetFullTime(float fullTime);
        float GetFullLength() const;

    private:
        std::vector<m3d::CameraPathState, std::allocator<m3d::CameraPathState>> m_cameraPathStates;
        float m_fullLength;
        float m_fullTime;
        void _Fix();
        void _DeFix();
        float _CalcSplineSegmentLength(unsigned int iPoint1, unsigned int iPoint2) const;
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

    struct CinematicItem
    {
        int m_flags = 0;
        bool m_bLookTo = false;
        int m_idToLookAt = -1;
        CVector m_pointToLookAt{0.0, 0.0, 0.0};
        mutable int m_baseId = -1;
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
        Cinematic(m3d::Cinematic const& cinematic);

    public:
        virtual ~Cinematic() override;
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classCinematic;
        int m_playTime;
        int m_fadeStartTime;
        m3d::CinematicState m_state;
        bool m_bWasSkipped;
        bool m_bWasSkippedInEnterFadeOut;
        m3d::CVar m_fadePeriod;

        using CinematicItems = std::list<m3d::CinematicItem, std::allocator<m3d::CinematicItem>>;

    public:
        std::list<m3d::CinematicItem, std::allocator<m3d::CinematicItem>> m_cinematicItems;
        m3d::CinematicItem m_curItem;
        bool Load(char const* FileName);
        bool InPlay() const;
        void LoadDefaults();
        void StartCinematic();
        bool SkipCinematic();
        void Play(float playTime);
        void PlayFromPoint(float playTime, int pointNum);
        void FlyAround(
            float phi,
            float theta,
            float radius,
            float playTime,
            CVector const& curPos,
            char const* flyPathName);
        CStr GetNextFlyPathName() const;
        void Stop();
        void Update(CCamera& cam, float dT);
        void UpdateCameraRotation(CCamera& cam);
        void SetAimToID(int objId);
        void SetAim(CVector const& lookAt);
        void SetRelativePoints(bool value);
        void SetRelativeRotations(bool value);
        void SetLookTo(bool value);
        void SetBaseToId(int objId);
        int GetFlags() const;
        void SetFlags(int flags);
        void SetFromPos(CVector const& pos, Quaternion const& rotation);
        bool SetPath(char const* pathName);
        void SetPathFromPos(CVector const& pos, Quaternion const& rotation, char const* pathName);
        m3d::CinematicType GetPlayType() const;
        bool bWaitWhenStop() const;
        void SetWaitWhenStop(bool wait);
        void SetLerpFromPreviousItem(bool bLerp);
        char const* GetFolder() const;
        void SetFolder(char const* Folder);
        float GetTimeToTheEnd() const;
        float GetFadePeriodForState(m3d::CinematicState state) const;
        bool bCanUpdate() const;
        bool bMustBeNextCinematic() const;
        void SetCameraStates(std::vector<m3d::CameraPathState, std::allocator<m3d::CameraPathState>> const& states);
        virtual void LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        virtual void SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        void DumpCurrentPath(CStr const& filename);
        bool GetDebugMode() const;
        void SetDebugMode(bool bDebug);
        void AddPointToCurrentPath(CVector const& point, Quaternion const& rot, float zoom, float speed);
        void InsertPointToCurrentPath(CVector const& point, Quaternion const& rot, float zoom, float speed);
        void SetCurrentDebugPointNum(int pointNum);
        void MoveCurrentDebugPoint(CVector const& point, Quaternion const& rot, float zoom);
        void RemoveCurrentDebugPoint();
        m3d::CameraPath const& GetPathByName(CStr const& pathName) const;
        bool RenderDebugInfo() const;
        m3d::CinematicItem const& GetCurItem() const;

    private:
        CStr m_folder;
        float m_curTime;

        using PathMap =
            std::map<CStr, m3d::CameraPath, std::less<CStr>, std::allocator<std::pair<CStr const, m3d::CameraPath>>>;

    private:
        std::map<CStr, m3d::CameraPath, std::less<CStr>, std::allocator<std::pair<CStr const, m3d::CameraPath>>>
            m_paths;
        bool m_bDebugMode;
        int m_curDebugPointNum;
        unsigned int m_numConsecutiveItemPlayingNow;
        CVector _GetPointToLookAt() const;
        CVector _GetBasePoint() const;
        Quaternion _GetBaseRotation() const;
        m3d::CameraPathState _GetPathState(float curTime) const;
        bool _TakeNextCinematicItem();
        void _PushCinematicItem();
        bool _bIsFirstItemPlayingNow() const;
    };
}  // namespace m3d
