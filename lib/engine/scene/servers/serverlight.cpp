#include <flares.h>
#include <m3dapp.h>
#include <stdexcept>
#include <scene/servers/serverlight.h>
#include <core/log.h>
#include <core/ini.h>
#include <core/ref_ptr.h>

int statsInited = 0;

struct PointLightStats
{
    /* 0x0000 */ int numLightsToRender = 0;
    /* 0x0004 */ int numLightCellsRendered = 0;
    /* 0x0008 */ int numLightModelsRendered = 0;
    /* 0x000c */ int curFrame = 0;

    void Zero()
    {
        int numLightsToRender = 0;
        int numLightCellsRendered = 0;
        int numLightModelsRendered = 0;
    }

}; /* size: 0x0010 */

namespace m3d
{
    int LightsServer::SaveAllLoadedEntities(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int LightsServer::Init()
    {
        m_valid = true;
        CFlare::Init();
        return 1;
    }

    int LightsServer::Release()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int LightsServer::AddItem(char const* params, char const* id)
    {
        auto item = GetItemByName(id, false);
        if (item == -1)
        {
            int protoPos = 0;
            DataServer::Proto proto;
            ParseProto(params, &proto, &protoPos);
            if (proto == PROTO_FILE)
            {
                auto const filename = &params[protoPos];
                CStr err;
                if (ref_ptr xmlFile = m3d::ReadXmlFile(&params[protoPos], &err))
                {
                    ref_ptr xmlNode = xmlFile->CreateNode();
                    xmlFile->GetFirstChild(xmlNode, "Light");
                    if (xmlNode->IsEmpty())
                    {
                        M3D_LOG_INFO("LightsServer:: light descriptor missing in " + CStr(filename));
                        return -1;
                    }

                    int radius = 0;
                    CVector color;
                    float ttl = 0.0;

                    SafeIntAttrib(radius, xmlNode, "Radius");
                    SafeVectorAttrib(color, xmlNode, "Color");
                    SafeFloatAttrib(ttl, xmlNode, "Ttl");

                    auto pointModel = new PointLightModel();
                    pointModel->m_radius = radius;
                    pointModel->m_color = color;
                    pointModel->m_ttl = ttl;

                    auto model = DataServer::Model(pointModel, filename, filename, id);
                    m_models.push_back(std::move(model));
                    return m_models.size() - 1;
                }
            }
            else
            {
                M3D_LOG_INFO("protocol is not supported");
                return -1;
            }
        }
        return item;
    }

    LightsServer::~LightsServer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void LightsServer::RenderItem(int id, void*)
    {
        static PointLightStats stats_0;
        m_profiler->StartCountdown();

        // TODO: implement LightsServer::RenderItem

        //if (id != -4)
        //{
        //    RETRUXX_NOT_IMPLEMENTED;
        //}

        m_profiler->EndCountdown();
    }

    int LightsServer::RemoveItem(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int LightsServer::GetItemProperty(int id, int prop, void* dest)
    {
        // TODO: check this
        if (prop != 8449)
            return m3d::DataServer::GetItemProperty(id, prop, dest);
        *(float*)dest = ((PointLightModel*)m_models[id].m_ptr)->m_radius;
        return 1;
    }

    LightsServer::LightsServer()
    {
        auto id = Application::g_pApp->GetProfilerStack().AddProfiler("point lights", 0x1E);
        if (id < Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profiler = Application::g_pApp->GetProfilerStack().GetProfiler(id);
        }
    }

    int LightsServer::SetItemProperty(int, int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void LightsServer::AddItemsList(retruxx::vector<ServerItem>& list)
    {
        auto size = list.size();
        for (int i = 0; i < size; ++i)
        {
            if (m_fnLoadCallback)
            {
                m_fnLoadCallback(i / size, m_fnLoadCallbackData);
            }
            if (AddItem(list[i].m_filename.c_str(), list[i].m_id.c_str()) == -1)
            {
                M3D_LOG_INFO("LightServer: cannot read " + list[i].m_filename);
            }
        }
    }
}  // namespace m3d
