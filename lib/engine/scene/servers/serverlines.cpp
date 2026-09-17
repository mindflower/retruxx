#include <stdexcept>
#include <scene/servers/serverlines.h>
#include <m3dapp.h>
#include <client.h>
#include <landscape.h>
#include <world.h>
#include <renderer/i_renderer.h>

namespace m3d
{
    namespace
    {
        // What SgLinesNode hands to RenderItem (SgLinesNode::Render::RenderInfo in the PDB).
        struct RenderInfo
        {
            /* 0x0000 */ int rad;
            /* 0x0004 */ int color;
            /* 0x0008 */ CMatrix matrix;
        }; /* size: 0x0048 */

        // Height of the ground at a point given relative to the item's origin, relative to that origin, lifted a
        // little so the line is not buried.
        float GroundZ(CVector const& origin, float x, float y)
        {
            return pClient->GetWorld().GetLandscape().GetHeight(x, y, -1, true) - origin.z + 0.5f;
        }
    }  // namespace

    int LinesServer::AddItem(char const*, char const*)
    {
        // RVA 0x76A290 - the four shapes are built in; nothing can be loaded.
        return 0;
    }

    int LinesServer::SaveAllLoadedEntities(char const*)
    {
        // RVA 0x76A2D0
        return 1;
    }

    LinesServer::~LinesServer()
    {
        // RVA 0x76AF00
        m_valid = false;
    }

    int LinesServer::RemoveItem(int)
    {
        // RVA 0x76A2A0
        return 1;
    }

    int LinesServer::Init()
    {
        m_valid = true;
        m_models.push_back({ nullptr, nullptr, nullptr, "LSI_CIRCLE" });
        m_models.push_back({ nullptr, nullptr, nullptr, "LSI_SIGHT" });
        m_models.push_back({ nullptr, nullptr, nullptr, "LSI_FLOWER" });
        m_models.push_back({ nullptr, nullptr, nullptr, "LSI_ARROW" });
        return 1;
    }

    int LinesServer::Release()
    {
        // RVA 0x76A2E0
        m_valid = false;
        return 1;
    }

    LinesServer::LinesServer()
    {
    }

