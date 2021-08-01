#include <skelmodel.h>
#include <core/kernel.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>

namespace m3d
{
    void AnimatedModel::CreateTexFileMapping()
    {
        scoped_ptr fileStream = g_Kernel->GetFileServer().CreateFileStream();
        if (fileStream->Open("data/models/ModelTextures.xml", fs::IStream::OPEN_READ))
        {
            ref_ptr xml = g_Kernel->CreateXmlFile();
            if (xml->Read(*fileStream))
            {
                fileStream->Close();

                ref_ptr texturesNode = xml->CreateNode(cmn::XmlNodeType::XML_NODE_EMPTY, nullptr);
                ref_ptr fileNode = xml->CreateNode(cmn::XmlNodeType::XML_NODE_EMPTY, nullptr);
                xml->GetFirstNestling(texturesNode, "Textures");
                xml->GetFirstNestling(fileNode, "file");
                //TODO: check this
                for (; !fileNode->IsEmpty(); fileNode->GetNextRelative(fileNode, "file"))
                {
                    CStr name;
                    CStr path;
                    SafeStrAttrib(name, fileNode, "name");
                    SafeStrAttrib(path, fileNode, "path");
                    m_textureFiles.emplace(std::move(name), std::move(path));
                }
            }
            else
            {
                LOG("Error: Can't read file: data/models/ModelTextures.xml", LOG_INFO);
            }
        }
        else
        {
            LOG("Error: Can't open file: data/models/ModelTextures.xml", LOG_INFO);
        }
    }
}
