#pragma once
#include "stringm3d.h"
#include "thirdparty/injecttools.h"
#include "thirdparty/containers.h"

#define RT_CLASS_LOCAL(cl) &cl::m_class##cl
#define RT_CLASS_DECLARE(cl) static m3d::Class m_class##cl
#define RT_CLASS_DEFINE(cl) m3d::Class cl::m_class##cl {#cl, sizeof(cl), CreateObject, GetBaseClass, -1, _exports_##cl}
#define RT_CLASS_INLINE_DECLARE(cl) static inline m3d::Class m_class##cl {#cl}

#define RT_CLASS_EXPORTS_BEGIN(cl) m3d::ExportInfo _exports_##cl[] {
#define RT_CLASS_EXPORTS_END m3d::ExportInfo{}}
#define RT_CLASS_EXPORT(cl, type, funcName, retVal, params, desc) m3d::ExportInfo{#funcName, type, _export_##cl##_##funcName, nullptr, retVal, params, desc},
#define RT_CLASS_EXPORT_METHOD_DEFINE(cl, funcName) int _export_##cl##_##funcName##(m3d::Context* context)

#undef GetFirstChild
#undef GetFirstSibling
#undef GetLastSibling
#undef GetNextSibling
#undef GetPrevSibling

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }

    struct ExportInfo;
    class Object;
    class AIParam;

    class Context
    {
    public:
        virtual ~Context() = default;
        virtual void pushObject(Object*) = 0;
        virtual void pushAIParam(AIParam const&) = 0;
        virtual int asInt(int) = 0;
        virtual bool asBool(int) = 0;
        virtual Object* asObject(int, char const*) = 0;
        virtual int countArgs() = 0;
        virtual float asFloat(int) = 0;
        virtual AIParam& asAIParam(int) = 0;
        virtual void pushBool(bool) = 0;
        virtual char const* asString(int) = 0;
        virtual void pushQuaternion(Quaternion const&) = 0;
        virtual void pushInt(int) = 0;
        virtual void pushFloat(float) = 0;
        virtual Quaternion& asQuaternion(int) = 0;
        virtual void pushString(char const*) = 0;
        virtual CVector& asVector(int) = 0;
        virtual void pushVector(CVector const&) = 0;
    };

    //IMPORTANT: fields and members order is strict
    struct Class
    {
        const char* m_className = nullptr;
        int m_classSize =0 ;
        Object* (__fastcall* m_fnCreateObject)() = nullptr;
        Class* (__fastcall* m_fnGetBaseClass)() = nullptr;
        int m_index = 0;
        ExportInfo* m_lExports = nullptr;
        void* m_scriptHandle = nullptr;

    public:
        bool IsKindOf(char const*) const;
        bool IsKindOf(Class const*) const;
        Object* NewInstance() const;
    };

    //IMPORTANT: fields and members order is strict
    class RefCountedBase
    {
    public:
        RefCountedBase();
        virtual ~RefCountedBase() = default;
        int IncRef();
        int DecRef();
        int GetRefCount() const;

    private:
        /* 0x0004 */ int m_refCount = 0;
    }; /* size: 0x0008 */
    static_assert(sizeof(RefCountedBase) == 0x0008);

    //IMPORTANT: fields and members order is strict c
    class Object : public RefCountedBase
    {
    public:
        virtual m3d::Object* Clone() /* 0x04 */;
        virtual int ReadFromXmlNode(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* node) /* 0x08 */;
        virtual int ReadFromXmlNodeAfterAdd(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) /* 0x0c */;
        virtual int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* writeTo) /* 0x10 */;
        bool GetPersistance() const;
        void SetPersistance(bool per);
        virtual int SetProperty(unsigned int propId, void* prop) /* 0x14 */;
        virtual int GetProperty(unsigned int propId, void* prop) const /* 0x18 */;
        virtual int GetPropertiesList(retruxx::set<unsigned int>& properties) const /* 0x1c */;
        m3d::Object* GetParent() const;
        m3d::Object* GetFirstChild() const;
        m3d::Object* GetLastChild() const;
        m3d::Object* GetNextSibling() const;
        m3d::Object* GetPrevSibling() const;
        int GetNumChildren() const;
        m3d::Object* GetChildByName(const CStr& str) const;
        bool IsDirectChild(const m3d::Object* w) const;
        bool IsChildOf(const m3d::Object* wnd) const;
        virtual int AddChild(m3d::Object* node) /* 0x20 */;
        int LinkChildAtHead(m3d::Object* node);
        int LinkChildAtTail(m3d::Object* node);
        int UnlinkChild(m3d::Object* node);
        virtual int RemoveChild(m3d::Object* node) /* 0x24 */;
        int RemoveAllChildren();
        void MoveChildToFirstPosition(m3d::Object* wnd);
        void MoveChildToLastPosition(m3d::Object* wnd);
        const char* GetName() const;
        void SetName(const CStr& str);
        void SetChildDirty(bool d);
        bool GetChildDirty() const;

    protected:
        /* 0x0008 */ CStr m_name;
        /* 0x0014 */ bool m_persistant = true;
        /* 0x0015 */ bool m_isChildDirty = false;
        /* 0x0016 */ char Padding_3[2];

    private:
        /* 0x0018 */ m3d::Object* m_parent = nullptr;
        /* 0x001c */ m3d::Object* m_firstChild = nullptr;
        /* 0x0020 */ m3d::Object* m_lastChild = nullptr;
        /* 0x0024 */ m3d::Object* m_nextSibling = nullptr;
        /* 0x0028 */ m3d::Object* m_prevSibling = nullptr;
        /* 0x002c */ int m_numChildren = 0;

    protected:
        m3d::Object* ChildNodeFromXmlNode(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode);

    public:
        m3d::Object* ChildNodeFromXmlFile(const char* extStr);
        virtual int IncWeakRef() /* 0x28 */;
        virtual int DecWeakRef() /* 0x2c */;
        virtual int GetWeakRefCount() /* 0x30 */;
        /* 0x0030 */ void* m_scriptHandle = nullptr;

        static m3d::Class m_classObject;
        virtual m3d::Class* GetClass() const /* 0x34 */;
        virtual const char* GetClassNameA() const /* 0x38 */;
        bool IsKindOf(const char* className) const;
        bool IsKindOf(const m3d::Class* object) const;
        static m3d::Class* __fastcall GetBaseClass();
        static m3d::Object* __fastcall CreateObject();

    protected:
        Object(const m3d::Object& clazz);
        Object();
    }; /* size: 0x0034 */

    enum eExportType
    {
        METHOD = 0x0,
        NATIVE_METHOD = 0x1,
    };

    struct ExportInfo
    {
        const char* name = nullptr;
        eExportType type = METHOD;
        void* addr1 = nullptr;
        void* addr2 = nullptr;
        const char* returns = nullptr;
        const char* params = nullptr;
        const char* desc = nullptr;
    };

}
