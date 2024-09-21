#pragma once
#include "stringm3d.h"
#include <vector>

class CVector;
class CVector2;
class Quaternion;

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
    public:
        class RepEl
        {
        private:
            CStr m_prototypeName;
            int m_Amount;
        };

    public:
        static int CompareInt(void const*, void const*);
        static int CompareStr(void const*, void const*);

    public:
        AIParam() = default;
        AIParam(AIParam const& param);
        AIParam(CVector const& pos);
        AIParam(CVector2 const& range);
        AIParam(std::vector<int> const& list);
        AIParam(CStr const& str);
        AIParam(int const& pid);
        AIParam(float const& value);
        AIParam(Quaternion const& pos);
        ~AIParam();

        int GetAsID() const;
        std::vector<int> GetAsIdList() const;
        CVector2 GetAsRange() const;
        CStr GetAsStr() const;
        CVector GetAsVector() const;
        void SaveToXML(cmn::XmlFile*, cmn::XmlNode*) const;
        std::vector<CStr> GetAsStringList() const;
        void Clear();
        void ReadFromString(CStr const&);
        bool operator==(AIParam const&);
        bool operator<=(AIParam const&);
        bool operator!=(AIParam const&);
        bool operator<(AIParam const&);
        AIParam& operator=(CVector2 const&);
        AIParam& operator=(CVector const&);
        AIParam& operator=(CStr const&);
        AIParam& operator=(AIParam const&);
        AIParam& operator=(std::vector<CStr> const&);
        AIParam& operator=(float const&);
        AIParam& operator=(std::vector<int> const&);
        AIParam& operator=(unsigned int const&);
        AIParam& operator=(Quaternion const&);
        AIParam& operator=(int const&);
        bool operator>(class AIParam const&);
        void Init();
        void LoadFromXML(cmn::XmlFile*, cmn::XmlNode const*);
        float GetAsFloat() const;
        eAIParamType GetType() const;
        Quaternion GetAsQuaternion() const;
        CStr ToStr() const;
        void SetType(eAIParamType);

    protected:
        void Copy(AIParam const& param);
        void ConvertFromString(void*, eAIParamType) const;
        void Detach();

    private:
        float y = 0;
        float z = 0;
        float w = 0;
        eAIParamType Type = AIPARAM_UNDEFINE;
        //CStr* (__fastcall* NameFromNum)(CStr* result, const m3d::AIParam*, int);
        //int(__fastcall* NumFromName)(const m3d::AIParam*, CStr*);
        //TODO: union initialization
        union
        {
            int id = 0;
            float x;
            float Value;
            std::vector<CStr>* m_NameList;
            std::vector<int>* m_NumList;
            CStr* m_Str;
        };
    };
}
