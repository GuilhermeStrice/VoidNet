#include "VoidNet_HL/Plugin/PluginManager.hpp"

#include "VoidNet_HL/Plugin/Plugin.hpp"

namespace std::net
{
	PluginManager::PluginManager()
	{
	}

	PluginManager::~PluginManager()
	{
		for (size_t i = 0; i < m_plugins.size(); i++)
		{
			delete m_plugins[i];
		}
	}
}