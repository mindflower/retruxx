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
        // TODO: generated code canAddChain
        size_t const n = costMatrix.getSize();

        // Reset parent arrays to unvisited state
        parentX->assign(n, -1);
        parentY->assign(n, -1);

        std::queue<TQueueVertex> bfsQueue;

        // Start BFS from the given X vertex
        bfsQueue.push({true, startVertexX});
        (*parentX)[startVertexX] = -2;  // Special marker for root

        while (!bfsQueue.empty())
        {
            TQueueVertex current = bfsQueue.front();
            bfsQueue.pop();

            if (current.inFirstPartite)
            {
                // We're at an X vertex - look for edges to Y vertices
                int const x = current.index;

                for (int y = 0; y < static_cast<int>(n); ++y)
                {
                    // Check for zero-cost edge (within tolerance)
                    if (std::fabs(costMatrix(x, y)) < 1.0e-10f)
                    {
                        // Check if this Y vertex is unvisited and the edge is not in current matching
                        if ((*parentY)[y] == -1 && matchingX[x] != y)
                        {
                            (*parentY)[y] = x;  // Record that we reached Y from X
                            bfsQueue.push({false, y});
                        }
                    }
                }
            }
            else
            {
                // We're at a Y vertex
                int const y = current.index;

                if (matchingY[y] == -1)
                {
                    // Found a free Y vertex - we have an augmenting path!
                    *freeYVertex = y;
                    return true;
                }
                else
                {
                    // This Y vertex is matched to some X vertex
                    int const matchedX = matchingY[y];

                    if ((*parentX)[matchedX] == -1)
                    {
                        (*parentX)[matchedX] = y;  // Record that we reached X from Y
                        bfsQueue.push({true, matchedX});
                    }
                }
            }
        }

        // No augmenting path found from this starting vertex
        return false;
    }

    void NormalizeHungarianMatrix(CSquareMatrix<float>* m)
    {
        // TODO: generated code NormalizeHungarianMatrix
        if (!m || m->getSize() == 0)
        {
            return;
        }

        size_t size = m->getSize();

        // First pass: normalize rows
        for (size_t i = 0; i < size; ++i)
        {
            // Find minimum value in the current row
            float minVal = std::numeric_limits<float>::max();
            for (size_t j = 0; j < size; ++j)
            {
                if ((*m)(i, j) < minVal)
                {
                    minVal = (*m)(i, j);
                }
            }

            // Subtract the minimum value from each element in the row
            for (size_t j = 0; j < size; ++j)
            {
                (*m)(i, j) -= minVal;
            }
        }

        // Second pass: normalize columns
        for (size_t j = 0; j < size; ++j)
        {
            // Find minimum value in the current column
            float minVal = std::numeric_limits<float>::max();
            for (size_t i = 0; i < size; ++i)
            {
                if ((*m)(i, j) < minVal)
                {
                    minVal = (*m)(i, j);
                }
            }

            // Subtract the minimum value from each element in the column
            for (size_t i = 0; i < size; ++i)
            {
                (*m)(i, j) -= minVal;
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

    void applyOperation(CSquareMatrix<float>*, std::vector<int> const&, std::vector<int> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        // TODO: generated code getMinValuedMatching
        // Normalize the matrix for Hungarian algorithm
        NormalizeHungarianMatrix(&m);

        // Initialize matchings with -1 (unmatched)
        std::vector<int> matchingX(m.getSize(), -1);
        std::vector<int> matchingY(m.getSize(), -1);

        // Try to find matches for each vertex in X
        for (size_t i = 0; i < m.getSize(); ++i)
        {
            if (matchingX[i] == -1)
            {
                bool matched = false;

                // Keep trying until this vertex is matched
                while (!matched)
                {
                    std::vector<int> parentX(m.getSize(), -1);
                    std::vector<int> parentY(m.getSize(), -1);
                    int freeVertexIndex = -1;

                    // Try to find an augmenting path
                    if (canAddChain(m, matchingX, matchingY, static_cast<int>(i), &parentX, &parentY, &freeVertexIndex))
                    {
                        // Found augmenting path, update matching
                        addChain(&matchingX, &matchingY, parentX, parentY, freeVertexIndex);
                        matched = true;
                    }
                    else
                    {
                        // No augmenting path found, apply Hungarian operation
                        applyOperation(&m, parentX, parentY);
                    }

                    // parentX and parentY will be automatically cleaned up when they go out of scope
                }
            }
        }

        return matchingX;
    }

    float getMatchingValue(Graph::CSquareMatrix<float> const& m, std::vector<int> const& matching)
    {
        // TODO: generated code getMatchingValue
        if (matching.empty() || m.getSize() == 0)
        {
            return 0.0;
        }

        // Count how many times each vertex appears in the matching
        std::vector<int> count(m.getSize(), 0);

        // Count occurrences of each target vertex in the matching
        for (int sourceVertex : matching)
        {
            if (sourceVertex >= 0 && sourceVertex < static_cast<int>(count.size()))
            {
                count[sourceVertex]++;
            }
        }

        // Calculate the total value of the matching
        double totalValue = 0.0;
        for (size_t sourceVertex = 0; sourceVertex < matching.size(); ++sourceVertex)
        {
            int targetVertex = matching[sourceVertex];
            if (targetVertex >= 0 && targetVertex < static_cast<int>(m.getSize()))
            {
                totalValue += m(sourceVertex, targetVertex);
            }
        }

        return totalValue;
    }
}  // namespace Graph
