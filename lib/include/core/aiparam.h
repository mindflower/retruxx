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
    }  // namespace cmn

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
        void Copy(m3d::AIParam const& PParam);
        void ConvertFromString(void* retVal, m3d::eAIParamType ToType) const;
        static int __cdecl CompareInt(void const* v1, void const* v2);
        static int __cdecl CompareStr(void const* v1, void const* v2);

        struct RepEl
        {
            CStr m_prototypeName;
            int m_Amount;
            bool operator==(m3d::AIParam::RepEl const&);
            RepEl(m3d::AIParam::RepEl const&);
            RepEl(CStr const&, int const&);
        };

    protected:
        static int CompareRepEl(void const*, void const*);

    public:
        CStr (*NameFromNum)(m3d::AIParam const*, int) = nullptr;
        int (*NumFromName)(m3d::AIParam const*, CStr&) = nullptr;
        void ReadFromString(CStr const& Str);
        void Clear();
        ~AIParam();
        AIParam(float const& PValue);
        AIParam(Quaternion const&);
        AIParam(CVector const& pos);
        AIParam(CVector2 const& range);
        AIParam(int const& PID);
        AIParam(unsigned int const&);
        AIParam(CStr const& str);
        AIParam();
        AIParam(m3d::AIParam const& PParam);
        AIParam(retruxx::vector<int> const& List);
        bool operator>(m3d::AIParam const& with);
        bool operator<(m3d::AIParam const& with);
        bool operator>=(m3d::AIParam const&);
        bool operator<=(m3d::AIParam const& with);
        bool operator==(m3d::AIParam const& with);
        bool operator!=(m3d::AIParam const& with);
        void Init();
        m3d::AIParam& operator=(float const&);
        m3d::AIParam& operator=(Quaternion const&);
        m3d::AIParam& operator=(CVector const&);
        m3d::AIParam& operator=(CVector2 const&);
        m3d::AIParam& operator=(int const&);
        m3d::AIParam& operator=(unsigned int const& PID);
        m3d::AIParam& operator=(CStr const&);
        m3d::AIParam& operator=(m3d::AIParam const& pparam);
        m3d::AIParam& operator=(retruxx::vector<CStr> const&);
        m3d::AIParam& operator=(retruxx::vector<int> const&);
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
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* OwnNode);
        void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const;
    };
}  // namespace m3d
