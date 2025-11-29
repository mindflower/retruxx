#include "bullet.h"

#include "core/profilerstack.h"
#include "scene/scenegraph.h"
#include "scene/servers/dataserver.h"

#include <stdexcept>
#include <server/objects/base/prototypemanager.h>
#include <server/server.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Bullet)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Bullet);

    bool BulletPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        const auto res = ShellPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (res)
        {
            _SetGeomType(GEOM_TYPE_RAY);
        }
        return res;
    }

    BulletPrototypeInfo::BulletPrototypeInfo() = default;

    Obj* BulletPrototypeInfo::CreateTargetObject() const
    {
        return new Bullet(*this);
    }

    void Bullet::LinkGeomsToCollisionCells()
    {
    }

    void Bullet::SetParentBarrel(unsigned idx)
    {
        m_parentBarrel = idx;
    }

    void Bullet::SetTracer(m3d::SgNode* tracer)
    {
        m_tracer = tracer;
    }

    m3d::Class* Bullet::GetClass() const
    {
        return RT_CLASS_LOCAL(Bullet);
    }

    Bullet::Bullet(BulletPrototypeInfo const& prototype) : Shell(prototype)
    {
        m_tracer = nullptr;
        m_parentBarrel = 0;

        dGeomSetData(m_physicBody->m_pGeoms[0]->GetGeom()->GetGeomId(), this);

        DisablePhysics();
        DisableGeometry(1);
        m_framesToLive = 1;
    }

    Geom::CellAabb Bullet::GetCollisionCellAabb() const
    {
        Geom::CellAabb result;
        result.x0 = 0;
        result.z0 = 0;
        result.x1 = -1;
        result.z1 = -1;
        return result;
    }

    void Bullet::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Ray* Bullet::_Ray()
    {
        return dynamic_cast<Ray*>(m_physicBody->m_pGeoms[0]->GetGeom());
    }

    Ray const* Bullet::_Ray() const
    {
        return dynamic_cast<Ray*>(m_physicBody->m_pGeoms[0]->GetGeom());
    }

    void Bullet::RelinkGeomsToCollisionCells()
    {
    }

    void Bullet::SetRange(float range)
    {
        _Ray()->SetLength(range);
    }

    void Bullet::UnlinkGeomsFromCollisionCells()
    {
    }

    void Bullet::SpecifyTracer(CVector const& endPos)
    {
        static retruxx::vector<CVector> trace(2);

        trace[0] = GetPosition();
        trace[1] = endPos;

        std::vector<m3d::Object*> stack;
        stack.push_back(m_tracer);

        while (!stack.empty())
        {
            m3d::Object* current = stack.back();
            stack.pop_back();

            // Process all siblings of the current node
            m3d::SgNode* sibling = dynamic_cast<m3d::SgNode*>(current->GetFirstChild());
            while (sibling)
            {
                sibling->SetProperty(m3d::PROP_PS_MOVE_PARTICLES, &trace);

                // If this sibling has children, add to stack for processing
                if (sibling->GetFirstChild())
                {
                    stack.push_back(sibling->GetFirstChild());
                }

                // Move to next sibling
                sibling = dynamic_cast<m3d::SgNode*>(sibling->GetNextSibling());
            }
        }
    }

    void Bullet::SetDirection(CVector const& newDirection)
    {
        _Ray()->SetDirection(newDirection);
    }

    CVector Bullet::GetDirection() const
    {
        return _Ray()->GetDirection();
    }

    void Bullet::Update(float elapsedTime, unsigned workTime)
    {
        SimplePhysicObj::Update(elapsedTime, workTime);
        pServer->GetBulletProfiler()->StartCountdown();
        if (m_tracer)
        {
            static retruxx::vector<CVector> trace(2);

            auto* ray = _Ray();
            const auto len = ray->GetLength();
            const auto dir = ray->GetDirection();

            trace[0] = GetPosition();
            trace[1].x = (len * dir.x) + trace[0].x;
            trace[1].y = (len * dir.y) + trace[0].y;
            trace[1].z = (len * dir.z) + trace[0].z;

            std::vector<m3d::Object*> stack;
            stack.push_back(m_tracer);

            while (!stack.empty())
            {
                m3d::Object* current = stack.back();
                stack.pop_back();

                // Process all siblings of the current node
                m3d::SgNode* sibling = dynamic_cast<m3d::SgNode*>(current->GetFirstChild());
                while (sibling)
                {
                    sibling->SetProperty(m3d::PROP_PS_ADD_PARTICLES, &trace);

                    // If this sibling has children, add to stack for processing
                    if (sibling->GetFirstChild())
                    {
                        stack.push_back(sibling->GetFirstChild());
                    }

                    // Move to next sibling
                    sibling = dynamic_cast<m3d::SgNode*>(sibling->GetNextSibling());
                }
            }
        }

        DynamicScene::CollideBullet(*this);
        Remove();
        pServer->GetBulletProfiler()->EndCountdown();
    }

    void Bullet::TransferPhysicParamsToSceneGraphNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Bullet::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Shell);
    }

    BulletPrototypeInfo const* Bullet::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), const BulletPrototypeInfo);
    }

    Bullet::~Bullet()
    {
        if (m_tracer)
        {
            std::vector<m3d::Object*> stack;
            stack.push_back(m_tracer);

            while (!stack.empty())
            {
                m3d::Object* current = stack.back();
                stack.pop_back();

                // Process all siblings of the current node
                m3d::SgNode* sibling = dynamic_cast<m3d::SgNode*>(current->GetFirstChild());
                while (sibling)
                {
                    sibling->CanBeFree();

                    // If this sibling has children, add to stack for processing
                    if (sibling->GetFirstChild())
                    {
                        stack.push_back(sibling->GetFirstChild());
                    }

                    // Move to next sibling
                    sibling = dynamic_cast<m3d::SgNode*>(sibling->GetNextSibling());
                }
            }

            auto* graph = m_tracer->GetGraph();
            graph->InsertInRemoveIfFree(m_tracer);
            m_tracer = nullptr;
        }
    }

    m3d::Object* Bullet::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Bullet::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
