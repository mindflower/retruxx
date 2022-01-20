#pragma once
#include "obj.h"
#include <core/containers.h>

class CVector;

namespace ai
{
    class ObjContainer;

    class GameTime
    {
    public:
        __int64 asInt64() const;
        void operator+=(float);
        m3d::AIParam asAIParam() const;
        float GameDiff(ObjContainer const*) const;
        GameTime(int, int, int, int, int);
        GameTime(__int64);
        GameTime();
        void setInt64(__int64);
        virtual void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        void setExpanded(int, int, int, int, int);
        float Diff() const;
        m3d::AIParam asAIParam24Hour() const;
        virtual void SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;

    private:
        //ai::GameTime_vtbl* __vftable /*VFT*/;
        __int64 m_milliSeconds = 0;
        __int64 m_milliSeconds0 = 0;
    };

    class ObjContainer :  public m3d::Object
    {
    public:
        enum eSAVE_TYPES
        {
            SAVE_LEVEL = 0x0,
            SAVE_FULL = 0x1,
            SAVE_EDITOR = 0x2,
        };

        class Node
        {
        public:
            Node();

        public:
            int m_id = -1;
            int m_prevId = -1;
            int m_nextId = -1;
            Obj* m_value = nullptr;
            bool m_isValid = false;
            int m_totalObjects = 0;
        };

        class InnerContainer
        {
        public:
            InnerContainer();
            void EraseNode(Node&, bool);
            int Add(Obj*);
            bool AddWithOwnObjId(Obj*, int);
            bool empty() const;
            Obj* GetObjById(int);
            void Clear();
            unsigned int size() const;

        private:
            Node* _GetNodeById(int);

        private:
            std::vector<Node> m_records;
            std::vector<int> m_freePlaces;
            unsigned int m_size = 0;
            int m_firstNodeId = -1;
            int m_lastNodeId = -1;
        };

        class const_iterator// : public std::_Bidit<Node, int, Node const*, Node const&>
        {
        public:
            const_iterator(std::vector<Node> const*, int);
            void _Inc();
            Obj const* operator*() const;
            Obj const* operator->() const;
            bool operator==(const_iterator const&) const;
            bool operator!=(const_iterator const&) const;
            class const_iterator& operator++();

        private:
            int m_nodeId;
            const std::vector<Node>* m_pRecords;
        };

        class iterator : public const_iterator
        {
        public:
            iterator(std::vector<Node>*, int);
            Obj* operator->() const;
            iterator operator++(int);
            iterator& operator++();
            Obj* operator*() const;
        };

    public:
        Obj * GetEntityByObjName(CStr const &);
        void Purge();
        iterator updatingEnd();
        virtual ~ObjContainer();
        CStr GetObjectFullName(CStr const &) const ;
        void RelinkGeomsToCollisionCells();
        void AddObjToUpdate(Obj *);
        virtual m3d::Object * Clone();
        void PermitCreation();
        int CreateNewObjectWithSuspendedPostLoad(int,char const *,int,int);
        iterator updatingBegin();
        void LoadNodeStatesFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *);
        bool IsSaveAllowed() const ;
        void PauseGameTime();
        void DeleteAll();
        void PassToMap(CStr const &,CStr const &,int,bool);
        float GetHeight(float,float) const ;
        static m3d::Class * GetBaseClass();
        int GetPrototypeId(char const *) const ;
        void Dump();
        void SetGameTimeInt64(__int64);
        int GetObjIdByObjName(CStr const &);
        float GetTolerance(int,int) const ;
        void SetGameTime(int,int,int,int,int);
        static Object * CreateObject();
        void TransferPhysicParamsToSceneGraph();
        void UnpauseGameTime();
        void UnlinkGeomsFromCollisionCells();
        void SetTolerance(int,int,float);
        bool AddWithOwnObjId(Obj *);
        void SetObjName(int,CStr const &);
        GameTime & getGameTime();
        m3d::AIParam GetGameTime() const ;
        void AddObjToPostCollideList(Obj *);
        void DumpPhysicInfo(CStr const &) const ;
        __int64 GetGameTimeInt64() const ;
        unsigned int GetNumUpdatingObjects() const ;
        bool empty() const ;
        Obj * GetEntityByObjId(int);
        void DenyCreation();
        void LoadObjectNamesFromXML(CStr const &);
        m3d::AIParam Get24HourTime() const ;
        int CreateEntityForLoad(int,char const *,int,int);
        void MessageBoxA(int,int,Obj *);
        void AllowSave(bool);
        float GetGameTimeDiff() const ;
        iterator begin();
        const_iterator begin() const ;
        int GetNumRemovalsLastFrame() const ;
        void Update(float,unsigned int,bool);
        int CreateNewObject(int,char const *,int,int);
        void Clear(bool);
        virtual m3d::Class * GetClass() const ;
        void SaveNodeStatesToXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        void SaveToXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        const_iterator end() const ;
        iterator end();
        void LinkGeomsToCollisionCells();
        void RelinkSceneGraphNodes();
        void AddObjIdToRemove(int);
        unsigned int size() const ;
        m3d::AIParam GetObjList(char const *,CVector const &,float) const ;
        void IncTolerance(int,int,float);
        void AddObjToNotUpdate(Obj *);
        void PostCollide();
        void AddObjIdToRelinkSceneGraphNode(int);

    protected:
        ObjContainer();
        ObjContainer(ObjContainer const &);

    private:
        void _SetObjNotUpdating(int);
        int _Add(Obj *);
        void _PassToMapAfterFading();
        void _SetObjUpdating(int);
        void _DeleteObj(Obj * &);

    public:
        RT_CLASS_DECLARE(ObjContainer);

    private:
        InnerContainer m_allObjects;
        InnerContainer m_updatingObjects;
        std::map<CStr,int> m_nameToIdMap;
        eSAVE_TYPES m_SaveType = SAVE_LEVEL;
        m3d::CStrHash<CStr> m_ObjectFullNames;
        GameTime m_GameTime;
        bool m_GameTimePaused = false;
        unsigned int m_denyCreationCount = 0;
        int m_numRemovalsLastFrame;
        std::vector<int> m_objIdsToUpdate;
        std::vector<int> m_objIdsToNotUpdate;
        std::vector<Obj *> m_objectsToPostCollide;
        std::vector<int> m_objIdsToRelinkSceneGraphNode;
        std::vector<int> m_objIdsToRemove;
        bool m_inPurge = false;
        bool m_inUpdate = false;
        bool m_bSaveAllowed = true;
    };

    void SetObjects(ObjContainer*);
}
