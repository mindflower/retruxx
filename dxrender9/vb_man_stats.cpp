// Vertex buffer statistics: the device memory counters and the "existing vertex buffers" report
// (the original dxrender9/vb_man_stats.cpp).
#include <algorithm>
#include <list>
#include <map>
#include <vector>

#include <core/kernel.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>

#include "device.h"
#include "log.h"

namespace
{
    // The build identification the original writes into its reports (see device.h).
    char const* const BUILD_STRING = DXRENDER9_BUILD_STRING;

    // The separator lines of the report: 75 characters of "-==- " and 98 dashes.
    char const* const REPORT_BANNER =
        "-==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==-";
    char const* const REPORT_DASHES =
        "----------"
        "----------"
        "----------"
        "----------"
        "----------"
        "----------"
        "----------"
        "----------"
        "----------"
        "--------";

    struct SortVbByOwnTypeVertexTypeOwnSize
    {
        // orig 0x62ea20 vb_man_stats.cpp:34
        bool operator()(CDevice::CVertexBuffer* vb1, CDevice::CVertexBuffer* vb2)
        {
            if ((vb1->m_desc.Usage & D3DUSAGE_DYNAMIC) && !(vb2->m_desc.Usage & D3DUSAGE_DYNAMIC))
            {
                return true;
            }
            if (!(vb1->m_desc.Usage & D3DUSAGE_DYNAMIC) && (vb2->m_desc.Usage & D3DUSAGE_DYNAMIC))
            {
                return false;
            }

            if (vb1->m_desc.FVF < vb2->m_desc.FVF)
            {
                return true;
            }
            if (vb1->m_desc.FVF > vb2->m_desc.FVF)
            {
                return false;
            }

            return vb1->m_desc.Size < vb2->m_desc.Size;
        }
    };
}

