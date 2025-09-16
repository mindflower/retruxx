#include <stdexcept>
#include <scene/servers/serversound.h>
#include <core/kernel.h>
#include <config.h>
#include <m3dapp.h>

namespace m3d
{
    void Sound3DServer::UnregisterNode(m3d::SgNode* node)
    {
        if (M3D_KERNEL->GetEngineCfg().m_snd_Enable.GetB())
        {
            auto channel = -1;
            node->GetProperty(9729u, &channel);
            if (channel != -1)
                m3d::Application::g_pApp->m_sound->StopChannel(channel);
        }
    }

    int Sound3DServer::RemoveItem(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Sound3DServer::~Sound3DServer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Sound3DServer::AddItem(char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Sound3DServer::PostLoad()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Sound3DServer::GetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Sound3DServer::RegisterNode(m3d::SgNode*)
    {
    }

    int Sound3DServer::SaveAllLoadedEntities(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Sound3DServer::RenderItem(int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Sound3DServer::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Sound3DServer::AddItemsList(retruxx::vector<m3d::DataServer::ServerItem>&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Sound3DServer::_AddItemFromXmlNode(m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Sound3DServer::_AddFakeItem()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Sound3DServer::_AddTripleItem(CStr, CStr, CStr, char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Sound3DServer::_AddItem(char const*, char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Sound3DServer::_AddDoubleItem(CStr, CStr, char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }
}
