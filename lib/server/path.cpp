#include "path.h"
#include <core/ini.h>
#include <core/kernel.h>
#include <core/ref_ptr.h>
#include <core/timer.h>

#include <stdexcept>

#include "retruxx/common.h"

namespace ai
{
    namespace
    {
        // RVA 0xA03950 - the four straight moves come first, so an index >= 4 is a diagonal
        // and needs its two neighbouring cells checked as well.
        struct PathDirection
        {
            int m_dx;
            int m_dy;
            int m_weight;
        };

        int constexpr NUM_STRAIGHT_DIRS = 4;

        PathDirection const PathDir[] = {
            {1, 0, 10},
            {0, 1, 10},
            {-1, 0, 10},
            {0, -1, 10},
            {1, 1, 14},
            {-1, 1, 14},
            {-1, -1, 14},
            {1, -1, 14},
        };

        int constexpr NUM_DIRS = sizeof(PathDir) / sizeof(PathDir[0]);
    }  // namespace

    DHeap::DHeap(unsigned char PD, unsigned int PMaxSize, unsigned int* PHInverse, unsigned int* PKey)
    {
        // RVA 0x6A9BD0 - a D-ary heap over cell ids. pKey is the caller's distance array, so the
        // heap orders ids by a cost it does not own; pHInverse maps an id back to its slot, which
        // is what lets SiftUp be called on a node already in the heap.
        D = PD;
        pKey = PKey;
        pHInverse = PHInverse;
        MaxSize = PMaxSize;
        Size = 0;
        pH = new unsigned int[PMaxSize];
    }

    bool DHeap::FindMin(unsigned int* pMinItem)
    {
        // RVA 0x6A9D40
        if (!Size)
        {
            return false;
        }
        *pMinItem = pH[0];
        return true;
    }

    void DHeap::Insert(unsigned int Item)
    {
        // RVA 0x6A9D20 - NOTE: Size is incremented before the capacity test, so inserting into a
        // full heap drops the item but still grows Size past MaxSize.
        ++Size;
        if (Size <= MaxSize)
        {
            SiftUp(Item, Size - 1);
        }
    }

    void DHeap::SiftUp(unsigned int Item, unsigned int HeapPos)
    {
        // RVA 0x6A9C40
        unsigned int pos = HeapPos;
        for (unsigned int parent = HeapPos / D; parent; parent /= D)
        {
            if (pKey[pH[parent - 1]] <= pKey[Item])
            {
                break;
            }
            pH[pos] = pH[parent - 1];
            if (pHInverse)
            {
                pHInverse[pH[pos]] = pos;
            }
            pos = parent - 1;
        }
        pH[pos] = Item;
        if (pHInverse)
        {
            pHInverse[pH[pos]] = pos;
        }
    }

    unsigned int DHeap::MinChild(unsigned int HeapPos)
    {
        // RVA 0x6A9CC0 - the cheapest of this node's D children, or 0 when it has none.
        unsigned int result = HeapPos * D + 1;
        if (result >= Size)
        {
            return 0;
        }
        unsigned int const last = D * (HeapPos + 1);
        unsigned int best = static_cast<unsigned>(-1);
        for (unsigned int i = result; i <= last && i < Size; ++i)
        {
            if (pKey[pH[i]] < best)
            {
                result = i;
                best = pKey[pH[i]];
            }
        }
        return result;
    }

    bool DHeap::DeleteMin(unsigned int* pMinItem)
    {
        // RVA 0x6AA390
        if (!Size)
        {
            return false;
        }
        --Size;
        *pMinItem = pH[0];
        if (Size)
        {
            SiftDown(pH[Size], 0);
        }
        return true;
    }

    void DHeap::SiftDown(unsigned int Item, unsigned int HeapPos)
    {
        // RVA 0x6AA320
        unsigned int pos = HeapPos;
        for (unsigned int child = MinChild(HeapPos); child; child = MinChild(child))
        {
            if (pKey[pH[child]] >= pKey[Item])
            {
                break;
            }
            pH[pos] = pH[child];
            if (pHInverse)
            {
                pHInverse[pH[pos]] = pos;
            }
            pos = child;
        }
        pH[pos] = Item;
        if (pHInverse)
        {
            pHInverse[pH[pos]] = pos;
        }
    }

    DHeap::~DHeap()
    {
        // RVA 0x6A9C20 - NOTE: only pH is freed; pKey, pHInverse and Size are not owned here.
        delete[] pH;
        pH = nullptr;
    }

