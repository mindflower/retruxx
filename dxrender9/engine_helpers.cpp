// The engine helpers of core/ini.h the driver uses (device.cpp reads the device compatibility
// database with ReadXmlFile/SafeStrAttrib/SafeClrAttrib, the shader code tokenizes with Tokenize).
// In the original they were part of the statically linked engine. retruxx defines them in
// lib/engine/core/ini.cpp, which cannot be compiled into the DLL because it also carries the XML
// file implementation (TinyXML); these are copies of that file's definitions. They reach the
// executable's file server and XML factory through the kernel the executable handed over
// (m3d::g_Kernel, defined in main.cpp).
#include <core/ini.h>
#include <core/kernel.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>

#include <cstring>

namespace m3d
{
    cmn::XmlFile* ReadXmlFile(char const* filename, CStr* errorStr)
    {
        scoped_ptr fileStream = g_Kernel->GetFileServer().CreateFileStream();
        if (fileStream->Open(filename, fs::IStream::OPEN_READ))
        {
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

    bool SafeClrAttrib(unsigned& clr, m3d::cmn::XmlNode const* node, char const* attrib)
    {
        CStr str;
        if (SafeStrAttrib(str, node, attrib) && !str.empty())
        {
            clr = std::stoul(str.c_str(), nullptr, 16);
            return true;
        }
        return false;
    }

    // orig 0x22fd0 core/stringm3d.inl:207
    void Tokenize(CStr const& str, retruxx::vector<CStr>& tokens, char const* chars)
    {
        if (!str.empty())
        {
            tokens.clear();
            auto temp = new char[str.length() + 1];
            strncpy(temp, str.c_str(), str.length() + 1);
            for (auto i = strtok(temp, chars); i; i = strtok(nullptr, chars))
            {
                tokens.push_back(i);
            }
            delete[] temp;
        }
    }
}  // namespace m3d
