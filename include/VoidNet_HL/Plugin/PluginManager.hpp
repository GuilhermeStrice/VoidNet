#pragma once

#include <vector>

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
			for (size_t i = 0; i < m_plugins.size(); i++)
				m_plugins.at(i)->HandleMessage(msg);
		}

		void AddPlugin(Plugin* p)
		{
			m_plugins.emplace_back(p);
		}

	private:
		vector<Plugin*> m_plugins;
	};
}