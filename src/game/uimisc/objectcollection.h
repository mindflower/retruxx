#pragma once
#include <map>
#include <set>
#include <vector>

namespace ai
{
    class Obj;
}

namespace m3d
{
    struct Class;
}

class ObjectCollection
{
public:
    ~ObjectCollection();
    int RemoveObject(ai::Obj*);
    std::set<int> const* GetObjectsByClass(m3d::Class const*) const;
    ObjectCollection();
    bool IsClassCollected(m3d::Class const*) const;
    void ClearObjects();
    int AddObject(ai::Obj*);
protected:
private:
    void ClearClasses();
    void InitClasses();
    std::map<m3d::Class const*, std::set<int>> m_objects;
    std::vector<m3d::Class const*> m_collectedClasses;
};
