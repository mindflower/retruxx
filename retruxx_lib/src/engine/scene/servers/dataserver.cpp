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
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DataServer::RenderNodeSet(SgNode**, unsigned, RenderNodeInfo)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DataServer::RenderTransparents(SgNode**, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DataServer::PostLoad()
    {
    }

    CStr const& DataServer::GetNameByItem(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool DataServer::ReportServerInfo(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DataServer::WriteItemToXmlNode(int, cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DataServer::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DataServer::UpdateItem(int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DataServer::Release()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DataServer::GenerateImpostorsIfNeeded()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DataServer::Init()
    {
        m_valid = true;
        return 1;
    }

    void DataServer::Restore()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DataServer::UnregisterNode(SgNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DataServer::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* root)
    {
        ref_ptr node = file->CreateNode();
        for (root->GetFirstChild(node, "Item"); !node->IsEmpty(); node->GetNextSibling(node, "Item"))
        {
            ServerItem item;
            m3d::SafeStrAttrib(item.m_id, node, "id");
            m3d::SafeStrAttrib(item.m_params, node, "params");

            CStr file;
            m3d::SafeStrAttrib(file, node, "file");

            item.m_filename = "file:" + file;
            m_itemslist.push_back(std::move(item));
        }

        AddItemsList(m_itemslist);
        GenerateItemsRemap();
        m_itemslist.clear();
        return 1;
    }

    RenderNodeInfo::RenderNodeInfo(m3d::RenderNodeType t)
    {
        this->rnt = t;
        this->isCullInverted = 0;
        this->isPrimaryRender = 0;
        this->isUseImpostors = 1;
    }

    RenderNodeInfo::RenderNodeInfo()
    {
        this->rnt = RNT_SIMPLE;
        this->isCullInverted = 0;
        this->isPrimaryRender = 0;
        this->isUseImpostors = 1;
    }

    DataServer::DataServer()
    {
    }

    int DataServer::SetItemProperty(int id, int prop, void* src)
    {
        if (prop > 6)
            return 0;
        this->m_models[id].m_additionalData[prop] = *(unsigned*)src;
        return 1;
    }

    void DataServer::GenerateItemsRemap()
    {
        m_shRemap.clear();
        for (int i = 0; i < m_models.size(); ++i)
        {
            auto name = m_models[i].m_name;
            if (!name.empty())
            {
                auto len = name.length() + 1;
                LCMapStringA(0x400u, 0x100u, name.c_str(), len, &name[0], len);
            }
            m_shRemap[name] = i;
        }
    }

    CStr DataServer::GetOriginalFileName(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DataServer::~DataServer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DataServer::RegisterNode(SgNode*)
    {
    }

    void DataServer::Invalidate()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DataServer::GetItemProperty(int id, int prop, void* dest)
    {
        if (prop > 6)
        {
            if (prop == 256)
            {
                *(CStr*)dest = this->m_models[id].m_name;
                return 1;
            }
            else if (prop == 257)
            {
                *(CStr*)dest = this->m_models[id].m_fileName;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            *(int*)dest = this->m_models[id].m_additionalData[prop];
            return 1;
        }
    }

    int DataServer::GetNumItems() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DataServer::SetError(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool DataServer::IsValid() const
    {
        return this->m_valid;
    }

    int DataServer::ParseProto(char const* in, Proto* protocol, int* paramsPos)
    {
        *protocol = PROTO_NONE;
        if (strlen(in) >= 3)
        {
            auto pos = strchr(in, ':');
            if (pos != 0)
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
        }
        m_lastError = "No protocol found";
        return 0;
    }
}
