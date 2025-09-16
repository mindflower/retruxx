#include <stdexcept>
#include <windows.h>
#include <scene/servers/dataserver.h>
#include <core/ref_ptr.h>
#include <core/ini.h>

namespace m3d
{
    DataServer::Model::Model(void* p, char const* fileName, char const* originalFileName, char const* id) :
        m_ptr(p),
        m_fileName(fileName),
        m_originalFileName(originalFileName),
        m_name(id)
    {
    }

    int DataServer::GetItemByName(char const* id, bool viaMap) const
    {
        if (!id)
        {
            return -1;
        }
        if (viaMap)
        {
            CStr name = id;
            if (!name.empty())
            {
                ::LCMapStringA(0x400u, 0x100u, name.c_str(), name.length() + 1, const_cast<char*>(name.c_str()), name.length() + 1);
            }
            auto const it = m_shRemap.find(name);
            if (it != m_shRemap.end())
            {
                return it->second;
            }
            return -1;
        }
	    for (int i = 0; i<m_models.size(); ++i)
	    {
		    if (!CStr::my_stricmp(id, m_models[i].m_name.c_str()))
		    {
                return i;
		    }
	    }
        return -1;
    }

    int DataServer::RenderShadowVolumesSet(SgNode**, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int DataServer::RenderNodeSet(SgNode**, unsigned, RenderNodeInfo)
    {
        throw std::logic_error("Not implemented");
    }

    void DataServer::RenderTransparents(SgNode**, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void DataServer::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& DataServer::GetNameByItem(int) const
    {
        throw std::logic_error("Not implemented");
    }

    bool DataServer::ReportServerInfo(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int DataServer::WriteItemToXmlNode(int, cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int DataServer::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void DataServer::UpdateItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int DataServer::Release()
    {
        throw std::logic_error("Not implemented");
    }

    int DataServer::GenerateImpostorsIfNeeded()
    {
        throw std::logic_error("Not implemented");
    }

    int DataServer::Init()
    {
        m_valid = true;
        return 1;
    }

    void DataServer::Restore()
    {
        throw std::logic_error("Not implemented");
    }

    void DataServer::UnregisterNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int DataServer::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* root)
    {
        ref_ptr node = file->CreateNode();
        for (root->GetFirstChild(node, "Item"); !node->IsEmpty(); node->GetNextSibling(node, "Item"))
        {
            ServerItem item;
            m3d::SafeStrAttrib(item.m_id, node, "id");
            m3d::SafeStrAttrib(item.m_params, node, "params");
            m3d::SafeStrAttrib(item.m_filename, node, "file");
            m_itemslist.push_back(std::move(item));
        }

        AddItemsList(m_itemslist);
        GenerateItemsRemap();
        m_itemslist.clear();
        return 1;
    }

    DataServer::DataServer()
    {
    }

    int DataServer::SetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void DataServer::GenerateItemsRemap()
    {
        throw std::logic_error("Not implemented");
    }

    CStr DataServer::GetOriginalFileName(int)
    {
        throw std::logic_error("Not implemented");
    }

    DataServer::~DataServer()
    {
        throw std::logic_error("Not implemented");
    }

    void DataServer::RegisterNode(SgNode*)
    {
    }

    void DataServer::Invalidate()
    {
        throw std::logic_error("Not implemented");
    }

    int DataServer::GetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int DataServer::GetNumItems() const
    {
        throw std::logic_error("Not implemented");
    }

    void DataServer::SetError(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool DataServer::IsValid() const
    {
        throw std::logic_error("Not implemented");
    }

    int DataServer::ParseProto(char const* in, Proto* protocol, int* paramsPos)
    {
        auto pos = strchr(in, ':');
        if (strlen(in) >= 3 && pos != 0)
        {
            *paramsPos = pos - in + 1;
            if (!strncmp(in, "new", 3))
            {
                *protocol = PROTO_NEW;
                return 1;
            }
            if (!strncmp(in, "file", 4))
            {
                *protocol = PROTO_FILE;
                return 1;
            }
        }
        m_lastError = "No protocol found";
        return 0;
    }
}
