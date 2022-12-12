#pragma once
#include <iface.h>
#include <core/console/console.h>
#include <math/quaternion.h>
#include <math/vector.h>

namespace ai
{
    class CServer;
}

namespace m3d
{
    class SgNode;
    class CWorld;

    class CClient :  public IConHandler, public IDeviceResetCallback
    {
    public:
        class PredictInfo
        {
        public:
            class Frame
            {

            private:
                int m_timeStamp;
                CVector m_origin;
                Quaternion m_direction;
            };

        private:
            SgNode* m_node;
            Frame m_prev;
            Frame m_cur;
        };

    public:
        virtual ~CClient();
        SgNode * CreateServerControlledNode(int);
        void Reset();
        CWorld& GetWorld();
        CClient();
        int Update(int,unsigned int);
        virtual char const* GetCallbackName() const;
        virtual void OnBeforeDeviceReset();
        virtual void OnAfterDeviceReset();
        int Init();
        int RecieveData(struct CSimpleNetPacket const *,class ai::CServer *);

    private:
        void PredictInit();
        virtual bool HandleCVar(CVar const *,CConsoleParams const &);
        void PredictUpdateNode(SgNode *,struct Quaternion const &,int);
        void PredictUpdate(int,unsigned int);
        void PredictAddNode(SgNode *);
        void PredictDone();
        virtual void HandleCommand(int,CConsoleParams const &);
        void PredictRemoveNode(SgNode *);

    private:
        CWorld *m_world = nullptr;
        std::vector<PredictInfo> m_predictData;
        std::vector<int> m_predictLastEmptySlots;
    };
}