    void LinesServer::RenderItem(int id, void* params)
    {
        // RVA 0x76A2F0 - outlines draped over the landscape: 0 a circle, 1 a gun sight, 2 a flower and 3 an arrow.
        auto* const renderer = M3D_RENDERER;
        if (id == -2)
        {
            renderer->SetStageState(0, rend::BM_COLOR, rend::TS_DIFFUSE);
            renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_NONE);
            renderer->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
            renderer->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);
            rend::TexHandle noTexture;
            renderer->SetTexture(0, noTexture, -1.0);
            renderer->PushBlend(rend::BM_NONE);
            return;
        }
        if (id == -3)
        {
            renderer->PopBlend();
            return;
        }
        if (id == -4 || static_cast<unsigned>(id) > 3)
        {
            return;
        }

        auto* const ri = static_cast<RenderInfo*>(params);
        CVector const origin(ri->matrix._41, ri->matrix._42, ri->matrix._43);
        float const PI_8 = 0.39269909f;
        float const PI_16 = 0.19634955f;

        switch (id)
        {
        case 0:
        {
            // NOTE: the node's matrix is replaced by its translation alone, dropping the rotation.
            ri->matrix.translation(origin);
            renderer->MatPush(ri->matrix);
            CVector prevPoint;
            for (int i = 0; i <= 16; ++i)
            {
                double const angle = static_cast<double>(i) * PI_8;
                float const x = static_cast<float>(cos(angle) * ri->rad);
                float const y = static_cast<float>(sin(angle) * ri->rad);
                CVector const curPoint(x, y, GroundZ(origin, origin.x + x, origin.y + y));
                if (i)
                {
                    M3D_APP->DrawLine(curPoint, prevPoint, ri->color);
                }
                prevPoint = curPoint;
            }
            break;
        }
        case 1:
        {
            // A circle with a cross 10% wider than it.
            ri->matrix.translation(origin);
            renderer->MatPush(ri->matrix);
            CVector prevPoint;
            for (int i = 0; i <= 16; ++i)
            {
                double const angle = static_cast<double>(i) * PI_8;
                float const x = static_cast<float>(cos(angle) * ri->rad);
                float const y = static_cast<float>(sin(angle) * ri->rad);
                CVector const curPoint(x, y, GroundZ(origin, origin.x + x, origin.y + y));
                if (i)
                {
                    M3D_APP->DrawLine(curPoint, prevPoint, ri->color);
                }
                prevPoint = curPoint;
            }

            float const left = static_cast<float>(-ri->rad) * 1.1f;
            CVector const p1(left, 0.0f, GroundZ(origin, origin.x + left, origin.y));
            float const right = static_cast<float>(ri->rad) * 1.1f;
            CVector const p2(right, 0.0f, GroundZ(origin, origin.x + right, origin.y));
            M3D_APP->DrawLine(p1, p2, ri->color);

            float const bottom = static_cast<float>(-ri->rad) * 1.1f;
            CVector const p3(0.0f, bottom, GroundZ(origin, origin.x, origin.y + bottom));
            float const top = static_cast<float>(ri->rad) * 1.1f;
            CVector const p4(0.0f, top, GroundZ(origin, origin.x, origin.y + top));
            M3D_APP->DrawLine(p3, p4, ri->color);
            break;
        }
        case 2:
        {
            // A circle where every fourth segment is replaced by a notch towards the centre.
            ri->matrix.translation(origin);
            renderer->MatPush(ri->matrix);
            CVector prevPoint;
            for (unsigned i = 0; i <= 16; ++i)
            {
                double const angle = static_cast<double>(static_cast<int>(i)) * PI_8 + PI_16;
                float const x = static_cast<float>(cos(angle) * ri->rad);
                float const y = static_cast<float>(sin(angle) * ri->rad);
                CVector const curPoint(x, y, GroundZ(origin, origin.x + x, origin.y + y));
                if (i)
                {
                    if (i % 4)
                    {
                        M3D_APP->DrawLine(curPoint, prevPoint, ri->color);
                    }
                    else
                    {
                        int const quarter = static_cast<int>(i / 4);
                        CVector const segment(curPoint.x - prevPoint.x, curPoint.y - prevPoint.y, curPoint.z - prevPoint.z);
                        float const half = segment.length() * 0.5f;
                        bool const odd = (quarter % 2) != 0;
                        float px;
                        float py;
                        float bx;
                        float by;
                        if (odd)
                        {
                            px = curPoint.x;
                            py = static_cast<float>(2 * (quarter == 1) - 1) * half;
                            bx = prevPoint.x;
                            by = py;
                        }
                        else
                        {
                            px = static_cast<float>(2 * (quarter == 4) - 1) * half;
                            py = curPoint.y;
                            bx = px;
                            by = prevPoint.y;
                        }
                        CVector const p1(px, py, GroundZ(origin, origin.x + px, origin.y + py));
                        CVector const p2(bx, by, GroundZ(origin, origin.x + bx, origin.y + by));
                        M3D_APP->DrawLine(curPoint, p1, ri->color);
                        M3D_APP->DrawLine(prevPoint, p2, ri->color);
                    }
                }
                prevPoint = curPoint;
            }
            break;
        }
        case 3:
        {
            // An arrow in the node's own orientation, sampled on the ground where it lands rotated.
            renderer->MatPush(ri->matrix);
            float const r = static_cast<float>(ri->rad);
            CVector2 const points[7] = {
                {-r, 0.0f}, {0.0f, -r}, {r, 0.0f}, {r * 0.5f, 0.0f}, {r * 0.5f, r}, {r * -0.5f, r}, {r * -0.5f, 0.0f}};
            CVector firstPoint;
            CVector prevPoint;
            for (unsigned i = 0; i < 7; ++i)
            {
                float const px = points[i].x;
                float const py = points[i].y;
                float yaw;
                float pitch;
                float roll;
                ri->matrix.getYPR(yaw, pitch, roll);
                float const c = static_cast<float>(cos(roll));
                float const sn = static_cast<float>(sin(roll));
                CVector const curPoint(
                    px, py, GroundZ(origin, c * px - sn * py + origin.x, c * py + sn * px + origin.y));
                if (i)
                {
                    M3D_APP->DrawLine(prevPoint, curPoint, ri->color);
                }
                else
                {
                    firstPoint = curPoint;
                }
                prevPoint = curPoint;
            }
            M3D_APP->DrawLine(prevPoint, firstPoint, ri->color);
            break;
        }
        }
        renderer->MatPop(true);
    }

    int LinesServer::SetItemProperty(int, int, void*)
    {
        // RVA 0x76A2C0
        return 0;
    }

    int LinesServer::GetItemProperty(int, int, void*)
    {
        // RVA 0x76A2B0
        return 0;
    }

    void LinesServer::AddItemsList(retruxx::vector<ServerItem>&)
    {
        // RVA 0x76A280
    }
}
