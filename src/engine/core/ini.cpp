#include <sstream>
#include <stdexcept>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>

namespace m3d
{
    cmn::XmlFile* ReadXmlFile(char const* filename, CStr* errorStr)
    {
        scoped_ptr fileStream = g_Kernel->GetFileServer().CreateFileStream();
        if (fileStream->Open(filename, fs::IStream::OPEN_READ))
        {
            //TODO: ref_ptr?
            auto* xmlFile = g_Kernel->CreateXmlFile();
            xmlFile->Read(*fileStream);
            fileStream->Close();
            if (xmlFile->GetError() != nullptr)
            {
                xmlFile->DecRef();
                xmlFile = nullptr;
                if (errorStr != nullptr)
                {
                    *errorStr = CStr("ReadXmlFile: Cannot parse file ") + filename;
                }
            }
            return xmlFile;
        }
        if (errorStr != nullptr)
        {
            *errorStr = CStr("ReadXmlFile: Cannot open file ") + filename;
        }
        return nullptr;
    }

    int SafeStrAttrib(CStr& v, cmn::XmlNode const* node, char const* attrName)
    {
        if (node->IsEmpty())
        {
            return 0;
        }
        auto const* attr = node->GetAttribute(attrName);
        if (attr == nullptr)
        {
            return 0;
        }
        v = attr;
        return 1;
    }

    void Tokenize(CStr const* str, std::vector<CStr>& tokens, char const* chars)
    {
        //TODO: check this
        if (str && !str->empty())
        {
            tokens.clear();
            size_t start;
            size_t end = 0;
            while ((start = str->find_first_not_of(chars, end)) != CStr::npos)
            {
                end = str->find(chars, start);
                tokens.push_back(str->substr(start, end - start));
            }
        }
    }
}
