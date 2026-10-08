#include "graph/hungarianalgo.h"

namespace Graph
{
    bool canAddChain(
        CSquareMatrix<float> const& costMatrix,
        std::vector<int> const& matchingX,
        std::vector<int> const& matchingY,
        int startVertexX,
        std::vector<int>* parentX,
        std::vector<int>* parentY,
        int* freeYVertex)
    {
        // RVA 0x948750 - breadth-first search from X vertex startVertexX for an augmenting path: along zero-weight
        // edges not in the matching to Y, and along matched edges back to X. Records the path in the parents (the
        // root's X parent is -2) and returns the free Y vertex reached.
        int const n = static_cast<int>(costMatrix.getSize());
        parentX->assign(n, -1);
        parentY->assign(n, -1);
        std::queue<TQueueVertex> queue;
        queue.push(TQueueVertex(true, startVertexX));
        (*parentX)[startVertexX] = -2;
        while (!queue.empty())
        {
            TQueueVertex const current = queue.front();
            queue.pop();
            if (current.inFirstPartite)
            {
                int const x = current.index;
                for (int y = 0; y < n; ++y)
                {
                    if (fabs(costMatrix(x, y)) < 1.0e-10 && (*parentY)[y] == -1 && matchingX[x] != y)
                    {
                        (*parentY)[y] = x;
                        queue.push(TQueueVertex(false, y));
                    }
                }
                continue;
            }
            int const y = current.index;
            int const matchedX = matchingY[y];
            if (matchedX == -1)
            {
                *freeYVertex = y;
                return true;
            }
            if ((*parentX)[matchedX] == -1)
            {
                (*parentX)[matchedX] = y;
                queue.push(TQueueVertex(true, matchedX));
            }
        }
        return false;
    }

    void NormalizeHungarianMatrix(CSquareMatrix<float>* m)
    {
        // RVA 0x948540 - subtracts each row's minimum from the row, then each column's from the column.
        // NOTE: the minimum search starts from 1e10, so entries above that are not reduced below it.
        int const size = static_cast<int>(m->getSize());
        for (int i = 0; i < size; ++i)
        {
            float minValue = 1.0e10f;
            for (int j = 0; j < size; ++j)
            {
                if (minValue > (*m)(i, j))
                {
                    minValue = (*m)(i, j);
                }
            }
            for (int j = 0; j < size; ++j)
            {
                (*m)(i, j) = (*m)(i, j) - minValue;
            }
        }
        for (int j = 0; j < size; ++j)
        {
            float minValue = 1.0e10f;
            for (int i = 0; i < size; ++i)
            {
                if (minValue > (*m)(i, j))
                {
                    minValue = (*m)(i, j);
                }
            }
            for (int i = 0; i < size; ++i)
            {
                (*m)(i, j) = (*m)(i, j) - minValue;
            }
        }
    }

    void addChain(
        std::vector<int>* matchingX,
        std::vector<int>* matchingY,
        std::vector<int> const& parentX,
        std::vector<int> const& parentY,
        int freeVertexIndex)
    {
        int currentY = freeVertexIndex;

        // Traverse the augmenting path backwards and flip the matching edges
        while (currentY != -2)
        {
            int parentXVertex = parentY[currentY];       // X vertex that leads to current Y
            int parentYVertex = parentX[parentXVertex];  // Y vertex that leads to parent X

            // Update matching: match parentXVertex with currentY
            (*matchingX)[parentXVertex] = currentY;
            (*matchingY)[currentY] = parentXVertex;

            // Move to the next vertex in the augmenting path
            currentY = parentYVertex;
        }
    }

    void applyOperation(CSquareMatrix<float>* m, std::vector<int> const& parentX, std::vector<int> const& parentY)
    {
        // RVA 0x9483E0 - finds the smallest weight between a reached X vertex and an unreached Y vertex, subtracts
        // it along those edges and adds it along the edges from unreached X to reached Y.
        int const size = static_cast<int>(m->getSize());
        float delta = 1.0e10f;
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                if (parentX[i] != -1 && parentY[j] == -1 && delta > (*m)(i, j))
                {
                    delta = (*m)(i, j);
                }
            }
        }
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                if (parentX[i] != -1 && parentY[j] == -1)
                {
                    (*m)(i, j) = (*m)(i, j) - delta;
                }
                if (parentX[i] == -1 && parentY[j] != -1)
                {
                    (*m)(i, j) = (*m)(i, j) + delta;
                }
            }
        }
    }

    TQueueVertex::TQueueVertex(bool inFirstPartite_, int index_)
    {
        this->inFirstPartite = inFirstPartite_;
        this->index = index_;
    }

    std::vector<int> getMaxValuedMatching(Graph::CSquareMatrix<float> const& m)
    {
        auto const size = m.getSize();
        Graph::CSquareMatrix<float> minusM(size);
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                minusM(i, j) = -m(i, j);
            }
        }

        return getMinValuedMatching(minusM);
    }

    std::vector<int> getMinValuedMatching(Graph::CSquareMatrix<float> m)
    {
        // RVA 0x948C10 - the Hungarian algorithm: for each X vertex, augment the matching along a zero-weight path,
        // adjusting the weights until one exists. Returns each X vertex's matched Y vertex.
        NormalizeHungarianMatrix(&m);
        int const size = static_cast<int>(m.getSize());
        std::vector<int> matchingX(size, -1);
        std::vector<int> matchingY(size, -1);
        for (int i = 0; i < size; ++i)
        {
            while (matchingX[i] == -1)
            {
                std::vector<int> parentX;
                std::vector<int> parentY;
                int freeVertexIndex = -1;
                if (canAddChain(m, matchingX, matchingY, i, &parentX, &parentY, &freeVertexIndex))
                {
                    addChain(&matchingX, &matchingY, parentX, parentY, freeVertexIndex);
                }
                else
                {
                    applyOperation(&m, parentX, parentY);
                }
            }
        }
        return matchingX;
    }

    float getMatchingValue(Graph::CSquareMatrix<float> const& m, std::vector<int> const& matching)
    {
        // RVA 0x948A20 - the total weight of the matched edges.
        // NOTE: the shipped code also counts how often each vertex is matched and never uses the counts.
        float value = 0.0f;
        for (size_t i = 0; i < matching.size(); ++i)
        {
            value = m(static_cast<int>(i), matching[i]) + value;
        }
        return value;
    }
}  // namespace Graph
