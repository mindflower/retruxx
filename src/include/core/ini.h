#pragma once
#include <iface.h>

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

        class XmlFile : public IBase
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
        protected:
        private:
        };

        class XmlAttrib : public IBase
        {
        public:
        protected:
        private:
        };
    }
}
