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
        /* 0x0008 */ m3d::CWorld* m_world = nullptr;
        virtual void HandleCommand(int __formal, const m3d::CConsoleParams& __formal1) override /* 0x00 */;
        virtual bool HandleCVar(const m3d::CVar* __formal, const m3d::CConsoleParams& __formal1) override /* 0x04 */;

        struct PredictInfo
        {
            /* 0x0000 */ m3d::SgNode* m_node;

            struct Frame
            {
                /* 0x0000 */ int m_timeStamp;
                /* 0x0004 */ CVector m_origin;
                /* 0x0010 */ Quaternion m_direction;
            }; /* size: 0x0020 */

            /* 0x0004 */ m3d::CClient::PredictInfo::Frame m_prev;
            /* 0x0024 */ m3d::CClient::PredictInfo::Frame m_cur;
        }; /* size: 0x0044 */

    private:
        /* 0x000c */ retruxx::vector<m3d::CClient::PredictInfo, retruxx::allocator<m3d::CClient::PredictInfo> > m_predictData;
        /* 0x001c */ retruxx::vector<int, retruxx::allocator<int> > m_predictLastEmptySlots;
        void PredictInit();
        void PredictDone();
        void PredictUpdate(int curTime, unsigned int dt);
        void PredictAddNode(m3d::SgNode* n);
        void PredictRemoveNode(m3d::SgNode* n);
        void PredictUpdateNode(m3d::SgNode* n, const Quaternion& direction, int curTime);

    public:
        CClient(const m3d::CClient&);
        CClient();
        virtual  ~CClient() /* 0x08 */;
        int Init();
        int RecieveData(const CSimpleNetPacket* pack, ai::CServer* server);
        m3d::SgNode* CreateServerControlledNode(const int numModel);
        int Update(int curTime, unsigned int dt);
        m3d::CWorld& GetWorld();
        virtual void OnBeforeDeviceReset() override /* 0x04 */;
        virtual void OnAfterDeviceReset() override /* 0x08 */;
        void Reset();
        virtual const char* GetCallbackName() const override /* 0x00 */;
    }; /* size: 0x002c */

    static_assert(sizeof(CClient) == 0x002c);

    inline CClient* pClient = nullptr;
}
