#include <stdexcept>
#include <scene/servers/dataserver.h>

namespace m3d
{
    DataServer::Model::Model(void* p, char const* fileName, char const* originalFileName, char const* id) :
        m_ptr(p),
        m_fileName(fileName),
        m_originalFileName(originalFileName),
        m_name(id)
    {
    }

    int DataServer::GetItemByName(char const*, bool) const
    {
        throw std::logic_error("Not implemented");
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

    int DataServer::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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

    int DataServer::ParseProto(char const*, Proto*, int*)
    {
        throw std::logic_error("Not implemented");
    }
}
