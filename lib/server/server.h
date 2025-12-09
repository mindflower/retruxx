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

    class CServer : public m3d::IConHandler
    {
    public:
        /* 0x0004 */ ai::Map* pGlobalMap;
        CServer(const ai::CServer&);
        CServer();
        virtual  ~CServer() /* 0x08 */;
        bool GetPause() const;
        void SetPause(bool pause);
        int GetLastId();
        void SetLastId(int newId);
        virtual void HandleCommand(int cmdID, const m3d::CConsoleParams& params) override /* 0x00 */;
        virtual bool HandleCVar(const m3d::CVar* cvar, const m3d::CConsoleParams& params) override /* 0x04 */;
        static void __fastcall Register();
        void Init(m3d::CWorld* world);
        void Clear();
        void InitOnce();
        void ClearOnce();
        void Load(ai::StartupMode mode, m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode, bool bContiniousMap, ai::ObjContainer::eSAVE_TYPES saveType);
        void LoadVisitedMap(const CStr& mapFileName, bool bContiniousMap);
        void SaveVisitedMap(const CStr& mapFileName);
        void Update(float elapsedTime);
        void GetControlData();
        void PutGameData();
        m3d::CWorld* GetWorld();
        int GetPathFindQuant();
        float GetMaxWorkTime();
        CStr GetMaxWorkTimeClassNum();
        void RelinkSceneGraphNodes();
        int GetPrototypeId(const CStr& prototypeName);
        CStr GetFullNameByObjID(int objId);
        ai::eTolerance CheckTolerance(int PlayerID1, int PlayerID2);
        const ai::PrototypeInfo* GetPrototypeInfo(int prototypeId);
        CStr GetPrototypeFullName(int prototypeId);
        CStr GetPrototypeFullName(const CStr& modelName);
        void PostPlayerEvent(ai::eGameEvent eventId);
        unsigned int GetGlobalMapValue(long xIndex, long yIndex);
        void LoadGlobalMapFromRawFile(const char* fileName);
        void LoadGlobalPropertiesFromXML(const CStr& fileName);
        void LoadPrototypeNamesFromXML(const CStr& fileName);
        void LoadTriggersFromXML(const CStr& fileName);
        ai::AffixManager* GetAffixManager() const;
        const ai::ExternalPaths* GetExternalPaths() const;
        ai::ExternalPaths* GetExternalPathsUnsafe();
        const ai::PlayerPassMap* GetPlayerPassMap() const;
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
        ai::PrototypeInfo* CreatePrototypeInfoByClassName(const CStr& className);

    private:
        /* 0x0008 */ bool m_StartServerUpdates;
        /* 0x0009 */ char Padding_218[3];
        /* 0x000c */ m3d::CWorld* m_pWorld;
        /* 0x0010 */ m3d::Level* m_level;
        /* 0x0014 */ ai::DynamicScene* m_pDynamicScene;
        /* 0x0018 */ ai::ObjContainer* m_pObjects;
        /* 0x001c */ ai::AIManager* pAIManager;
        /* 0x0020 */ ai::AffixManager* m_pAffixManager;
        /* 0x0024 */ ai::ExternalPaths* m_pExternalPaths;
        /* 0x0028 */ ai::PlayerPassMap* m_pPlayerPassMap;
        /* 0x002c */ float m_LastUpdateTime;
        CVector cam;
        /* 0x003c */ bool PrevState;
        /* 0x003d */ bool fPause;
        /* 0x003e */ char Padding_219[2];
        /* 0x0040 */ int m_lastId;
        /* 0x0044 */ float m_averageElapsedTime;
        retruxx::vector<float, retruxx::allocator<float> > m_lastElapsedTimes;
        /* 0x0058 */ int m_CurIndex;
        /* 0x005c */ int m_MaxAverageLength;
        /* 0x0060 */ bool m_Accumulation;
        /* 0x0061 */ bool m_AveElapsedTimeUsed;
        /* 0x0062 */ bool m_InCinematic;
        /* 0x0063 */ char Padding_220;
        /* 0x0064 */ int m_LastSenderID;
        /* 0x0068 */ m3d::Profiler* m_profilerTmpForServer;
        /* 0x006c */ m3d::Profiler* m_collideProfiler;
        /* 0x0070 */ m3d::Profiler* m_objectsUpdateProfiler;
        /* 0x0074 */ m3d::Profiler* m_serverUpdateProfiler;
        /* 0x0078 */ m3d::Profiler* m_pathFindingProfiler;
        /* 0x007c */ m3d::Profiler* m_bulletProfiler;
        /* 0x0080 */ m3d::Profiler* m_blastWaveProfiler;
        retruxx::queue<int, retruxx::deque<int, retruxx::allocator<int> > > m_consoleCommandsToPostProcess;
        void _RegisterConsoleCommands();
        void _PostProcessConsoleCommands();
        void _SetLevel(m3d::Level* newLevel);
        void SavePrevCinematicState();
        void RestorePrevCinematicState();
    }; /* size: 0x0098 */

    void SetDynamicScene(DynamicScene*);
    void UpdateLights();

    inline CServer* pServer = nullptr;
}
