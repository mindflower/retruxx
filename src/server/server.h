#pragma once
#include "relationship.h"
#include "objects/base/obj.h"
#include "objects/base/objcontainer.h"
#include <queue>
#include <vector>
#include <core/console/cvar.h>
#include <math/vector.h>

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
    class ExternalPaths;
    class PrototypeInfo;
    class Obj;
    class PlayerPassMap;
    class AffixManager;
    class AIManager;
    class DynamicScene;
    class Map;

    class CServer :  public m3d::IConHandler
    {
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
        Map *pGlobalMap;
        bool m_StartServerUpdates;
        m3d::CWorld *m_pWorld;
        m3d::Level *m_level;
        DynamicScene *m_pDynamicScene;
        ObjContainer *m_pObjects;
        AIManager *pAIManager;
        AffixManager *m_pAffixManager;
        ExternalPaths *m_pExternalPaths;
        PlayerPassMap *m_pPlayerPassMap;
        float m_LastUpdateTime;
        CVector cam;
        bool PrevState;
        bool fPause;
        int m_lastId;
        float m_averageElapsedTime;
        std::vector<float> m_lastElapsedTimes;
        int m_CurIndex;
        int m_MaxAverageLength;
        bool m_Accumulation;
        bool m_AveElapsedTimeUsed;
        bool m_InCinematic;
        int m_LastSenderID;
        m3d::Profiler *m_profilerTmpForServer;
        m3d::Profiler *m_collideProfiler;
        m3d::Profiler *m_objectsUpdateProfiler;
        m3d::Profiler *m_serverUpdateProfiler;
        m3d::Profiler *m_pathFindingProfiler;
        m3d::Profiler *m_bulletProfiler;
        m3d::Profiler *m_blastWaveProfiler;
        std::queue<int> m_consoleCommandsToPostProcess;
    };
}
