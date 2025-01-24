#pragma once
#include "obj.h"
#include <core/containers.h>

class CVector;

namespace ai
{
    class ObjContainer;

    class GameTime
    {
        ///* 0x0004 */ char Padding_42[4];

    private:
        /* 0x0008 */ int64_t m_milliSeconds;
        /* 0x0010 */ int64_t m_milliSeconds0;
        static const int64_t SecondBase;
        static const int64_t MinuteBase;
        static const int64_t HourBase;
        static const int64_t DayBase;
        static const int64_t MonthBase;
        static const int64_t YearBase;

    public:
        GameTime(const ai::GameTime&);
        GameTime(int64_t milliSeconds);
        GameTime(int hour, int minute, int day, int month, int year);
        GameTime();
        void setInt64(int64_t milliSeconds);
        void setExpanded(int hour, int minute, int day, int month, int year);
        int64_t asInt64() const;
        m3d::AIParam asAIParam24Hour() const;
        m3d::AIParam asAIParam() const;
        void operator+=(float gameTimeSeconds);
        float Diff() const;
        float GameDiff(const ai::ObjContainer* container) const;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) /* 0x00 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const /* 0x04 */;
    }; /* size: 0x0018 */

    static_assert(sizeof(GameTime) == 0x0018);

    class ObjContainer : public m3d::Object
    {
    protected:
        ObjContainer();
        ObjContainer(const ai::ObjContainer& rhs);

    public:
        virtual  ~ObjContainer() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x04 */;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x34 */;
        static m3d::Class m_classObjContainer;

    private:
        static const int BITS_IN_MAX_OBJECTS;
        static const int MAX_OBJECTS;
        static const int MAX_OBJECTS_MASK;
        static const int MAX_OBJECTS_IN_CELL;

        struct Node
        {
            /* 0x0000 */ int m_id;
            /* 0x0004 */ int m_prevId;
            /* 0x0008 */ int m_nextId;
            /* 0x000c */ ai::Obj* m_value;
            /* 0x0010 */ bool m_isValid;
            /* 0x0011 */ char Padding_197[3];
            /* 0x0014 */ int m_totalObjects;
            Node();
        }; /* size: 0x0018 */

        using allocator_type = retruxx::allocator<ai::ObjContainer::Node>;
        using size_type = unsigned int;
        using difference_type = int;
        using InnerRecordVector = retruxx::vector<ai::ObjContainer::Node, retruxx::allocator<ai::ObjContainer::Node> >;
        using KeyTypeVector = retruxx::vector<int, retruxx::allocator<int> >;

        struct InnerContainer
        {
            /* 0x0000 */ retruxx::vector<ai::ObjContainer::Node, retruxx::allocator<ai::ObjContainer::Node> > m_records;
            /* 0x0010 */ retruxx::vector<int, retruxx::allocator<int> > m_freePlaces;
            /* 0x0020 */ unsigned int m_size;
            /* 0x0024 */ int m_firstNodeId;
            /* 0x0028 */ int m_lastNodeId;
            InnerContainer(const ai::ObjContainer::InnerContainer&);
            InnerContainer();
            void Clear();
            int Add(ai::Obj* pObj);
            bool AddWithOwnObjId(ai::Obj* pObj, int id);
            unsigned int size() const;
            bool empty() const;
            ai::Obj* GetObjById(int objId);
            void EraseNode(ai::ObjContainer::Node& node, bool deleteObj);
            ai::ObjContainer::Node* _GetNodeById(int nodeId);
        }; /* size: 0x002c */

    private:
        /* 0x0034 */ ai::ObjContainer::InnerContainer m_allObjects;
        /* 0x0060 */ ai::ObjContainer::InnerContainer m_updatingObjects;

        using StrObjIdMap = retruxx::map<CStr, int, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, int> > >;

    private:
        /* 0x008c */ retruxx::map<CStr, int, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, int> > > m_nameToIdMap;

        class const_iterator : public retruxx::_Bidit<ai::ObjContainer::Node, int, ai::ObjContainer::Node const*, ai::ObjContainer::Node const&>
        {
        public:
            using iterator_category = retruxx::bidirectional_iterator_tag;
            using BaseType = retruxx::_Bidit<ai::ObjContainer::Node, int, ai::ObjContainer::Node const*, ai::ObjContainer::Node const&>;
            using difference_type = int;

        public:
            const_iterator(const retruxx::vector<ai::ObjContainer::Node, retruxx::allocator<ai::ObjContainer::Node> >* pRecords, int nodeId);
            const ai::Obj* operator*() const;
            const ai::Obj* operator->() const;
            ai::ObjContainer::const_iterator operator++(int);
            ai::ObjContainer::const_iterator& operator++();
            ai::ObjContainer::const_iterator operator--(int);
            ai::ObjContainer::const_iterator& operator--();
            bool operator==(const ai::ObjContainer::const_iterator& rhs) const;
            bool operator!=(const ai::ObjContainer::const_iterator& rhs) const;
            void _Dec();
            void _Inc();

        protected:
            /* 0x0000 */ int m_nodeId;
            /* 0x0004 */ const retruxx::vector<ai::ObjContainer::Node, retruxx::allocator<ai::ObjContainer::Node> >* m_pRecords;
        }; /* size: 0x0008 */

        class iterator : public ai::ObjContainer::const_iterator
        {
        public:
            using iterator_category = retruxx::bidirectional_iterator_tag;
            using difference_type = int;

        public:
            iterator(retruxx::vector<ai::ObjContainer::Node, retruxx::allocator<ai::ObjContainer::Node> >* pRecords, int nodeId);
            ai::Obj* operator*() const;
            ai::Obj* operator->() const;
            ai::ObjContainer::iterator operator++(int __formal);
            ai::ObjContainer::iterator& operator++();
            ai::ObjContainer::iterator operator--(int);
            ai::ObjContainer::iterator& operator--();
        }; /* size: 0x0008 */

    public:
        unsigned int size() const;
        bool empty() const;
        unsigned int GetNumUpdatingObjects() const;
        ai::ObjContainer::const_iterator begin() const;
        ai::ObjContainer::iterator begin();
        ai::ObjContainer::const_iterator end() const;
        ai::ObjContainer::iterator end();
        ai::ObjContainer::const_iterator updatingBegin() const;
        ai::ObjContainer::iterator updatingBegin();
        ai::ObjContainer::const_iterator updatingEnd() const;
        ai::ObjContainer::iterator updatingEnd();

        enum eSAVE_TYPES
        {
            SAVE_LEVEL = 0,
            SAVE_FULL = 1,
            SAVE_EDITOR = 2,
        };

    public:
        /* 0x0098 */ ai::ObjContainer::eSAVE_TYPES m_SaveType;
        void SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        void Clear(bool bDeleteObjectsPassedToAnotherMap);
        void DeleteAll();
        void Purge();
        void Update(float elapsedTime, unsigned int workTime, bool bCinematic);
        int GetPrototypeId(const char* prototypeName) const;
        CStr GetPrototypeName(int) const;
        int CreateEntityForLoad(int prototypeId, const char* name, int parentId, int objId);
        int CreateNewObject(int prototypeId, const char* name, int parentId, int belongId);
        int CreateNewObjectWithSuspendedPostLoad(int prototypeId, const char* name, int parentId, int belongId);
        void MessageBoxA(int command, int textId, ai::Obj* pPlayer);
        void SetTolerance(int PlayerID1, int PlayerID2, float Tolerance);
        float GetTolerance(int PlayerID1, int PlayerID2) const;
        void IncTolerance(int PlayerID1, int PlayerID2, float IncTolerance);
        ai::Obj* GetEntityByObjId(int objId);
        ai::Obj* GetEntityByObjName(const CStr& name);
        int GetObjIdByObjName(const CStr& name);
        void SetObjName(int objId, const CStr& name);
        CStr GetObjectFullName(const CStr& ObjectName) const;
        m3d::AIParam GetObjList(const char* ClassName, const CVector& Pos, float radius) const;
        bool AddWithOwnObjId(ai::Obj* pObj);
        void AddObjToPostCollideList(ai::Obj* pObj);
        void PostCollide();
        void LoadObjectNamesFromXML(const CStr& fileName);
        void SetGameTimeInt64(int64_t Seconds);
        int64_t GetGameTimeInt64() const;
        float GetGameTimeDiff() const;
        void SetGameTime(int hour, int minute, int day, int month, int year);
        m3d::AIParam GetGameTime() const;
        m3d::AIParam Get24HourTime() const;
        void PauseGameTime();
        void UnpauseGameTime();
        void SaveNodeStatesToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        void LoadNodeStatesFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode);
        void TransferPhysicParamsToSceneGraph();
        void RelinkSceneGraphNodes();
        void LinkGeomsToCollisionCells();
        void UnlinkGeomsFromCollisionCells();
        void RelinkGeomsToCollisionCells();
        int GetNumRemovalsLastFrame() const;
        void PassToMap(const CStr& mapName, const CStr& locationName, int angle, bool bImmediate);
        float GetHeight(float x, float z) const;
        void AddObjToUpdate(ai::Obj* obj);
        void AddObjToNotUpdate(ai::Obj* obj);
        void AddObjIdToRelinkSceneGraphNode(int objId);
        void AddObjIdToRemove(int objId);
        void DumpPhysicInfo(const CStr& fileName) const;
        void Dump();
        ai::GameTime& getGameTime();
        const ai::GameTime& getGameTime() const;
        void DenyCreation();
        void PermitCreation();
        void AllowSave(bool allow);
        bool IsSaveAllowed() const;

    private:
        /* 0x009c */ m3d::CStrHash<CStr> m_ObjectFullNames;
        /* 0x00a8 */ ai::GameTime m_GameTime;
        /* 0x00c0 */ bool m_GameTimePaused;
        /* 0x00c1 */ char Padding_198[3];
        /* 0x00c4 */ unsigned int m_denyCreationCount;
        /* 0x00c8 */ int m_numRemovalsLastFrame;
        /* 0x00cc */ retruxx::vector<int, retruxx::allocator<int> > m_objIdsToUpdate;
        /* 0x00dc */ retruxx::vector<int, retruxx::allocator<int> > m_objIdsToNotUpdate;
        /* 0x00ec */ retruxx::vector<ai::Obj*, retruxx::allocator<ai::Obj*> > m_objectsToPostCollide;
        /* 0x00fc */ retruxx::vector<int, retruxx::allocator<int> > m_objIdsToRelinkSceneGraphNode;
        /* 0x010c */ retruxx::vector<int, retruxx::allocator<int> > m_objIdsToRemove;
        /* 0x011c */ bool m_inPurge;
        /* 0x011d */ bool m_inUpdate;
        /* 0x011e */ bool m_bSaveAllowed;
        int _Add(ai::Obj* pObj);
        void _DeleteObj(ai::Obj*& pObj);
        void _PassToMapAfterFading();
        void _SetObjUpdating(int objId);
        void _SetObjNotUpdating(int objId);
    }; /* size: 0x0120 */

    static_assert(sizeof(ObjContainer) == 0x0120);

    void SetObjects(ObjContainer*);
}
