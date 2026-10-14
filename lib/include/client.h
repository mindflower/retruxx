#pragma once
#include <iface.h>
#include <core/console/console.h>
#include <math/quaternion.h>
#include <math/vector.h>

class CSimpleNetPacket;

namespace ai
{
    class CServer;
}

namespace m3d
{
    class SgNode;
    class CWorld;

    class CClient : public m3d::IConHandler, public m3d::IDeviceResetCallback
    {
    private:
        m3d::CWorld* m_world = nullptr;
        virtual void HandleCommand(int __formal, m3d::CConsoleParams const& __formal1) override;
        virtual bool HandleCVar(m3d::CVar const* __formal, m3d::CConsoleParams const& __formal1) override;

        struct PredictInfo
        {
            m3d::SgNode* m_node;

            struct Frame
            {
                int m_timeStamp;
                CVector m_origin;
                Quaternion m_direction;
            };

            m3d::CClient::PredictInfo::Frame m_prev;
            m3d::CClient::PredictInfo::Frame m_cur;
        };

    private:
        retruxx::vector<m3d::CClient::PredictInfo, retruxx::allocator<m3d::CClient::PredictInfo>> m_predictData;
        retruxx::vector<int, retruxx::allocator<int>> m_predictLastEmptySlots;
        void PredictInit();
        void PredictDone();
        void PredictUpdate(int curTime, unsigned int dt);
        void PredictAddNode(m3d::SgNode* n);
        void PredictRemoveNode(m3d::SgNode* n);
        void PredictUpdateNode(m3d::SgNode* n, Quaternion const& direction, int curTime);

    public:
        CClient(m3d::CClient const&);
        CClient();
        virtual ~CClient();
        int Init();
        int RecieveData(CSimpleNetPacket const* pack, ai::CServer* server);
        m3d::SgNode* CreateServerControlledNode(int const numModel);
        int Update(int curTime, unsigned int dt);
        m3d::CWorld& GetWorld();
        virtual void OnBeforeDeviceReset() override;
        virtual void OnAfterDeviceReset() override;
        void Reset();
        virtual char const* GetCallbackName() const override;
    };

    inline CClient* pClient = nullptr;
}  // namespace m3d
