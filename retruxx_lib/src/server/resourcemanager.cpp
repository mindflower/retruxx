#include "resourcemanager.h"
#include <stdexcept>

#include "core/ini.h"
#include "core/log.h"
#include "core/ref_ptr.h"
#include "objects/base/globalproperties.h"

namespace ai
{
	Resource::Resource(Resource const* parent)
	{
		if (parent)
		{
			m_geomSize.x = parent->GetGeomSize().x;
			m_geomSize.y = parent->GetGeomSize().y;
			m_parentId = parent->GetId();
		}
	}

	PointBase<int> Resource::GetGeomSize() const
	{
		return this->m_geomSize;
	}

	int Resource::GetParentId() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CStr Resource::GetName() const
	{
		return m_name;
	}

	int Resource::GetId() const
	{
		return m_id;
	}

	bool Resource::bIsKindOf(int resourceId) const
	{
        if (m_id == -1)
        {
            return false;
        }

        auto id = m_id;
        while (id != resourceId)
        {
            auto res = theResourceManager->GetResource(id);
            id = res->m_parentId;
            if (id == -1)
            {
                return false;
            }
        }
        return true;
	}

	void Resource::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
		m3d::SafeStrAttrib(m_name, xmlNode, "Name");
		CStr geomSize;
		m3d::SafeStrAttrib(geomSize, xmlNode, "geomsize");
		if (!geomSize.empty())
		{
			int x = 0;
			int y = 0;
			if (sscanf(geomSize.c_str(), "%d %d", &x, &y))
			{
				m_geomSize.x = x;
				m_geomSize.y = y;
			}
		}
	}

	CStr ResourceManager::GetResourceName(int resourceId) const
	{
        if (resourceId >=0 && resourceId < m_resourceVector.size() && m_resourceVector[resourceId])
        {
            return m_resourceVector[resourceId]->m_name;
        }
        return {};
	}

	void ResourceManager::Init()
	{
		_LoadFromXmlFile(theGlobProp.m_pathToResourceTypes.c_str());
		_LoadVehiclePartTypeToResourceXmlFile(theGlobProp.m_pathToVehiclePartTypes.c_str());
	}

	void ResourceManager::GetResourceDescendants(int resourceId, std::vector<int>& descendants) const
	{
        descendants.clear();
        for (auto& resource : m_resourceVector)
        {
            if (resource->bIsKindOf(resourceId))
            {
                descendants.push_back(resource->m_id);
            }
        }
	}

	bool ResourceManager::ResourceHasChildren(int resourceId) const
	{
        for (auto res : m_resourceVector)
        {
            if (res->m_parentId == resourceId)
            {
                return true;
            }
        }
        return false;
	}

	int ResourceManager::GetResourceId(CStr const& resourceName) const
	{
		auto it = m_resourceMap.find(resourceName);
		if (it != m_resourceMap.end())
		{
			return it->second->GetId();
		}
		return -1;
	}

	ResourceManager::ResourceManager()
	{
	}

	CStr ResourceManager::GetResourceNameByVehiclePartName(CStr const& vehiclePartName) const
	{
		auto it = m_vehiclePart2Resource.find(vehiclePartName);
		if (it == m_vehiclePart2Resource.end())
		{
			return {};
		}

		return it->second;
	}

	Resource* ResourceManager::GetResource(int resourceId) const
	{
        if (resourceId < 0 || resourceId >= m_resourceVector.size())
        {
            return nullptr;
        }

        return m_resourceVector[resourceId];
	}

	bool ResourceManager::bResourceIsKindOf(int resourceId, int ancestorId)
	{
        if (resourceId >= 0 && resourceId < m_resourceVector.size())
        {
            return m_resourceVector[resourceId] && m_resourceVector[resourceId]->bIsKindOf(ancestorId);
        }
        return false;
	}

	ResourceManager::~ResourceManager()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void ResourceManager::_ReadResourceFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode, Resource const* parent)
	{
		auto resource = new Resource(parent);
		resource->LoadFromXML(xmlFile, xmlNode);

		auto it = m_resourceMap.find(resource->GetName());
		if (it != m_resourceMap.end())
		{
			M3D_LOG_ERR("Error: duplicate resource name");
			return;
		}
		resource->m_id = m_resourceVector.size();
		m_resourceMap[resource->GetName()] = resource;
		m_resourceVector.push_back(resource);
		if (!xmlNode->IsEmpty())
		{
			ref_ptr typeNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
			for (xmlNode->GetFirstChild(typeNode, "Type"); !typeNode->IsEmpty(); typeNode->GetNextSibling(typeNode, "Type"))
			{
				_ReadResourceFromXml(xmlFile, typeNode, nullptr);
			}
		}
	}

	void ResourceManager::_LoadFromXmlFile(char const* fileName)
	{
		CStr err;
		ref_ptr xmlFile = m3d::ReadXmlFile(fileName, &err);
		if (xmlFile)
		{
			ref_ptr xmlNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
			xmlFile->GetFirstChild(xmlNode, "ResourceTypes");
			if (!xmlNode->IsEmpty())
			{
				ref_ptr typeNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
				for (xmlNode->GetFirstChild(typeNode, "Type"); !typeNode->IsEmpty(); typeNode->GetNextSibling(typeNode, "Type"))
				{
					_ReadResourceFromXml(xmlFile, typeNode, nullptr);
				}
			}
		}
		else
		{
			M3D_LOG_INFO("Error: No Resource file: " + CStr(fileName));
		}
	}

	void ResourceManager::_LoadVehiclePartTypeToResourceXmlFile(char const* fileName)
	{
		CStr err;
		ref_ptr xmlFile = m3d::ReadXmlFile(fileName, &err);
		if (xmlFile)
		{
			ref_ptr partTypes = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
			ref_ptr part = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
			xmlFile->GetFirstChild(partTypes, "VehiclePartTypes");
			if (partTypes->IsEmpty())
			{
				return;
			}
			for (partTypes->GetFirstChild(part, "VehiclePart"); !part->IsEmpty(); part->GetNextSibling(part, "VehiclePart"))
			{
				CStr vehiclePartName;
				CStr resourceName;
				m3d::SafeStrAttrib(vehiclePartName, part, "PartName");
				m3d::SafeStrAttrib(resourceName, part, "ResourceName");
				if (GetResourceId(resourceName) == -1)
				{
					M3D_LOG_INFO("ResourceManager: warning - invalid resource name " + resourceName + " is match to vehicle part type " + vehiclePartName);
				}
				m_vehiclePart2Resource[vehiclePartName] = resourceName;
			}
		}
		else
		{
			M3D_LOG_INFO("Error: No Resource file: " + CStr(fileName));
		}
	}
}
