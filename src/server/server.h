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
        class XmlNode;
        class XmlFile;
    }

    class Profiler;
    class Level;
    class CWorld;
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

    class CServer :  public m3d::IConHandler
    {
    public:
        static CServer* pServer;

    public:
        void AddToCinematic(Obj *,bool);
        void AddToCinematic(int,bool);
        void SetPause(bool);
        m3d::Profiler * GetTmpProfiler();
        m3d::Profiler * GetBulletProfiler();
        m3d::Profiler * GetCollideProfiler();
        m3d::Profiler * GetObjectsUpdateProfiler();
        ExternalPaths const * GetExternalPaths() const ;
        PlayerPassMap const * GetPlayerPassMap() const ;
        void SetLastId(int);
        void Load(StartupMode,m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *,bool,ObjContainer::eSAVE_TYPES);
        bool GetPause() const ;
        int GetPrototypeId(CStr const &);
        PrototypeInfo * CreatePrototypeInfoByClassName(CStr const &);
        m3d::CWorld * GetWorld();
        void LoadVisitedMap(CStr const &,bool);
        static void __fastcall Register();
        float GetLevelSize() const ;
        m3d::Level * GetLevel() const ;
        m3d::Profiler * GetPathFindingProfiler();
        void RelinkSceneGraphNodes();
        CServer();
        void LoadTriggersFromXML(CStr const &);
        void PostPlayerEvent(eGameEvent);
        int GetLastId();
        CStr GetPrototypeFullName(int);
        CStr GetPrototypeFullName(CStr const &);
        void Update(float);
        void LoadGlobalMapFromRawFile(char const *);
        void ClearOnce();
        void EndCinematic();
        void GetControlData();
        unsigned int GetGlobalMapValue(long,long);
        int GetPathFindQuant();
        CStr GetFullNameByObjID(int);
        void Clear();
        void ResetCinematicObjects();
        PrototypeInfo const * GetPrototypeInfo(int);
        void InitOnce();
        eTolerance CheckTolerance(int,int);
        void LoadPrototypeNamesFromXML(CStr const &);
        void LoadGlobalPropertiesFromXML(CStr const &);
        AffixManager * GetAffixManager() const ;
        void StartCinematic();
        void Init(m3d::CWorld *);
        void PutGameData();
        virtual void HandleCommand(int,m3d::CConsoleParams const &);
        virtual ~CServer();
        virtual bool HandleCVar(m3d::CVar const *,m3d::CConsoleParams const &);
        void SaveVisitedMap(CStr const &);

    private:
        void _PostProcessConsoleCommands();
        void _RegisterConsoleCommands();
        void RestorePrevCinematicState();
        void SavePrevCinematicState();
        void _SetLevel(m3d::Level *);

    private:
        Map *pGlobalMap = nullptr;
        bool m_StartServerUpdates;
        m3d::CWorld *m_pWorld = nullptr;
        m3d::Level *m_level = nullptr;
        DynamicScene *m_pDynamicScene = nullptr;
        ObjContainer *m_pObjects = nullptr;
        AIManager *pAIManager = nullptr;
        AffixManager *m_pAffixManager = nullptr;
        ExternalPaths *m_pExternalPaths = nullptr;
        PlayerPassMap *m_pPlayerPassMap = nullptr;
        float m_LastUpdateTime;
        CVector cam{0.0, 0.0, 0.0};
        bool PrevState;
        bool fPause = false;
        int m_lastId = 0;
        float m_averageElapsedTime = 0.0;
        std::vector<float> m_lastElapsedTimes;
        int m_CurIndex = 0;
        int m_MaxAverageLength = 20;
        bool m_Accumulation = true;
        bool m_AveElapsedTimeUsed = false;
        bool m_InCinematic;
        int m_LastSenderID;
        m3d::Profiler *m_profilerTmpForServer = nullptr;
        m3d::Profiler *m_collideProfiler = nullptr;
        m3d::Profiler *m_objectsUpdateProfiler = nullptr;
        m3d::Profiler *m_serverUpdateProfiler = nullptr;
        m3d::Profiler *m_pathFindingProfiler = nullptr;
        m3d::Profiler *m_bulletProfiler = nullptr;
        m3d::Profiler *m_blastWaveProfiler = nullptr;
        std::queue<int> m_consoleCommandsToPostProcess;
    };
}
