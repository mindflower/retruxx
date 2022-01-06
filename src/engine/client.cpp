#include <client.h>
#include <stdexcept>

namespace m3d
{
    CClient* pClient = nullptr;

    CClient::~CClient()
    {
        throw std::logic_error("Not implemented");
    }

    SgNode* CClient::CreateServerControlledNode(int)
    {
        throw std::logic_error("Not implemented");
    }

    void CClient::Reset()
    {
        throw std::logic_error("Not implemented");
    }

    char const* CClient::GetCallbackName() const
    {
        throw std::logic_error("Not implemented");
    }

    CClient::CClient()
    {
        throw std::logic_error("Not implemented");
    }

    int CClient::Update(int, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void CClient::OnBeforeDeviceReset()
    {
        throw std::logic_error("Not implemented");
    }

    int CClient::Init()
    {
        throw std::logic_error("Not implemented");
    }

    void CClient::OnAfterDeviceReset()
    {
        throw std::logic_error("Not implemented");
    }

    int CClient::RecieveData(CSimpleNetPacket const*, ai::CServer*)
    {
        throw std::logic_error("Not implemented");
    }

    void CClient::PredictInit()
    {
        throw std::logic_error("Not implemented");
    }

    bool CClient::HandleCVar(CVar const*, CConsoleParams const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CClient::PredictUpdateNode(SgNode*, Quaternion const&, int)
    {
        throw std::logic_error("Not implemented");
    }

    void CClient::PredictUpdate(int, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void CClient::PredictAddNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void CClient::PredictDone()
    {
        throw std::logic_error("Not implemented");
    }

    void CClient::HandleCommand(int, CConsoleParams const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CClient::PredictRemoveNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    CWorld& CClient::GetWorld()
    {
        return *m_world;
    }
}
