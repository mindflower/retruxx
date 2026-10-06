// Index buffer statistics: the memory counters and the "Existing index buffers info" report
// (the original dxrender9/ib_man_stats.cpp).
#include <algorithm>
#include <list>
#include <vector>

#include <core/kernel.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>

#include "device.h"
#include "log.h"

namespace
{
    // Dynamic buffers first, then by buffer size.
    struct SortIbByOwnSize
    {
        // orig 0x62b6f0 ib_man_stats.cpp:34
        bool operator()(CDevice::CIndexBuffer* ib1, CDevice::CIndexBuffer* ib2)
        {
            bool dyn1 = (ib1->m_desc.Usage & D3DUSAGE_DYNAMIC) != 0;
            bool dyn2 = (ib2->m_desc.Usage & D3DUSAGE_DYNAMIC) != 0;
            if (dyn1 && !dyn2)
            {
                return true;
            }
            if (!dyn1 && dyn2)
            {
                return false;
            }
            return ib1->m_desc.Size < ib2->m_desc.Size;
        }
    };
}

// orig 0x62ce40 ib_man_stats.cpp:49
bool CDevice::ReportIbsInfo(const char* fileName)
{
    std::vector<CIndexBuffer*> ibPools;
    std::vector<CIndexBuffer*> ibs;

    for (std::vector<IbHandle>::iterator it = m_IbPoolBuffers.begin(); it != m_IbPoolBuffers.end(); ++it)
    {
        CIndexBuffer* ib = &m_ibs[IbId(*it)];

        if (ib->m_ib)
        {
            ibPools.push_back(ib);
        }
    }

    unsigned int dynamincIbSize = 0;
    unsigned int staticIbSize = 0;

    unsigned int dynamicIbCount = 0;
    unsigned int staticIbCount = 0;

    unsigned int ibPoolsSize = 0;

    for (unsigned int i = 0; i < m_ibs.size(); i++)
    {
        if (m_ibs[i].m_ib == 0)
        {
            continue;
        }

        if (std::find(ibPools.begin(), ibPools.end(), &m_ibs[i]) == ibPools.end())
        {
            ibs.push_back(&m_ibs[i]);

            if (m_ibs[i].m_desc.Usage & D3DUSAGE_DYNAMIC)
            {
                dynamincIbSize += m_ibs[i].m_desc.Size;
                dynamicIbCount++;
            }
            else
            {
                staticIbSize += m_ibs[i].m_desc.Size;
                staticIbCount++;
            }
        }
        else
        {
            ibPoolsSize += m_ibs[i].m_desc.Size;
        }
    }

    std::sort(ibPools.begin(), ibPools.end(), SortIbByOwnSize());
    std::sort(ibs.begin(), ibs.end(), SortIbByOwnSize());

    scoped_ptr<m3d::fs::FileStream> stream(g_kernel->GetFileServer().CreateFileStream());
    if (!stream->Open(fileName, m3d::fs::IStream::OPEN_WRITE))
    {
        return false;
    }

    *stream << "-==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==-\n";
    *stream << "-==\n";
    *stream << "-== Log category  : Existing index buffers info\n";
    *stream << "-== Build         : " << DXRENDER9_BUILD_STRING << "\n";
    *stream << "-==\n";
    *stream << "-==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==-\n\n";

    *stream << "** General stats **\n";
    *stream << "Index buffers count: " << (unsigned int)(ibs.size() + ibPools.size()) << "\n";
    *stream << "Native index buffers count: " << (unsigned int)ibs.size() << "\n";
    *stream << "Dynamic native index buffers count: " << dynamicIbCount << "\n";
    *stream << "Static native index buffers count: " << staticIbCount << "\n";
    *stream << "Index buffers used in pools count: " << (unsigned int)ibPools.size() << "\n";
    *stream << "\n\n";

    *stream << "** Native index buffers (" << (unsigned int)ibs.size() << " entries) **\n\n";
    *stream << "** Dynamic native index buffers (" << dynamicIbCount << " entries) **\n";
    *stream << "   NumVerts            BufSize             Id\n";
    *stream << "--------------------------------------------------------------------------------------------------\n";

    for (unsigned int i = 0; i < ibs.size(); i++)
    {
        if (ibs[i]->m_desc.Usage & D3DUSAGE_DYNAMIC)
        {
            CStr outStr("   ");

            outStr += CStr(ibs[i]->m_desc.Size / 2);
            outStr += CStr(' ', 23 - outStr.length());
            outStr += CStr(ibs[i]->m_desc.Size) + " (";
            outStr += CStr((float)ibs[i]->m_desc.Size / 1024.0f) + "Kb)";
            outStr += CStr(' ', 43 - outStr.length());
            outStr += CStr((int)(ibs[i] - &m_ibs[0]));
            outStr += "\n";

            *stream << outStr.c_str();
        }
    }

    *stream << "--------------------------------------------------------------------------------------------------\n";
    *stream << "Total size of dynamic native index buffers: " << dynamincIbSize << " (" << (float)dynamincIbSize / 1024.0f
            << "Kb / " << (float)dynamincIbSize / 1048576.0f << "Mb)" << "\n\n";

    *stream << "** Static native index buffers (" << staticIbCount << " entries) **\n";
    *stream << "   NumVerts            BufSize             Id\n";
    *stream << "--------------------------------------------------------------------------------------------------\n";

    for (unsigned int i = 0; i < ibs.size(); i++)
    {
        if (!(ibs[i]->m_desc.Usage & D3DUSAGE_DYNAMIC))
        {
            CStr outStr("   ");

            outStr += CStr(ibs[i]->m_desc.Size / 2);
            outStr += CStr(' ', 23 - outStr.length());
            outStr += CStr(ibs[i]->m_desc.Size) + " (";
            outStr += CStr((float)ibs[i]->m_desc.Size / 1024.0f) + "Kb)";
            outStr += CStr(' ', 43 - outStr.length());
            outStr += CStr((int)(ibs[i] - &m_ibs[0]));
            outStr += "\n";

            *stream << outStr.c_str();
        }
    }

    *stream << "--------------------------------------------------------------------------------------------------\n";
    *stream << "Total size of static native index buffers: " << staticIbSize << " (" << (float)staticIbSize / 1024.0f
            << "Kb / " << (float)staticIbSize / 1048576.0f << "Mb)" << "\n\n";

    ibs = ibPools;

    *stream << "** Index buffers pool (" << (unsigned int)m_IbPoolBuffers.size() << " buffers) **\n";
    *stream << "   BufSize             UsedSize            Id\n";
    *stream << "--------------------------------------------------------------------------------------------------\n";

    unsigned int usedSize = 0;

    for (unsigned int i = 0; i < ibs.size(); i++)
    {
        unsigned int fieldUsedSize = 0;

        CStr outStr("   ");

        outStr += CStr(ibs[i]->m_desc.Size) + " (";
        outStr += CStr((float)ibs[i]->m_desc.Size / 1024.0f) + "Kb)";
        outStr += CStr(' ', 23 - outStr.length());

        for (std::list<PoolFieldInfo>::iterator it = m_IbPoolFields.begin(); it != m_IbPoolFields.end(); ++it)
        {
            if (it->Offset / m_IbPoolSize == i)
            {
                fieldUsedSize += it->Size;
            }
        }

        fieldUsedSize *= 2;

        outStr += CStr(fieldUsedSize) + " (";
        outStr += CStr((float)fieldUsedSize / 1024.0f) + "Kb)";
        outStr += CStr(' ', 43 - outStr.length());
        outStr += CStr((int)(ibs[i] - &m_ibs[0]));
        outStr += "\n";

        usedSize += fieldUsedSize;

        *stream << outStr.c_str();
    }

    *stream << "--------------------------------------------------------------------------------------------------\n";
    *stream << "Total pool size: " << ibPoolsSize << " (" << (float)ibPoolsSize / 1024.0f << "Kb / "
            << (float)ibPoolsSize / 1048576.0f << "Mb)" << "\n\n";
    *stream << "Total used size: " << usedSize << " (" << (float)usedSize / 1024.0f << "Kb / "
            << (float)usedSize / 1048576.0f << "Mb)" << "\n\n";

    *stream << "Total size of index buffers: " << dynamincIbSize + staticIbSize + ibPoolsSize << " ("
            << (float)(dynamincIbSize + staticIbSize + ibPoolsSize) / 1024.0f << "Kb / "
            << (float)(dynamincIbSize + staticIbSize + ibPoolsSize) / 1048576.0f << "Mb)" << "\n\n";

    stream->Close();

    return true;
}