    void Path::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* OwnNode, ai::Map* pMap)
    {
        // RVA 0x7BF6B0 - an unfinished search is restarted from its original question; a finished
        // one has its points read straight back.
        int inSearching = 0;
        m3d::SafeIntAttrib(inSearching, OwnNode, "InSearching");
        m_bSearching = inSearching != 0;

        if (m_bSearching)
        {
            m_pMap = pMap;
            m3d::SafeVector2Attrib(m_startPoint, OwnNode, "Start");
            m3d::SafeVector2Attrib(m_endPoint, OwnNode, "End");
            m3d::SafeIntAttrib(m_radius, OwnNode, "radius");

            int blockedValue = m_blockedValue;
            m3d::SafeIntAttrib(blockedValue, OwnNode, "BV");
            m_blockedValue = static_cast<unsigned char>(blockedValue);

            m3d::SafeIntAttrib(m_minIndex.x, OwnNode, "RectLeft");
            m3d::SafeIntAttrib(m_minIndex.y, OwnNode, "RectTop");
            m3d::SafeIntAttrib(m_maxIndex.x, OwnNode, "RectRight");
            m3d::SafeIntAttrib(m_maxIndex.y, OwnNode, "RectBottom");

            m_bSearching = false;
            StartSearchInternal();
            return;
        }

        delete[] m_pPath;
        m_pPath = nullptr;

        CStr buf(OwnNode->GetAttribute("PathPoints"));
        if (buf.empty())
        {
            return;
        }

        // The points are one "x,y;" run, so the string is cut twice: first into points, then
        // each point into its pair.
        retruxx::vector<char*> tmpVect;
        // strtok writes into the string as it goes, which is what the shipped build does to the
        // CStr's own buffer; buf outlives the pointers taken into it.
        char* const raw = const_cast<char*>(buf.c_str());
        for (char* tok = strtok(raw, ";"); tok; tok = strtok(nullptr, ";"))
        {
            tmpVect.push_back(tok);
        }

        m_size = static_cast<unsigned>(tmpVect.size());
        m_pPath = new CVector2[m_size];
        for (unsigned i = 0; i < m_size; ++i)
        {
            m_pPath[i].x = static_cast<float>(atof(strtok(tmpVect[i], ",")));
            m_pPath[i].y = static_cast<float>(atof(strtok(nullptr, ",")));
        }
        m_startPoint = m_pPath[0];
        m_endPoint = m_pPath[m_size - 1];
    }

    void Path::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const
    {
        // RVA 0x7BE6C0 - a search still running is saved as the question that was asked; a
        // finished one is saved as the answer.
        OwnNode->SetAttribute("Map", "GLOBAL");
        if (m_bSearching)
        {
            OwnNode->SetAttribute("InSearching", "1");
            OwnNode->SetAttribute("Start",
                CStr::format_("%.3f %.3f", m_startPoint.x, m_startPoint.y).c_str());
            OwnNode->SetAttribute("End",
                CStr::format_("%.3f %.3f", m_endPoint.x, m_endPoint.y).c_str());
            OwnNode->SetAttribute("radius", CStr(m_radius).c_str());
            OwnNode->SetAttribute("BV", CStr(m_blockedValue).c_str());
            OwnNode->SetAttribute("RectLeft", CStr(m_minIndex.x).c_str());
            OwnNode->SetAttribute("RectTop", CStr(m_minIndex.y).c_str());
            OwnNode->SetAttribute("RectRight", CStr(m_maxIndex.x).c_str());
            OwnNode->SetAttribute("RectBottom", CStr(m_maxIndex.y).c_str());
            return;
        }

        OwnNode->SetAttribute("InSearching", "0");
        if (m_size == 0)
        {
            return;
        }

        CStr buff;
        for (unsigned i = 0; i < m_size; ++i)
        {
            buff += CStr(m_pPath[i].x) + CStr(",") + CStr(m_pPath[i].y) + CStr(";");
        }
        // The trailing separator is trimmed back off.
        OwnNode->SetAttribute("PathPoints", buff.substr(0, buff.length() - 1).c_str());
    }

    Path::Path(const std::vector<CVector2, std::allocator<CVector2>>& path)
        : m_startPointIndex(0, 0)
        , m_endPointIndex(0, 0)
        , m_minIndex(0, 0)
        , m_maxIndex(0, 0)
    {
        this->m_size = 0;
        this->m_pPath = 0;
        this->pHeap = 0;
        this->pDist = 0;
        this->pHInverse = 0;
        this->pFlag = 0;
        this->pParent = 0;
        this->pLength = 0;
        this->m_bSearching = 0;
        this->m_blockedValue = -1;

        m_size = path.size();
        m_pPath = new CVector2[m_size];
        for (int i = 0 ; i < m_size ; ++i)
        {
            m_pPath[i] = path[i];
        }
    }

    Path::Path()
        : m_startPointIndex(0, 0)
        , m_endPointIndex(0, 0)
        , m_minIndex(0, 0)
        , m_maxIndex(0, 0)
    {
        this->m_size = 0;
        this->m_pPath = 0;
        this->pHeap = 0;
        this->pDist = 0;
        this->pHInverse = 0;
        this->pFlag = 0;
        this->pParent = 0;
        this->pLength = 0;
        this->m_bSearching = 0;
        this->m_blockedValue = -1;
    }

    Path::~Path()
    {
        delete[] m_pPath;
        ClearHelpers();
    }

    void Path::StartSearch(ai::Map* pMap, const CVector2& startPoint, const CVector2& endPoint, float radius,
        unsigned char blockedValue)
    {
        // RVA 0x7BED00 - searches the whole map. A search already under way is left alone.
        if (m_bSearching)
        {
            return;
        }
        m_blockedValue = blockedValue;
        m_bSearching = true;
        m_SearchResult = PATH_NOT_ENOUGH_TIME;
        m_pMap = pMap;
        // The radius is in cells, measured against the smaller of the two cell dimensions.
        m_radius = static_cast<int>(radius /
            (pMap->m_cellSize.x <= pMap->m_cellSize.y ? pMap->m_cellSize.x : pMap->m_cellSize.y));
        m_startPoint = startPoint;
        m_endPoint = endPoint;
        m_minIndex.x = 0;
        m_minIndex.y = 0;
        m_maxIndex = pMap->m_lastIndex;
        StartSearchInternal();
    }

    void Path::StartSearchInRect(ai::Map* pMap, const CVector2& startPoint, const CVector2& endPoint,
        const CVector2& rectPoint1, const CVector2& rectPoint2, float radius, unsigned char blockedValue)
    {
        // RVA 0x7BD600 - as StartSearch, but confined to a rectangle of the map.
        if (m_bSearching)
        {
            return;
        }
        m_blockedValue = blockedValue;
        m_bSearching = true;
        m_SearchResult = PATH_NOT_ENOUGH_TIME;
        m_pMap = pMap;
        m_radius = static_cast<int>(radius /
            (pMap->m_cellSize.x <= pMap->m_cellSize.y ? pMap->m_cellSize.x : pMap->m_cellSize.y));

        // NOTE: the end point indices are computed from the PREVIOUS search's points, because
        // m_startPoint and m_endPoint are only assigned afterwards. StartSearchInternal recomputes
        // them from the new points, so the values written here are dead.
        m_startPointIndex.x = static_cast<int>(m_startPoint.x / pMap->m_cellSize.x);
        m_startPointIndex.y = static_cast<int>(m_startPoint.y / pMap->m_cellSize.y);
        m_endPointIndex.x = static_cast<int>(m_endPoint.x / m_pMap->m_cellSize.x);
        m_endPointIndex.y = static_cast<int>(m_endPoint.y / m_pMap->m_cellSize.y);

        m_startPoint = startPoint;
        m_endPoint = endPoint;

        m_minIndex.x = static_cast<int>(rectPoint1.x / m_pMap->m_cellSize.x);
        m_minIndex.y = static_cast<int>(rectPoint1.y / m_pMap->m_cellSize.y);
        m_maxIndex.x = static_cast<int>(rectPoint2.x / m_pMap->m_cellSize.x);
        m_maxIndex.y = static_cast<int>(rectPoint2.y / m_pMap->m_cellSize.y);

        // The two corners may be given either way round, and the result is clipped to the map.
        if (m_minIndex.x > m_maxIndex.x)
        {
            std::swap(m_minIndex.x, m_maxIndex.x);
        }
        if (m_minIndex.y > m_maxIndex.y)
        {
            std::swap(m_minIndex.y, m_maxIndex.y);
        }
        if (m_minIndex.x < 0)
        {
            m_minIndex.x = 0;
        }
        if (m_minIndex.y < 0)
        {
            m_minIndex.y = 0;
        }
        if (m_maxIndex.x > m_pMap->m_lastIndex.x)
        {
            m_maxIndex.x = m_pMap->m_lastIndex.x;
        }
        if (m_maxIndex.y > m_pMap->m_lastIndex.y)
        {
            m_maxIndex.y = m_pMap->m_lastIndex.y;
        }
        StartSearchInternal();
    }

    void Path::ProceedSearch1(unsigned int msInterval)
    {
        // RVA 0x7BE310 - Dijkstra over the cell grid, weighted by the map's own cell values so
        // that rough ground costs more than smooth. Runs for at most msInterval and returns,
        // leaving m_bSearching set until it is done.
        unsigned int const begTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
        unsigned int curTime = begTime;
        bool fTarget = false;

        while (curTime - begTime < msInterval)
        {
            int const cx = v % m_maxIndex.x;
            int const cy = v / m_maxIndex.x;

            for (int dir = 0; dir < NUM_DIRS; ++dir)
            {
                // A diagonal may not cut a corner between two blocked cells.
                bool const fDiagBad = dir >= NUM_STRAIGHT_DIRS &&
                    (m_pMap->GetValue(cx + m_minIndex.x, cy + m_minIndex.y + PathDir[dir].m_dy) >=
                            m_blockedValue ||
                        m_pMap->GetValue(cx + m_minIndex.x + PathDir[dir].m_dx, cy + m_minIndex.y) >=
                            m_blockedValue);

                int const nx = cx + PathDir[dir].m_dx;
                int const ny = cy + PathDir[dir].m_dy;
                if (nx < 0 || nx >= m_maxIndex.x || ny < 0 || ny >= m_maxIndex.y || fDiagBad ||
                    m_pMap->GetValue(nx + m_minIndex.x, ny + m_minIndex.y) >= m_blockedValue)
                {
                    continue;
                }

                w = nx + ny * m_maxIndex.x;
                unsigned int const newDist = PathDir[dir].m_weight + pDist[v] +
                    m_pMap->GetValue(nx + m_minIndex.x, ny + m_minIndex.y);
                if (newDist >= pDist[w])
                {
                    continue;
                }
                pDist[w] = newDist;
                pParent[w] = v;
                pLength[w] = pLength[v] + 1;
                if (pFlag[w])
                {
                    pHeap->SiftUp(w, pHInverse[w]);
                }
                else
                {
                    pFlag[w] = 1;
                    pHeap->Insert(w);
                }
            }

            bool fProceed = pHeap->DeleteMin(&v);
            pFlag[v] = 0;

            // The closest node reached so far is remembered, so an unreachable target still
            // yields a path that gets as near as it can.
            int const px = v % m_maxIndex.x;
            int const py = v / m_maxIndex.x;
            unsigned int const distToEnd = (m_endPointIndex.y - py) * (m_endPointIndex.y - py) +
                (m_endPointIndex.x - px) * (m_endPointIndex.x - px);
            if (distToEnd < m_minDist)
            {
                m_minDist = distToEnd;
                m_idOfClosestPoint = v;
            }
            if (m_endPointIndex.x == px && m_endPointIndex.y == py)
            {
                fProceed = false;
                fTarget = true;
            }

            curTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
            if (!fProceed)
            {
                _BuildPathFromParents(m_idOfClosestPoint, fTarget);
                ClearHelpers();
                m_bSearching = false;
                return;
            }
        }
    }

    void Path::ProceedSearch2(unsigned int msInterval)
    {
        // RVA 0x7BDFF0 - greedy best-first: the heap is keyed on distance to the target rather
        // than distance travelled, and blocking is tested through the vehicle radius.
        unsigned int const begTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
        unsigned int curTime = begTime;

        while (curTime - begTime < msInterval)
        {
            for (int dir = 0; dir < NUM_DIRS; ++dir)
            {
                int const cx = v % m_maxIndex.x;
                int const cy = v / m_maxIndex.x;

                bool const fDiagBad = dir >= NUM_STRAIGHT_DIRS &&
                    (m_pMap->IsBlocked(m_minIndex.x + cx + PathDir[dir].m_dx, cy + m_minIndex.y, dir,
                         m_radius, m_blockedValue) ||
                        m_pMap->IsBlocked(cx + m_minIndex.x, m_minIndex.y + cy + PathDir[dir].m_dy, dir,
                            m_radius, m_blockedValue));

                int const nx = cx + PathDir[dir].m_dx;
                int const ny = cy + PathDir[dir].m_dy;
                if (nx < 0 || nx >= m_maxIndex.x || ny < 0 || ny >= m_maxIndex.y || fDiagBad ||
                    m_pMap->IsBlocked(
                        nx + m_minIndex.x, ny + m_minIndex.y, dir, m_radius, m_blockedValue))
                {
                    continue;
                }

                w = ny * m_maxIndex.x + nx;
                if (pFlag[w])
                {
                    continue;
                }
                pDist[w] = (m_endPointIndex.y - ny) * (m_endPointIndex.y - ny) +
                    (m_endPointIndex.x - nx) * (m_endPointIndex.x - nx);
                pParent[w] = v;
                pLength[w] = pLength[v] + 1;
                pFlag[w] = 1;
                pHeap->Insert(w);
            }

            bool fProceed = pHeap->DeleteMin(&v);
            if (m_endPointIndex.x == static_cast<int>(v % m_maxIndex.x) &&
                m_endPointIndex.y == static_cast<int>(v / m_maxIndex.x))
            {
                fProceed = false;
            }

            curTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
            if (!fProceed)
            {
                // NOTE: unlike ProceedSearch1 this always rebuilds from the target cell, even when
                // the search ran out of nodes without reaching it, and always reports PATH_OK.
                unsigned int const endNode =
                    m_endPointIndex.x + m_maxIndex.x * m_endPointIndex.y;
                _BuildPathFromParents(endNode, true);
                ClearHelpers();
                m_bSearching = false;
                return;
            }
        }
    }

    void Path::ProceedSearch3(unsigned int msInterval)
    {
        // RVA 0x7BF0A0 - ProceedSearch2 followed by a smoothing pass that drops every point the
        // path can see past, using a plain 4-connected line test.
        ProceedSearch2(msInterval);
        if (m_bSearching || m_size == 0)
        {
            return;
        }

        // Skip forward to the first point that is not itself blocked.
        int curPos = 0;
        int x1 = 0;
        int y1 = 0;
        for (unsigned i = 0; i < m_size; ++i)
        {
            x1 = static_cast<int>(m_pPath[i].x / m_pMap->m_cellSize.x);
            y1 = static_cast<int>(m_pPath[i].y / m_pMap->m_cellSize.y);
            curPos = i;
            if (!m_pMap->IsLine4Blocked(x1, y1, x1, y1, m_blockedValue))
            {
                break;
            }
        }

        int outPos = curPos + 1;
        while (curPos < static_cast<int>(m_size) - 1)
        {
            // Reach as far along the path as stays visible from here.
            unsigned int last = m_size - 1;
            unsigned int j = curPos + 1;
            for (; j < m_size; ++j)
            {
                if (m_pMap->IsLine4Blocked(x1, y1,
                        static_cast<int>(m_pPath[j].x / m_pMap->m_cellSize.x),
                        static_cast<int>(m_pPath[j].y / m_pMap->m_cellSize.y), m_blockedValue))
                {
                    last = j - 1;
                    break;
                }
            }

            // Nothing was visible at all, so take the very next point regardless.
            if (static_cast<int>(last) == curPos)
            {
                ++last;
            }
            m_pPath[outPos] = m_pPath[last];
            x1 = static_cast<int>(m_pPath[last].x / m_pMap->m_cellSize.x);
            y1 = static_cast<int>(m_pPath[last].y / m_pMap->m_cellSize.y);
            ++outPos;
            curPos = last;
        }
        m_size = outPos;
    }

    void Path::ProceedSearch4(unsigned int msInterval)
    {
        // RVA 0x7BEDA0 - Dijkstra, then each straight run is replaced by evenly spaced points
        // along it, then the whole path is smoothed by FilterPath.
        ProceedSearch1(msInterval);
        if (m_bSearching)
        {
            return;
        }

        if (m_size != 0)
        {
            MapIndex p1(0, 0);
            MapIndex p2(0, 0);

            int curPos = 0;
            for (unsigned i = 0; i < m_size; ++i)
            {
                p1.x = static_cast<int>(m_pPath[i].x / m_pMap->m_cellSize.x);
                p1.y = static_cast<int>(m_pPath[i].y / m_pMap->m_cellSize.y);
                curPos = i;
                if (!m_pMap->IsSolidBlocked(p1, p1, m_radius + 1, m_blockedValue))
                {
                    break;
                }
            }

            while (curPos < static_cast<int>(m_size) - 1)
            {
                // NOTE: the visibility test uses the map value under the current point as its
                // threshold rather than m_blockedValue, so how far the path may be straightened
                // depends on the terrain it is standing on.
                unsigned char const checkValue = m_pMap->GetValue(
                    static_cast<int>(m_pPath[curPos].x), static_cast<int>(m_pPath[curPos].y));

                unsigned int last = m_size - 1;
                for (unsigned j = curPos + 1; j < m_size; ++j)
                {
                    p2.x = static_cast<int>(m_pPath[j].x / m_pMap->m_cellSize.x);
                    p2.y = static_cast<int>(m_pPath[j].y / m_pMap->m_cellSize.y);
                    if (m_pMap->IsSolidBlocked(p1, p2, m_radius + 1, checkValue))
                    {
                        last = j - 1;
                        break;
                    }
                }

                CVector2 const* anchor = nullptr;
                if (static_cast<int>(last) == curPos)
                {
                    anchor = &m_pPath[last + 1];
                    curPos = last + 1;
                }
                else
                {
                    // Spread the points between the two ends evenly over the straight run.
                    float const dx = m_pPath[last].x - m_pPath[curPos].x;
                    float const dy = m_pPath[last].y - m_pPath[curPos].y;
                    float const span = static_cast<float>(last - curPos);
                    for (unsigned k = curPos + 1; k < last; ++k)
                    {
                        float const t = static_cast<float>(k - curPos);
                        m_pPath[k].x = m_pPath[curPos].x + t * dx / span;
                        m_pPath[k].y = m_pPath[curPos].y + t * dy / span;
                    }
                    anchor = &m_pPath[last];
                    curPos = last;
                }
                p1.x = static_cast<int>(anchor->x / m_pMap->m_cellSize.x);
                p1.y = static_cast<int>(anchor->y / m_pMap->m_cellSize.y);
            }
        }

        if (!m_bSearching)
        {
            FilterPath();
        }
    }

    void Path::ProceedSearch5(unsigned int msInterval)
    {
        // RVA 0x7BDAF0 - the same greedy search as ProceedSearch2 but, unlike it, honest about
        // failure: it falls back on the closest node reached and reports PATH_POS_UNREACHABLE.
        // The straightening pass afterwards is StraightenPath's, repeated inline.
        unsigned int const begTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
        unsigned int curTime = begTime;

        unsigned int closestNode = v;
        unsigned int minDist = (m_endPointIndex.y - v / m_maxIndex.x) *
                (m_endPointIndex.y - v / m_maxIndex.x) +
            (m_endPointIndex.x - v % m_maxIndex.x) * (m_endPointIndex.x - v % m_maxIndex.x);
        bool fProceed = true;
        bool fTarget = false;

        do
        {
            if (curTime - begTime >= msInterval)
            {
                return;
            }

            for (int dir = 0; dir < NUM_DIRS; ++dir)
            {
                int const cx = v % m_maxIndex.x;
                int const cy = v / m_maxIndex.x;

                bool const fDiagBad = dir >= NUM_STRAIGHT_DIRS &&
                    (m_pMap->IsBlocked(m_minIndex.x + cx + PathDir[dir].m_dx, cy + m_minIndex.y, dir,
                         m_radius, m_blockedValue) ||
                        m_pMap->IsBlocked(cx + m_minIndex.x, cy + PathDir[dir].m_dy + m_minIndex.y, dir,
                            m_radius, m_blockedValue));

                int const nx = cx + PathDir[dir].m_dx;
                int const ny = cy + PathDir[dir].m_dy;
                if (nx < 0 || nx >= m_maxIndex.x || ny < 0 || ny >= m_maxIndex.y || fDiagBad ||
                    m_pMap->IsBlocked(
                        nx + m_minIndex.x, ny + m_minIndex.y, dir, m_radius, m_blockedValue))
                {
                    continue;
                }

                w = ny * m_maxIndex.x + nx;
                if (pFlag[w])
                {
                    continue;
                }
                pDist[w] = (m_endPointIndex.y - ny) * (m_endPointIndex.y - ny) +
                    (m_endPointIndex.x - nx) * (m_endPointIndex.x - nx);
                pParent[w] = v;
                pLength[w] = pLength[v] + 1;
                pFlag[w] = 1;
                pHeap->Insert(w);
            }

            fProceed = pHeap->DeleteMin(&v);
            int const px = v % m_maxIndex.x;
            int const py = v / m_maxIndex.x;
            unsigned int const distToEnd = (m_endPointIndex.y - py) * (m_endPointIndex.y - py) +
                (m_endPointIndex.x - px) * (m_endPointIndex.x - px);
            if (distToEnd < minDist)
            {
                minDist = distToEnd;
                closestNode = v;
            }
            if (m_endPointIndex.x == px && m_endPointIndex.y == py)
            {
                fProceed = false;
                fTarget = true;
            }
            curTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
        } while (fProceed);

        _BuildPathFromParents(closestNode, fTarget);
        ClearHelpers();
        m_bSearching = false;
        StraightenPath(false);
    }

    void Path::ProceedSearchAStarStraightment(unsigned int msInterval)
    {
        // RVA 0x7BD770 - the greedy search of ProceedSearch5 followed by StraightenPath, which is
        // what makes the result look like a route rather than a staircase of cells.
        ProceedSearch5(msInterval);
    }

    void Path::ProceedSearchDijkstraStraightment(unsigned int msInterval)
    {
        // RVA 0x7BED80 - the weighted search, straightened with the terrain-aware test.
        ProceedSearch1(msInterval);
        if (!m_bSearching)
        {
            StraightenPath(true);
        }
    }

    float Path::GetPathLength(int PathNum) const
    {
        // RVA 0x7BD160 - the length of what is left of the path from PathNum onwards.
        // NOTE: the loop runs one point past the end, so the segment beyond the last point is
        // measured against whatever follows it in memory.
        if (static_cast<unsigned>(PathNum) >= m_size)
        {
            return 0.0f;
        }
        float result = 0.0f;
        for (unsigned i = PathNum; i < m_size; ++i)
        {
            float const dx = m_pPath[i + 1].x - m_pPath[i].x;
            float const dy = m_pPath[i + 1].y - m_pPath[i].y;
            result += sqrtf(dx * dx + dy * dy);
        }
        return result;
    }

    bool Path::GetSearchStatus() const
    {
        return this->m_bSearching;
    }

    ai::eSearchResult Path::GetResult() const
    {
        // RVA 0x655AA0
        return m_SearchResult;
    }

    unsigned int Path::GetSize() const
    {
        return this->m_size;
    }

    void Path::GetItem(unsigned int ItemNum, float* PathCoordx, float* PathCoordy) const
    {
        if (ItemNum < m_size)
        {
            const auto& coord = m_pPath[ItemNum];
            *PathCoordx = coord.x;
            *PathCoordy = coord.y;
        }
    }

    bool Path::IsStraightPathValid(ai::Map* pMap, const CVector2& startPoint, const CVector2& endPoint, float radius,
        unsigned char blockedValue)
    {
        // RVA 0x7BCD70 - if the two points can see each other there is nothing to search for, and
        // the path becomes the pair of points.
        if (m_bSearching)
        {
            return false;
        }
        m_blockedValue = blockedValue;
        m_pMap = pMap;
        m_radius = static_cast<int>(radius /
            (pMap->m_cellSize.x <= pMap->m_cellSize.y ? pMap->m_cellSize.x : pMap->m_cellSize.y));

        m_startPointIndex.x = static_cast<int>(startPoint.x / pMap->m_cellSize.x);
        m_startPointIndex.y = static_cast<int>(startPoint.y / pMap->m_cellSize.y);
        m_endPointIndex.x = static_cast<int>(endPoint.x / m_pMap->m_cellSize.x);
        m_endPointIndex.y = static_cast<int>(endPoint.y / m_pMap->m_cellSize.y);

        if (m_pMap->IsSolidBlocked(m_startPointIndex, m_endPointIndex, m_radius, m_blockedValue))
        {
            return false;
        }

        m_bSearching = false;
        m_SearchResult = PATH_OK;
        delete[] m_pPath;
        m_size = 2;
        m_pPath = new CVector2[2];
        m_startPoint = startPoint;
        m_pPath[0] = startPoint;
        m_endPoint = endPoint;
        m_pPath[1] = endPoint;
        return true;
    }

    void Path::_Init()
    {
        // RVA 0x7BC860
        m_size = 0;
        m_pPath = nullptr;
        pHeap = nullptr;
        pDist = nullptr;
        pHInverse = nullptr;
        pFlag = nullptr;
        pParent = nullptr;
        pLength = nullptr;
        m_bSearching = false;
        m_blockedValue = 0xFF;
    }

    void Path::StartSearchInternal()
    {
        // RVA 0x7BD3A0 - sets up the working arrays for a search, or finishes immediately when
        // the answer needs no search at all.
        m_bSearching = true;
        m_SearchResult = PATH_NOT_ENOUGH_TIME;
        m_size = 0;
        delete[] m_pPath;
        m_pPath = nullptr;

        m_startPointIndex.x = static_cast<int>(m_startPoint.x / m_pMap->m_cellSize.x);
        m_startPointIndex.y = static_cast<int>(m_startPoint.y / m_pMap->m_cellSize.y);
        m_endPointIndex.x = static_cast<int>(m_endPoint.x / m_pMap->m_cellSize.x);
        m_endPointIndex.y = static_cast<int>(m_endPoint.y / m_pMap->m_cellSize.y);

        if (m_startPointIndex.x < m_minIndex.x || m_startPointIndex.x >= m_maxIndex.x ||
            m_startPointIndex.y < m_minIndex.y || m_startPointIndex.y >= m_maxIndex.y ||
            m_endPointIndex.x < m_minIndex.x || m_endPointIndex.x >= m_maxIndex.x ||
            m_endPointIndex.y < m_minIndex.y || m_endPointIndex.y >= m_maxIndex.y)
        {
            m_bSearching = false;
            m_SearchResult = PATH_POS_UNREACHABLE;
            return;
        }

        ClearHelpers();

        // Both ends in the same passable cell: the path is just the two points.
        if (m_startPointIndex.x == m_endPointIndex.x && m_startPointIndex.y == m_endPointIndex.y &&
            m_pMap->GetValue(m_endPointIndex.x, m_endPointIndex.y) < m_blockedValue)
        {
            m_size = 2;
            m_pPath = new CVector2[2];
            m_pPath[0] = m_startPoint;
            m_pPath[1] = m_endPoint;
            m_bSearching = false;
            m_SearchResult = PATH_OK;
            return;
        }

        // From here on every index is relative to the search rectangle's corner.
        m_startPointIndex.x -= m_minIndex.x;
        m_startPointIndex.y -= m_minIndex.y;
        m_endPointIndex.x -= m_minIndex.x;
        m_endPointIndex.y -= m_minIndex.y;
        m_maxIndex.x -= m_minIndex.x;
        m_maxIndex.y -= m_minIndex.y;

        int const numCells = m_maxIndex.x * m_maxIndex.y;
        pDist = new unsigned int[numCells];
        pHInverse = new unsigned int[numCells];
        pFlag = new unsigned char[numCells];
        pParent = new long[numCells];
        pLength = new long[numCells];

        v = m_startPointIndex.x + m_maxIndex.x * m_startPointIndex.y;
        m_idOfClosestPoint = v;
        int const dx = m_endPointIndex.x - m_startPointIndex.x;
        int const dy = m_endPointIndex.y - m_startPointIndex.y;
        m_minDist = dx * dx + dy * dy;

        for (int i = 0; i < numCells; ++i)
        {
            pDist[i] = static_cast<unsigned>(-1);
            pParent[i] = -1;
            pFlag[i] = 0;
        }
        pDist[v] = 0;
        pFlag[v] = 1;
        pLength[v] = 1;

        pHeap = new DHeap(2, numCells, pHInverse, pDist);
    }

    void Path::_BuildPathFromParents(unsigned int endNode, bool fTarget)
    {
        // Common tail of every search: walk the parent chain back from the node the search
        // settled on, writing cell centres into m_pPath from the end backwards. Not a function
        // in the shipped build, which repeats it inline in each ProceedSearch.
        float const cellSizeX = m_pMap->m_cellSize.x;
        float const cellSizeY = m_pMap->m_cellSize.y;

        v = endNode;
        w = endNode;
        delete[] m_pPath;
        m_pPath = nullptr;

        if (pParent[w] == -1)
        {
            m_size = 0;
            m_SearchResult = PATH_POS_UNREACHABLE;
            return;
        }

        m_size = pLength[w];
        m_pPath = new CVector2[m_size];
        for (int i = m_size - 1; v != static_cast<unsigned>(-1); --i)
        {
            m_pPath[i].x = (static_cast<int>(v % m_maxIndex.x) + m_minIndex.x + 0.5f) * cellSizeX;
            m_pPath[i].y = (static_cast<int>(v / m_maxIndex.x) + m_minIndex.y + 0.5f) * cellSizeY;
            v = pParent[v];
        }

        // The two ends are snapped back to the exact points that were asked for, rather than
        // left on the centres of their cells.
        m_pPath[0] = m_startPoint;
        if (!fTarget)
        {
            m_SearchResult = PATH_POS_UNREACHABLE;
            return;
        }
        m_pPath[m_size - 1] = m_endPoint;
        m_SearchResult = PATH_OK;
    }

    void Path::ClearHelpers()
    {
        // RVA 0x7BD300 - the working set of a search is large, so it is released the moment
        // the search finishes rather than being kept alive with the path.
        delete pHeap;
        pHeap = nullptr;

        delete[] pDist;
        pDist = nullptr;

        delete[] pHInverse;
        pHInverse = nullptr;

        delete[] pParent;
        pParent = nullptr;

        delete[] pFlag;
        pFlag = nullptr;

        delete[] pLength;
        pLength = nullptr;
    }

    unsigned int Path::MapIndexToId(const ai::MapIndex& v) const
    {
        // RVA 0x7BC800 - the search grid is addressed row-major within the search rectangle.
        return v.x + v.y * m_maxIndex.x;
    }

    void Path::IdToMapIndex(unsigned int id, ai::MapIndex& res) const
    {
        // RVA 0x7BC810
        res.x = id % m_maxIndex.x;
        res.y = id / m_maxIndex.x;
    }

    void Path::StraightenPath(bool bForDijkstra)
    {
        // RVA 0x7BC890 - drops every point the path can see past, so a staircase of cells becomes
        // a handful of corners.
        if (m_size == 0)
        {
            return;
        }

        MapIndex p1(0, 0);
        MapIndex p2(0, 0);

        // Skip forward to the first point that is not itself blocked.
        int curPos = 0;
        for (unsigned i = 0; i < m_size; ++i)
        {
            p1.x = static_cast<int>(m_pPath[i].x / m_pMap->m_cellSize.x);
            p1.y = static_cast<int>(m_pPath[i].y / m_pMap->m_cellSize.y);
            curPos = i;
            if (!m_pMap->IsSolidBlocked(p1, p1, m_radius, m_blockedValue))
            {
                break;
            }
        }

        int outPos = curPos + 1;
        while (curPos < static_cast<int>(m_size) - 1)
        {
            // For a weighted search the threshold is the terrain the path is standing on, so it
            // will not straighten out of rough ground onto smooth; otherwise it is the blocking
            // value the search itself used.
            unsigned char const checkValue = bForDijkstra
                ? m_pMap->GetValue(
                      static_cast<int>(m_pPath[curPos].x), static_cast<int>(m_pPath[curPos].y))
                : m_blockedValue;

            unsigned int last = m_size - 1;
            for (unsigned j = curPos + 1; j < m_size; ++j)
            {
                p2.x = static_cast<int>(m_pPath[j].x / m_pMap->m_cellSize.x);
                p2.y = static_cast<int>(m_pPath[j].y / m_pMap->m_cellSize.y);
                if (m_pMap->IsSolidBlocked(p1, p2, m_radius, checkValue))
                {
                    last = j - 1;
                    break;
                }
            }

            if (static_cast<int>(last) == curPos)
            {
                ++last;
            }
            m_pPath[outPos] = m_pPath[last];
            p1.x = static_cast<int>(m_pPath[last].x / m_pMap->m_cellSize.x);
            p1.y = static_cast<int>(m_pPath[last].y / m_pMap->m_cellSize.y);
            ++outPos;
            curPos = last;
        }
        m_size = outPos;
    }

    void Path::FilterPath()
    {
        // RVA 0x7BCEC0 - a smoothing pass: each point becomes the average of a window of its
        // neighbours, with the window shrinking towards both ends so the endpoints stay put.
        if (m_size <= 2)
        {
            return;
        }

        CVector2* const pNewPath = new CVector2[m_size];
        pNewPath[0] = m_pPath[0];

        // The second point averages the first three.
        float sumX = 0.0f;
        float sumY = 0.0f;
        for (int i = 0; i < 3; ++i)
        {
            sumX += m_pPath[i].x;
            sumY += m_pPath[i].y;
        }
        pNewPath[1].x = sumX * 0.33333334f;
        pNewPath[1].y = sumY * 0.33333334f;

        // The body of the path averages a window of five.
        for (unsigned i = 2; i < m_size - 2; ++i)
        {
            float windowX = 0.0f;
            float windowY = 0.0f;
            for (unsigned k = i - 2; k <= i + 2; ++k)
            {
                windowX += m_pPath[k].x;
                windowY += m_pPath[k].y;
            }
            pNewPath[i].x = windowX * 0.2f;
            pNewPath[i].y = windowY * 0.2f;
        }

        // Towards the end the window shrinks so that it never runs past the last point.
        for (unsigned i = m_size - 2; i < m_size; ++i)
        {
            unsigned int const first = 2 * i - m_size + 1;
            float tailX = 0.0f;
            float tailY = 0.0f;
            for (unsigned k = first; k < m_size; ++k)
            {
                tailX += m_pPath[k].x;
                tailY += m_pPath[k].y;
            }
            float const inv = 1.0f / (2 * (m_size - i) - 1);
            pNewPath[i].x = inv * tailX;
            pNewPath[i].y = inv * tailY;
        }

        pNewPath[m_size - 1] = m_pPath[m_size - 1];
        delete[] m_pPath;
        m_pPath = pNewPath;
    }
}
