#pragma once

#include <vector>
#include <mutex>

#include "VoidNet_HL/Plugin/Plugin.hpp"

namespace std::net
{
	class PluginManager
	{
	public:
		PluginManager();
		~PluginManager();

		void HandleMessage(const NetworkMessage& msg)
		{
			m_pluginsMutex.lock();

			for (size_t i = 0; i < m_plugins.size(); i++)
				m_plugins.at(i)->HandleMessage(msg);

			m_pluginsMutex.unlock();
		}

		void AddPlugin(Plugin* p)
		{
			m_pluginsMutex.lock();
			m_plugins.emplace_back(p);
			m_pluginsMutex.unlock();
		}

	private:
		vector<Plugin*> m_plugins;

		mutex m_pluginsMutex;
	};
}