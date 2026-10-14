#pragma once
#include "stringm3d.h"
#include "retruxx/common.h"

struct CVector;
struct CVector2;
struct Quaternion;

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }

    enum eAIParamType
    {
        AIPARAM_UNDEFINE = 0x0,
        AIPARAM_VECTOR = 0x1,
        AIPARAM_QUATERNION = 0x2,
        AIPARAM_ID = 0x3,
        AIPARAM_FLOAT = 0x4,
        AIPARAM_STRING = 0x5,
        AIPARAM_ID_LIST = 0x6,
        AIPARAM_STRING_LIST = 0x7,
        AIPARAM_RANGE = 0x8,
    };

    class AIParam
    {
    protected:
        union
        {
            struct
            {
                float x;
                float y;
                float z;
                float w;
            };
            int id = 0;
            float Value;
            retruxx::vector<CStr>* m_NameList;
            retruxx::vector<int>* m_NumList;
            CStr* m_Str;
            // retruxx adaptation: an object pointer travelling as an ID (see FromPointer); the slot is
            // pointer-sized already, the shipped build read it back through id.
            void* m_ptr;
        };
        m3d::eAIParamType Type = AIPARAM_UNDEFINE;
        void Detach();
        void Copy(const m3d::AIParam& PParam);
        void ConvertFromString(void* retVal, m3d::eAIParamType ToType) const;
        static int __cdecl CompareInt(const void* v1, const void* v2);
        static int __cdecl CompareStr(const void* v1, const void* v2);

        struct RepEl
        {
            CStr m_prototypeName;
            int m_Amount;
            bool operator==(const m3d::AIParam::RepEl&);
            RepEl(const m3d::AIParam::RepEl&);
            RepEl(const CStr&, const int&);
        };

    protected:
        static int __fastcall CompareRepEl(const void*, const void*);

    public:
        CStr(*NameFromNum)(const m3d::AIParam*, int) = nullptr;
        int (*NumFromName)(const m3d::AIParam*, CStr&) = nullptr;
        void ReadFromString(const CStr& Str);
        void Clear();
        ~AIParam();
        AIParam(const float& PValue);
        AIParam(const Quaternion&);
        AIParam(const CVector& pos);
        AIParam(const CVector2& range);
        AIParam(const int& PID);
        AIParam(const unsigned int&);
        AIParam(const CStr& str);
        AIParam();
        AIParam(const m3d::AIParam& PParam);
        AIParam(const retruxx::vector<int>& List);
        bool operator>(const m3d::AIParam& with);
        bool operator<(const m3d::AIParam& with);
        bool operator>=(const m3d::AIParam&);
        bool operator<=(const m3d::AIParam& with);
        bool operator==(const m3d::AIParam& with);
        bool operator!=(const m3d::AIParam& with);
        void Init();
        m3d::AIParam& operator=(const float&);
        m3d::AIParam& operator=(const Quaternion&);
        m3d::AIParam& operator=(const CVector&);
        m3d::AIParam& operator=(const CVector2&);
        m3d::AIParam& operator=(const int&);
        m3d::AIParam& operator=(const unsigned int& PID);
        m3d::AIParam& operator=(const CStr&);
        m3d::AIParam& operator=(const m3d::AIParam& pparam);
        m3d::AIParam& operator=(const retruxx::vector<CStr>&);
        m3d::AIParam& operator=(const retruxx::vector<int>&);
        CVector GetAsVector() const;
        Quaternion GetAsQuaternion() const;
        CVector2 GetAsRange() const;
        int GetAsID() const;
        // retruxx adaptation: the shipped build packed a pointer into the id of an ID parameter in a
        // few places (the local chart's object info); these carry it at full width.
        static m3d::AIParam FromPointer(void* ptr);
        void* GetAsPointer() const;
        float GetAsFloat() const;
        retruxx::vector<int> GetAsIdList() const;
        retruxx::vector<CStr> GetAsStringList() const;
        void SetType(m3d::eAIParamType ParamType);
        m3d::eAIParamType GetType() const;
        CStr ToStr() const;
        CStr GetAsStr() const;
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* OwnNode);
        void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const;
    };
}
