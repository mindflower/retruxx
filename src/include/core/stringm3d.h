#pragma once
#include <string>
#include <math/quaternion.h>
#include <math/vector2.h>

using CStr = std::string;


void UnifyFileName(CStr& fileName);
void UnifyFileName0(CStr& fileName);
CStr DirectoryFromFileName(CStr const& source);
CStr NameFromFileName(CStr const& source);

void toLower(CStr& str);

namespace m3d
{
    namespace fs
    {
        class IStream;
    }
}

//string class with restricted fields for dlls
class CStr2
{
public:
    class ZeroCharHolder
    {
        char m_zeroChar;

    public:
        ZeroCharHolder();
        operator char* ();
       
    };

public:
    char* m_charPtr;
    int m_allocSz;
    ZeroCharHolder ZERO;

private:
    void cleanup();
    void realloc(int);

public:
    static int my_strcmp(char const*, char const*);
    static int my_stricmp(char const*, char const*);

public:
    CStr2(Quaternion const&);
    CStr2(CVector2 const&);
    CStr2(CVector const&);
    CStr2(double);
    CStr2(float);
    CStr2(unsigned long long);
    CStr2(long long);
    CStr2(unsigned long);
    CStr2(unsigned int);
    CStr2(int);
    CStr2(unsigned char);
    CStr2(char const*, int);
    CStr2(char const*);
    CStr2(char, int);
    CStr2(CStr2 const&);
    CStr2();

    ~CStr2();

    int length() const;
    bool empty() const;
    void erase();

    char& operator[](int);
    char const& operator[](int) const;

    char const* c_str() const;
    CStr& operator+=(CStr const&);
    char* getHashCode();            //TODO: what the heck
    void toLower(unsigned long);
    void toUpper(unsigned long);
    void FirstCharToLower(unsigned long);
    void FirstCharToUpper(unsigned long);
    int findOneOf(char const*, int) const;
    int find(char, int) const;
    int rfind(char) const;
    CStr2 substr(int, int) const;
    int findsubstr(char const*, int) const;
    int del(int, int);
    int format(char const*, ...);
    static CStr2 format_(char const*, ...);
    int Write(m3d::fs::IStream&);
    int Read(m3d::fs::IStream&);

    //CStr2& operator=(CStr2 const&);
    //bool operator==(CStr2 const&) const;
    //CStr2& operator+(CStr2 const&);
    //CStr2& operator+(int);
};