// orig 0x62cbe0 ib_man_stats.cpp:232
void CDevice::UpdateIBMemStats()
{
    std::vector<CIndexBuffer*> ibPools;

    for (std::vector<IbHandle>::iterator it = m_IbPoolBuffers.begin(); it != m_IbPoolBuffers.end(); ++it)
    {
        ibPools.push_back(&m_ibs[IbId(*it)]);
    }

    m_devMemStats.DynamicIBCount = 0;
    m_devMemStats.DynamicIBSize = 0;
    m_devMemStats.StaticIBCount = 0;
    m_devMemStats.StaticIBSize = 0;
    m_devMemStats.IBPoolsSize = 0;

    for (unsigned int i = 0; i < m_ibs.size(); i++)
    {
        if (std::find(ibPools.begin(), ibPools.end(), &m_ibs[i]) == ibPools.end())
        {
            if (m_ibs[i].m_desc.Usage & D3DUSAGE_DYNAMIC)
            {
                m_devMemStats.DynamicIBSize += m_ibs[i].m_desc.Size;
                m_devMemStats.DynamicIBCount++;
            }
            else
            {
                m_devMemStats.StaticIBSize += m_ibs[i].m_desc.Size;
                m_devMemStats.StaticIBCount++;
            }
        }
        else
        {
            m_devMemStats.IBPoolsSize += m_ibs[i].m_desc.Size;
        }
    }
}
