#include "pch.h"

namespace Library
{
	void ServiceContainer::AddService(uint64_t typeID, void* service)
	{
		mServices.insert(std::pair<uint64_t, void*>(typeID, service));
	}

	void ServiceContainer::RemoveService(uint64_t typeID)
	{
		mServices.erase(typeID);
	}

	void* ServiceContainer::GetService(uint64_t typeID) const
	{
		std::map<uint64_t, void*>::const_iterator it = mServices.find(typeID);

		return (it != mServices.end() ? it->second : nullptr);
	}
}
