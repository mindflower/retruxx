#pragma once
#include "stringm3d.h"
#include <iface.h>

namespace m3d
{
    namespace fs
    {
        class IStream;
    }
}

namespace m3d
{
    //TODO: add static functions

    namespace cmn
    {
        class IniFile : public IBase
        {
        public:
        protected:
        private:
        };

        class XmlAttrib : public IBase
        {
        public:
        protected:
        private:
        };

        enum XmlNodeType
        {
            XML_NODE_EMPTY = 0x0,
            XML_NODE_DOCUMENT = 0x1,
            XML_NODE_ELEMENT = 0x2,
            XML_NODE_COMMENT = 0x3,
            XML_NODE_UNKNOWN = 0x4,
            XML_NODE_TEXT = 0x5,
            XML_NODE_DECLARATION = 0x6,
        };

        class XmlNode : public IBase
        {
        public:
            virtual ~XmlNode() = default;
            virtual bool HasChildOrAttribute() const = 0;
            virtual bool AddBeforeChild(XmlNode const*, XmlNode*) = 0;
            virtual char const* GetValue() const = 0;
            virtual bool GetFirstAttribute(XmlAttrib*) const = 0;
            virtual bool RemoveAttribute(char const*) = 0;
            virtual bool IsEmpty() const = 0;
            virtual bool GetPrevRelative(XmlNode*, char const*) const = 0;  //GetPrevSibling
            virtual bool IsOfType(XmlNodeType) const = 0;
            virtual bool AddAfterChild(XmlNode const*, XmlNode*) = 0;
            virtual void SetValue(char const*) = 0;
            virtual bool GetNextRelative(XmlNode*, char const*) const = 0;  //GetNextSibling
            virtual bool GetParent(XmlNode*) const = 0;
            virtual void GetAttributeMbcsSafe(char const*, char**, int*) const = 0;
            virtual bool AddChild(XmlNode*) = 0;
            virtual bool SetAttribute(char const*, char const*) = 0;
            virtual bool RemoveChild(XmlNode*) = 0;
            virtual XmlAttrib* CreateAttribute() const = 0;
            virtual bool GetFirstNestling(XmlNode*, char const*) const = 0; //GetFirstChild
            virtual char const* GetAttribute(char const*) const = 0;
            virtual bool GetLastNestling(XmlNode*, char const*) const = 0;     //GetLastNestling
        };

        class XmlFile : public IBase
        {
        public:
            virtual ~XmlFile() = default;
            virtual bool AddBeforeChild(XmlNode const*, XmlNode*) = 0;
            virtual void SetHeader(char const*, char const*, char const*) = 0;
            virtual bool AddAfterChild(XmlNode const*, XmlNode*) = 0;
            virtual int Write(fs::IStream&) = 0;
            virtual void GetHeader(char**, char**, char**) = 0;
            virtual bool AddChild(XmlNode*) = 0;
            virtual bool RemoveChild(XmlNode*) = 0;
            virtual bool GetFirstNestling(XmlNode*, char const*) const = 0;
            virtual int Read(fs::IStream&) = 0;
            virtual XmlNode* CreateNode(XmlNodeType, char const*) const = 0;
            virtual bool GetLastNestling(XmlNode*, char const*) const = 0;
            virtual char const* GetError() = 0;
        };
    }

    cmn::XmlFile* ReadXmlFile(char const* filename, CStr* errorStr);
    int SafeStrAttrib(CStr&, cmn::XmlNode const*, char const*);
}
