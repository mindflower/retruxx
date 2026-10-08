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
        // RVA 0x75EB20 - servers that cast shadows override this.
        return 0;
    }

    int DataServer::RenderNodeSet(SgNode**, unsigned, RenderNodeInfo)
    {
        // RVA 0x75EB00 - servers that render whole node sets override this.
        return 0;
    }

    void DataServer::RenderTransparents(SgNode**, unsigned)
    {
        // RVA 0x75EB10
    }

    void DataServer::PostLoad()
    {
    }

    CStr const& DataServer::GetNameByItem(int sh) const
    {
        // RVA 0x6A6D30 - NOTE: the handle is not range checked.
        return m_models[sh].m_name;
    }

    bool DataServer::ReportServerInfo(char const*)
    {
        // RVA 0x75EB80 - servers with something to report override this.
        return true;
    }

    int DataServer::WriteItemToXmlNode(int sh, cmn::XmlFile*, cmn::XmlNode* node)
    {
        // RVA 0x6A6D80 - the file attribute is written only for items that came from one.
        node->SetAttribute("id", m_models[sh].m_name.c_str());
        if (!m_models[sh].m_originalFileName.empty())
        {
            node->SetAttribute("file", m_models[sh].m_originalFileName.c_str());
        }
        return 1;
    }

    int DataServer::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        // RVA 0x6A5C30
        for (unsigned i = 0; i < m_models.size(); ++i)
        {
            ref_ptr itemNode = file->CreateNode(cmn::XML_NODE_ELEMENT, "Item");
            if (!WriteItemToXmlNode(i, file, itemNode))
            {
                return 0;
            }
            node->AddChild(itemNode);
        }
        return 1;
    }

    void DataServer::UpdateItem(int, void*)
    {
        // RVA 0x75EB40
    }

    int DataServer::Release()
    {
        // RVA 0x75D0F0
        m_valid = false;
        return 1;
    }

    int DataServer::GenerateImpostorsIfNeeded()
    {
        // RVA 0x75EB30 - only the animated models server builds impostors.
        return 0;
    }

    int DataServer::Init()
    {
        m_valid = true;
        return 1;
    }

    void DataServer::Restore()
    {
        // RVA 0x75EBA0
    }

    void DataServer::UnregisterNode(SgNode*)
    {
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

    CStr DataServer::GetOriginalFileName(int sh)
    {
        // RVA 0x6A6D50 - NOTE: despite the name this returns m_fileName, not m_originalFileName.
        return m_models[sh].m_fileName;
    }

    DataServer::~DataServer()
    {
        // RVA 0x75EA30 - the members clean themselves up; the items a server loaded are freed by its own Release.
    }

    void DataServer::RegisterNode(SgNode*)
    {
    }

    void DataServer::Invalidate()
    {
        // RVA 0x75EB90
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
        // RVA 0x6A5D70
        return m_models.size();
    }

    void DataServer::SetError(CStr const& err)
    {
        // RVA 0x6A5A90
        m_lastError = err;
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