// orig 0x631420 vb_man_stats.cpp:55
bool CDevice::ReportVbsInfo(const char* fileName)
{
    std::map<IDirect3DVertexDeclaration9*, CStr> typeToStr;

    typeToStr[m_vdXYZCT1] = "XYZCT1";
    typeToStr[m_vdXYZT1] = "XYZT1";
    typeToStr[m_vdXYZNT1] = "XYZNT1";
    typeToStr[m_vdXYZNT2] = "XYZNT2";
    typeToStr[m_vdXYZNT3] = "XYZNT3";
    typeToStr[m_vdXYZN] = "XYZN";
    typeToStr[m_vdXYZ] = "XYZ";
    typeToStr[m_vdXYZW] = "XYZW";
    typeToStr[m_vdXYZC] = "XYZC";
    typeToStr[m_vdXYZNC] = "XYZNC";
    typeToStr[m_vdXYZWCT1] = "XYZWCT1";
    typeToStr[m_vdXYZWC] = "XYZWC";
    typeToStr[m_vdXYZNCT1] = "XYZNCT1";
    typeToStr[m_vdXYZNCT2] = "XYZNCT2";
    typeToStr[m_vdXYZCT2] = "XYZCT2";
    typeToStr[m_vdXYZW4NCT1] = "XYZW4NCT1";
    typeToStr[m_vdXYZW4TNCT1] = "XYZW4TNCT1";
    typeToStr[m_vdXYZNT1T] = "XYZNT1T";
    typeToStr[m_vdXYZNCT1T] = "XYZNCT1T";
    typeToStr[m_vdXYZCT1_UVW] = "XYZCT1_UVW";
    typeToStr[m_vdXYZCT2_UVW] = "XYZCT2_UVW";
    typeToStr[m_vdXYZNCT1_UV2_S1] = "XYZNCT1_UV2_S1";
    typeToStr[m_vdWaterTest] = "WaterTest";
    typeToStr[m_vdGrassTest] = "GrassTest";
    typeToStr[m_vdImpostorTest] = "ImpostorTest";
    typeToStr[m_vdYNI] = "YNI";
    typeToStr[m_vdXYZT1I] = "XYZT1I";
    typeToStr[m_vdInstanceId] = "InstanceId";

    // The buffers owned by the pools, then the native ones.
    std::vector<CVertexBuffer*> vbs;
    std::vector<CVertexBuffer*> vbPools;

    for (std::vector<std::vector<VbHandle> >::iterator poolIter = m_VbPoolBuffers.begin();
         poolIter != m_VbPoolBuffers.end(); ++poolIter)
    {
        for (std::vector<VbHandle>::iterator vbIter = poolIter->begin(); vbIter != poolIter->end(); ++vbIter)
        {
            CVertexBuffer* vb = &m_vbs[VbId(*vbIter)];

            if (vb->m_vb)
            {
                vbPools.push_back(vb);
            }
        }
    }

    unsigned int dynamincVbSize = 0;
    unsigned int staticVbSize = 0;

    unsigned int dynamicVbCount = 0;
    unsigned int staticVbCount = 0;

    unsigned int vbPoolsSize = 0;

    for (unsigned int i = 0; i < m_vbs.size(); i++)
    {
        if (!m_vbs[i].m_vb)
        {
            continue;
        }

        if (std::find(vbPools.begin(), vbPools.end(), &m_vbs[i]) == vbPools.end())
        {
            vbs.push_back(&m_vbs[i]);

            if (m_vbs[i].m_desc.Usage & D3DUSAGE_DYNAMIC)
            {
                dynamincVbSize += m_vbs[i].m_desc.Size;
                dynamicVbCount++;
            }
            else
            {
                staticVbSize += m_vbs[i].m_desc.Size;
                staticVbCount++;
            }
        }
        else
        {
            vbPoolsSize += m_vbs[i].m_desc.Size;
        }
    }

    std::sort(vbPools.begin(), vbPools.end(), SortVbByOwnTypeVertexTypeOwnSize());
    std::sort(vbs.begin(), vbs.end(), SortVbByOwnTypeVertexTypeOwnSize());

    scoped_ptr<m3d::fs::FileStream> stream(g_kernel->GetFileServer().CreateFileStream());
    if (!stream->Open(fileName, m3d::fs::IStream::OPEN_WRITE))
    {
        return false;
    }

    *stream << REPORT_BANNER << "\n";
    *stream << "-==\n";
    *stream << "-== Log category  : Existing vertex buffers info\n";
    *stream << "-== Build         : " << BUILD_STRING << "\n";
    *stream << "-==\n";
    *stream << REPORT_BANNER << "\n\n";

    *stream << "** General stats **\n";
    *stream << "Vertex Buffers count: " << (unsigned int)(vbs.size() + vbPools.size()) << "\n";
    *stream << "Native vertex buffers count: " << (unsigned int)vbs.size() << "\n";
    *stream << "Dynamic native vertex buffers count: " << dynamicVbCount << "\n";
    *stream << "Static native vertex buffers count: " << staticVbCount << "\n";
    *stream << "Vertex buffers used in pools count: " << (unsigned int)vbPools.size() << "\n";

    *stream << "\n\n";
    *stream << "** Native vertex buffers (" << (unsigned int)vbs.size() << " entries) **\n\n";
    *stream << "** Dynamic native vertex buffers (" << dynamicVbCount << " entries) **\n";
    *stream << "   VERTEX_TYPE      VertexSize   NumVerts   BufSize              Id             Description\n";
    *stream << REPORT_DASHES << "\n";

    for (unsigned int i = 0; i < vbs.size(); i++)
    {
        if (!(vbs[i]->m_desc.Usage & D3DUSAGE_DYNAMIC))
        {
            continue;
        }
        CStr outStr("   ");
        outStr += typeToStr[vbs[i]->m_vertexDecl];
        outStr += CStr(' ', 20 - outStr.length());
        outStr += CStr(vbs[i]->m_vertSz);
        outStr += CStr(' ', 33 - outStr.length());
        outStr += CStr(vbs[i]->m_desc.Size / vbs[i]->m_vertSz);
        outStr += CStr(' ', 44 - outStr.length());
        outStr += CStr(vbs[i]->m_desc.Size) + " (";
        outStr += CStr((float)vbs[i]->m_desc.Size / 1024.0f) + "Kb)";
        outStr += CStr(' ', 65 - outStr.length());
        outStr += CStr((int)(vbs[i] - &m_vbs[0]));
        outStr += CStr(' ', 80 - outStr.length());
        outStr += vbs[i]->m_vbName;
        outStr += "\n";

        *stream << outStr.c_str();
    }

    *stream << REPORT_DASHES << "\n";
    *stream << "Total size of dynamic native vertex buffers: " << dynamincVbSize << " (" << (float)dynamincVbSize / 1024.0f
            << "Kb / " << (float)dynamincVbSize / (1024.0f * 1024.0f) << "Mb)" << "\n\n";

    *stream << "** Static native vertex buffers (" << staticVbCount << " entries) **\n";
    *stream << "   VERTEX_TYPE      VertexSize   NumVerts   BufSize              Id             Description\n";
    *stream << REPORT_DASHES << "\n";

    for (unsigned int i = 0; i < vbs.size(); i++)
    {
        if (vbs[i]->m_desc.Usage & D3DUSAGE_DYNAMIC)
        {
            continue;
        }
        CStr outStr("   ");
        outStr += typeToStr[vbs[i]->m_vertexDecl];
        outStr += CStr(' ', 20 - outStr.length());
        outStr += CStr(vbs[i]->m_vertSz);
        outStr += CStr(' ', 33 - outStr.length());
        outStr += CStr(vbs[i]->m_desc.Size / vbs[i]->m_vertSz);
        outStr += CStr(' ', 44 - outStr.length());
        outStr += CStr(vbs[i]->m_desc.Size) + " (";
        outStr += CStr((float)vbs[i]->m_desc.Size / 1024.0f) + "Kb)";
        outStr += CStr(' ', 65 - outStr.length());
        outStr += CStr((int)(vbs[i] - &m_vbs[0]));
        outStr += CStr(' ', 80 - outStr.length());
        outStr += vbs[i]->m_vbName;
        outStr += CStr("\n");

        *stream << outStr.c_str();
    }

    *stream << REPORT_DASHES << "\n";
    *stream << "Total size of static native vertex buffers: " << staticVbSize << " (" << (float)staticVbSize / 1024.0f
            << "Kb / " << (float)staticVbSize / (1024.0f * 1024.0f) << "Mb)" << "\n\n";

    *stream << "Total size of native vertex buffers: " << (staticVbSize + dynamincVbSize) << " ("
            << (float)(staticVbSize + dynamincVbSize) / 1024.0f << "Kb / "
            << (float)(staticVbSize + dynamincVbSize) / (1024.0f * 1024.0f) << "Mb)" << "\n\n";

    *stream << "** Vertex buffers pools (" << (unsigned int)m_VbPoolBuffers.size() << " entries) **\n";
    *stream << REPORT_DASHES << "\n\n\n";

    unsigned int poolSize = 0;
    unsigned int poolUsedSize = 0;
    unsigned int bufferCount = 0;

    CStr outStr;

    bool dumpLastRecord = false;
    // The buffer the current pool group started with (the original carries its index across iterations).
    unsigned int lastI = 0;

    for (unsigned int i = 0; i < vbPools.size() || dumpLastRecord; i++)
    {
        if (dumpLastRecord || vbPools[i]->m_vertexDecl != vbPools[lastI]->m_vertexDecl)
        {
            *stream << "** Pool for vertexes " << typeToStr[vbPools[lastI]->m_vertexDecl].c_str() << " ("
                    << CStr(bufferCount).c_str() << " buffers) **\n\n";
            *stream << "   BufferSize            UsedSize              FieldVbId\n";
            *stream << REPORT_DASHES << "\n";

            *stream << outStr.c_str();

            *stream << REPORT_DASHES << "\n\n";

            *stream << "Total pool size: " << poolSize << " (" << (float)poolSize / 1024.0f << "Kb / "
                    << (float)poolSize / (1024.0f * 1024.0f) << "Mb)" << "\n";

            *stream << "Total used size: " << poolUsedSize << " (" << (float)poolUsedSize / 1024.0f << "Kb / "
                    << (float)poolUsedSize / (1024.0f * 1024.0f) << "Mb)" << "\n\n\n";

            outStr = "";
            bufferCount = 0;
            poolSize = 0;
            poolUsedSize = 0;
            if (dumpLastRecord)
            {
                break;
            }
        }

        CStr outStr1("   ");

        outStr1 += CStr(vbPools[i]->m_desc.Size) + " (";
        outStr1 += CStr((float)vbPools[i]->m_desc.Size / (1024.0f * 1024.0f)) + "Mb)";
        outStr1 += CStr(' ', 25 - outStr1.length());

        // The pool this buffer belongs to, by its handle.
        unsigned int poolIndex = 0;
        InternalHandle<VbHandle> vbHandle;
        vbHandle.SetId(vbPools[i] - &m_vbs[0]);
        for (std::vector<std::vector<VbHandle> >::iterator poolIter = m_VbPoolBuffers.begin();
             poolIter != m_VbPoolBuffers.end(); ++poolIter)
        {
            if (std::find(poolIter->begin(), poolIter->end(), vbHandle) != poolIter->end())
            {
                poolIndex = poolIter - m_VbPoolBuffers.begin();
                break;
            }
        }

        // The vertices of the fields that live in this buffer of the pool.
        unsigned int usedVerts = 0;
        for (std::list<PoolFieldInfo>::iterator fieldIter = m_VbPoolFields[poolIndex].begin();
             fieldIter != m_VbPoolFields[poolIndex].end(); ++fieldIter)
        {
            if (fieldIter->Offset / m_VbPoolSize == bufferCount)
            {
                usedVerts += fieldIter->Size;
            }
        }
        unsigned int usedSize = m_vbs[VbId(m_VbPoolBuffers[poolIndex][0])].m_vertSz * usedVerts;

        poolUsedSize += usedSize;

        outStr1 += CStr(usedSize) + " (";
        outStr1 += CStr((float)usedSize / (1024.0f * 1024.0f)) + "Mb)";
        outStr1 += CStr(' ', 45 - outStr1.length());
        outStr1 += CStr((int)(vbPools[i] - &m_vbs[0]));
        outStr1 += CStr("\n");

        outStr += outStr1;
        poolSize += vbPools[i]->m_desc.Size;
        bufferCount++;

        lastI = i;
        dumpLastRecord = vbPools.size() == i + 1;
    }

    *stream << REPORT_DASHES << "\n";
    *stream << "Total size of vertex buffers pools: " << vbPoolsSize << " (" << (float)vbPoolsSize / 1024.0f << "Kb / "
            << (float)vbPoolsSize / (1024.0f * 1024.0f) << "Mb)" << "\n\n";

    *stream << "Total size of vertex buffers: " << (vbPoolsSize + staticVbSize + dynamincVbSize) << " ("
            << (float)(vbPoolsSize + staticVbSize + dynamincVbSize) / 1024.0f << "Kb / "
            << (float)(vbPoolsSize + staticVbSize + dynamincVbSize) / (1024.0f * 1024.0f) << "Mb)" << "\n\n";

    stream->Close();

    return true;
}

