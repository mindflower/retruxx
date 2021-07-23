#pragma once

class CMediaType :  public _AMMediaType
{
public:
    void ResetFormatBuffer();
    class CMediaType & operator=(struct _AMMediaType const &);
    class CMediaType & operator=(class CMediaType const &);
    unsigned char * AllocFormatBuffer(unsigned long);
    long Set(class CMediaType const &);
    long Set(struct _AMMediaType const &);
    int IsFixedSize() const ;
    CMediaType();
    CMediaType(struct _GUID const *);
    CMediaType(struct _AMMediaType const &,long *);
    CMediaType(class CMediaType const &,long *);
    struct _GUID const * Type() const ;
    unsigned long GetSampleSize() const ;
    void SetSampleSize(unsigned long);
    int IsPartiallySpecified() const ;
    int IsValid() const ;
    unsigned char * ReallocFormatBuffer(unsigned long);
    int SetFormat(unsigned char *,unsigned long);
    void SetTemporalCompression(int);
    void SetFormatType(struct _GUID const *);
    unsigned char * Format() const ;
    void SetVariableSize();
    void InitMediaType();
    int operator!=(class CMediaType const &) const ;
    void SetSubtype(struct _GUID const *);
    int operator==(class CMediaType const &) const ;
    struct _GUID const * Subtype() const ;
    ~CMediaType();
    struct _GUID const * FormatType() const ;
    int MatchesPartial(class CMediaType const *) const ;
    void SetType(struct _GUID const *);
protected:
private:
};
