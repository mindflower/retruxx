#pragma once

class CGenericList<CDeferredCommand> :  public CBaseList
{
public:
protected:
private:
    GetNext(__POSITION * &);
    CGenericList<CDeferredCommand>(char *);
    GetHeadPosition();
    AddTail(CDeferredCommand *);
    AddBefore(__POSITION *,CDeferredCommand *);
    Get(__POSITION *);
    GetCount();
    Remove(__POSITION *);
};

namespace CBaseList
{
    class CNode
    {
    public:
        void SetData(void *);
        void * GetData() const ;
        CNode();
        void SetPrev(class CNode *);
        void SetNext(class CNode *);
        class CNode * Prev() const ;
        class CNode * Next() const ;
    protected:
    private:
        CNode *m_pPrev;
        CNode *m_pNext;
        void *m_pObject;
    };
}

namespace CBaseList
{
    class CNodeCache
    {
    public:
        ~CNodeCache();
        CNodeCache(int);
        void AddToCache(CNode *);
        CNode * RemoveFromCache();
    protected:
    private:
        int m_iCacheSize;
        int m_iUsed;
        CNode *m_pHead;
    };
}

class CGenericList<CBasePin> :  public CBaseList
{
public:
protected:
private:
    CGenericList<CBasePin>(char *);
    Find(CBasePin *);
    AddTail(CBasePin *);
    AddTail(CBasePin *);
};

class CBaseList
{
public:
    int AddHead(class CBaseList *);
    int AddAfter(struct __POSITION *,class CBaseList *);
    int GetCountI() const ;
    int AddBefore(struct __POSITION *,class CBaseList *);
    void Reverse();
    struct __POSITION * GetHeadPositionI() const ;
    CBaseList(char *,int);
    CBaseList(char *);
    struct __POSITION * GetTailPositionI() const ;
    struct __POSITION * Prev(struct __POSITION *) const ;
    void RemoveAll();
    int MoveToHead(struct __POSITION *,class CBaseList *);
    struct __POSITION * Next(struct __POSITION *) const ;
    ~CBaseList();
    int AddTail(class CBaseList *);
    int MoveToTail(struct __POSITION *,class CBaseList *);
protected:
    struct __POSITION * AddAfterI(struct __POSITION *,void *);
    struct __POSITION * AddHeadI(void *);
    void * RemoveTailI();
    void * RemoveI(struct __POSITION *);
    struct __POSITION * FindI(void *) const ;
    struct __POSITION * AddBeforeI(struct __POSITION *,void *);
    void * RemoveHeadI();
    void * GetNextI(struct __POSITION * &) const ;
    void * GetI(struct __POSITION *) const ;
    struct __POSITION * AddTailI(void *);
private:
    CBaseList::CNode *m_pFirst;
    CBaseList::CNode *m_pLast;
    int m_Count;
    CBaseList::CNodeCache m_Cache;
};
