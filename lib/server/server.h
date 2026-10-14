#pragma once
#include <math/vector.h>
#include "relationship.h"
#include "objects/base/obj.h"
#include "objects/base/objcontainer.h"
#include <core/console/console.h>
#include <queue>

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }  // namespace cmn

    class Profiler;
    class Level;
    class CWorld;
}  // namespace m3d

namespace m3d
{
    struct sArgStack;
}

namespace ai
{
    class ObjContainer;
    class ExternalPaths;
    class PrototypeInfo;
    class Obj;
    class PlayerPassMap;
    class AffixManager;
    class AIManager;
    class DynamicScene;
    class Map;

    enum StartupMode
    {
        LOCAL_GAME = 0x0,
        SERVER_GAME = 0x1,
        CLIENT_GAME = 0x2,
    };

    int n_EndCinematic(m3d::sArgStack& scriptStack);

    class CServer : public m3d::IConHandler
    {
        // The n_EndCinematic script callback clears the cinematic flag directly.
        friend int ::ai::n_EndCinematic(m3d::sArgStack&);

    public:
        ai::Map* pGlobalMap;
        CServer(ai::CServer const&);
        CServer();
        virtual ~CServer();
        bool GetPause() const;
        void SetPause(bool pause);
        int GetLastId();
        void SetLastId(int newId);
        virtual void HandleCommand(int cmdID, m3d::CConsoleParams const& params) override;
        virtual bool HandleCVar(m3d::CVar const* cvar, m3d::CConsoleParams const& params) override;
        static void __fastcall Register();
        void Init(m3d::CWorld* world);
        void Clear();
        void InitOnce();
        void ClearOnce();
        void Load(
            ai::StartupMode mode,
            m3d::cmn::XmlFile* xmlFile,
            m3d::cmn::XmlNode const* xmlNode,
            bool bContiniousMap,
            ai::ObjContainer::eSAVE_TYPES saveType);
        void LoadVisitedMap(CStr const& mapFileName, bool bContiniousMap);
        void SaveVisitedMap(CStr const& mapFileName);
        void Update(float elapsedTime);
        void GetControlData();
        void PutGameData();
        m3d::CWorld* GetWorld();
        int GetPathFindQuant();
        float GetMaxWorkTime();
        CStr GetMaxWorkTimeClassNum();
        void RelinkSceneGraphNodes();
        int GetPrototypeId(CStr const& prototypeName);
        CStr GetFullNameByObjID(int objId);
        ai::eTolerance CheckTolerance(int PlayerID1, int PlayerID2);
        ai::PrototypeInfo const* GetPrototypeInfo(int prototypeId);
        CStr GetPrototypeFullName(int prototypeId);
        CStr GetPrototypeFullName(CStr const& modelName);
        void PostPlayerEvent(ai::eGameEvent eventId);
        unsigned int GetGlobalMapValue(long xIndex, long yIndex);
        void LoadGlobalMapFromRawFile(char const* fileName);
        void LoadGlobalPropertiesFromXML(CStr const& fileName);
        void LoadPrototypeNamesFromXML(CStr const& fileName);
        void LoadTriggersFromXML(CStr const& fileName);
        ai::AffixManager* GetAffixManager() const;
        ai::ExternalPaths const* GetExternalPaths() const;
        ai::ExternalPaths* GetExternalPathsUnsafe();
        ai::PlayerPassMap const* GetPlayerPassMap() const;
        void StartCinematic();
        void EndCinematic();
        void ResetCinematicObjects();
        void AddToCinematic(ai::Obj* obj, bool WithChildren);
        void AddToCinematic(int id, bool WithChildren);
        bool IsInCinematic();
        m3d::Level* GetLevel() const;
        float GetLevelSize() const;
        m3d::Profiler* GetTmpProfiler();
        m3d::Profiler* GetCollideProfiler();
        m3d::Profiler* GetObjectsUpdateProfiler();
        m3d::Profiler* GetPathFindingProfiler();
        m3d::Profiler* GetBulletProfiler();
        m3d::Profiler* GetBlastWaveProfiler();
        ai::PrototypeInfo* CreatePrototypeInfoByClassName(CStr const& className);

    private:
        bool m_StartServerUpdates;
        m3d::CWorld* m_pWorld;
        m3d::Level* m_level;
        ai::DynamicScene* m_pDynamicScene;
        ai::ObjContainer* m_pObjects;
        ai::AIManager* pAIManager;
        ai::AffixManager* m_pAffixManager;
        ai::ExternalPaths* m_pExternalPaths;
        ai::PlayerPassMap* m_pPlayerPassMap;
        float m_LastUpdateTime;
        CVector cam;
        bool PrevState;
        bool fPause;
        int m_lastId;
        float m_averageElapsedTime;
        retruxx::vector<float, retruxx::allocator<float>> m_lastElapsedTimes;
        int m_CurIndex;
        int m_MaxAverageLength;
        bool m_Accumulation;
        bool m_AveElapsedTimeUsed;
        bool m_InCinematic;
        int m_LastSenderID;
        m3d::Profiler* m_profilerTmpForServer;
        m3d::Profiler* m_collideProfiler;
        m3d::Profiler* m_objectsUpdateProfiler;
        m3d::Profiler* m_serverUpdateProfiler;
        m3d::Profiler* m_pathFindingProfiler;
        m3d::Profiler* m_bulletProfiler;
        m3d::Profiler* m_blastWaveProfiler;
        retruxx::queue<int, retruxx::deque<int, retruxx::allocator<int>>> m_consoleCommandsToPostProcess;
        void _RegisterConsoleCommands();
        void _PostProcessConsoleCommands();
        void _SetLevel(m3d::Level* newLevel);
        void SavePrevCinematicState();
        void RestorePrevCinematicState();
    };

    void SetDynamicScene(DynamicScene*);
    void UpdateLights();

    inline CServer* pServer = nullptr;
}  // namespace ai
