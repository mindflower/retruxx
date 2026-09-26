#include <client.h>
#include <stdexcept>

#include "m3dapp.h"
#include "world.h"
#include "core/kernel.h"

namespace m3d
{
    CClient::~CClient()
    {
        // RVA 0x5C0350
        M3D_RENDERER->UnregisterResetCallback(this);
        delete m_world;
        m_world = nullptr;
        PredictDone();
    }

    SgNode* CClient::CreateServerControlledNode(int numModel)
    {
        if ((numModel & 0x200000) != 0)
        {
            numModel -= 0x200000;
            auto obj = M3D_KERNEL->New("SgGameUnitNode");
            obj->SetProperty(4360u, &numModel);
            return RT_DYNCAST(obj, SgNode);
        }
        if ((numModel & 0x100000) != 0)
        {
            numModel -= 0x100000;
            auto obj = M3D_KERNEL->New("SgProjectorNode");
            obj->SetProperty(4360u, &numModel);
            return RT_DYNCAST(obj, SgNode);
        }
        if ((numModel & 0x800000) != 0)
        {
            numModel -= 0x800000;
            auto obj = M3D_KERNEL->New("SgPointLightSourceNode");
            obj->SetProperty(4360u, &numModel);
            return RT_DYNCAST(obj, SgNode);
        }
        if ((numModel & 0x1000000) != 0)
        {
            numModel -= 0x1000000;
            auto obj = M3D_KERNEL->New("SgSpriteNode");
            obj->SetProperty(4360u, &numModel);
            return RT_DYNCAST(obj, SgNode);
        }
        return m_world->CreatePrefabsNode(numModel - 0x400000);
    }

    void CClient::Reset()
    {
        GetWorld().GetGraph().DeleteAllTtledNodes();
        GetWorld().GetGraph().DeleteAllRemoveIfFreeNodes();
    }

    char const* CClient::GetCallbackName() const
    {
        // RVA 0x5BFD60
        return nullptr;
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
        // RVA 0x5BFD40
        m_world->GetLandscape().Invalidate();
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
        // RVA 0x5BFD50
        m_world->GetLandscape().Restore();
    }

    int CClient::RecieveData(CSimpleNetPacket const*, ai::CServer*)
    {
        // RVA 0x5BFD00 - the network client does nothing.
        return 1;
    }

    void CClient::PredictInit()
    {
    }

    bool CClient::HandleCVar(CVar const*, CConsoleParams const&)
    {
        // RVA 0x5BFD80
        return false;
    }

    void CClient::PredictUpdateNode(SgNode*, Quaternion const&, int)
    {
        // RVA 0x7AD570 - empty; prediction is not used.
    }

    void CClient::PredictUpdate(int, unsigned)
    {
    }

    void CClient::PredictAddNode(SgNode*)
    {
        // RVA 0x7AD580 - empty.
    }

    void CClient::PredictDone()
    {
        // RVA 0x7AD550 - empty.
    }

    void CClient::HandleCommand(int, CConsoleParams const&)
    {
        // RVA 0x5BFD70 - empty.
    }

    void CClient::PredictRemoveNode(SgNode*)
    {
        // RVA 0x7AD590 - empty.
    }

    CWorld& CClient::GetWorld()
    {
        return *m_world;
    }
}
