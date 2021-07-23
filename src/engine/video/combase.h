#pragma once

class CUnknown :  public INonDelegatingUnknown, CBaseObject
{
public:
    CUnknown(char const *,struct IUnknown *);
    CUnknown(char *,struct IUnknown *,long *);
    virtual unsigned long __stdcall NonDelegatingRelease();
    struct IUnknown * GetOwner() const ;
    virtual ~CUnknown();
    virtual unsigned long __stdcall NonDelegatingAddRef();
    virtual long __stdcall NonDelegatingQueryInterface(struct _GUID const &,void * *);
protected:
private:
    IUnknown *const m_pUnknown;
    volatile int m_cRef;
};