// orig 0x630dd0 vb_man_stats.cpp:335
void CDevice::UpdateVBMemStats()
{
    std::vector<CVertexBuffer*> vbPools;

    for (std::vector<std::vector<VbHandle> >::iterator iter = m_VbPoolBuffers.begin(); iter != m_VbPoolBuffers.end();
         ++iter)
    {
        for (std::vector<VbHandle>::iterator vbIter = iter->begin(); vbIter != iter->end(); ++vbIter)
        {
            vbPools.push_back(&m_vbs[VbId(*vbIter)]);
        }
    }

    m_devMemStats.DynamicVBCount = 0;
    m_devMemStats.DynamicVBSize = 0;
    m_devMemStats.StaticVBCount = 0;
    m_devMemStats.StaticVBSize = 0;
    m_devMemStats.VBPoolsSize = 0;

    for (unsigned int i = 0; i < m_vbs.size(); i++)
    {
        if (std::find(vbPools.begin(), vbPools.end(), &m_vbs[i]) == vbPools.end())
        {
            if (m_vbs[i].m_desc.Usage & D3DUSAGE_DYNAMIC)
            {
                m_devMemStats.DynamicVBSize += m_vbs[i].m_desc.Size;
                m_devMemStats.DynamicVBCount++;
            }
            else
            {
                m_devMemStats.StaticVBSize += m_vbs[i].m_desc.Size;
                m_devMemStats.StaticVBCount++;
            }
        }
        else
        {
            m_devMemStats.VBPoolsSize += m_vbs[i].m_desc.Size;
        }
    }
}
