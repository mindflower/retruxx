#include <client.h>
#include <stdexcept>

#include "m3dapp.h"
#include "world.h"
#include "core/kernel.h"

namespace m3d
{
    CClient::~CClient()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    int CClient::RecieveData(CSimpleNetPacket const*, ai::CServer*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CClient::PredictInit()
    {
    }

    bool CClient::HandleCVar(CVar const*, CConsoleParams const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CClient::PredictUpdateNode(SgNode*, Quaternion const&, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CClient::PredictUpdate(int, unsigned)
    {
    }

    void CClient::PredictAddNode(SgNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CClient::PredictDone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CClient::HandleCommand(int, CConsoleParams const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CClient::PredictRemoveNode(SgNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CWorld& CClient::GetWorld()
    {
        return *m_world;
    }
}
