#include <client.h>
#include <stdexcept>

#include "m3dapp.h"
#include "world.h"

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
        M3D_RENDERER->RegisterResetCallback(
            this);
    }

    int CClient::Update(int curTime, unsigned dt)
    {
        m3d::CClient::PredictUpdate(curTime, dt);
        m_world->Update();
        return 1;
    }

    void CClient::OnBeforeDeviceReset()
    {
        throw std::logic_error("Not implemented");
    }

    int CClient::Init()
    {
        m_world = new CWorld;
        m_world->SetOwner(this);
        PredictInit();
        return 1;
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
