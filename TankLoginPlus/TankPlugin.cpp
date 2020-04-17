#include "pch.h"
#include "TankPlugin.h"

LPTSTR TankPluginManager::config_path = NULL;
std::vector<TankPlugin> TankPluginManager::plugins = std::vector<TankPlugin>();

void TankPluginManager::readPlugin()
{
	std::fstream _file;
	_file.open(TankPluginManager::config_path, std::ios::in);
	std::string a, b;
	char buf[1024] = { 0 };
	TankPluginManager::plugins.clear();
	while (_file.getline(buf, sizeof(buf))) {
		std::stringstream line(buf);
		try {
			struct TankPlugin temp;
			line >> a;
			line >> b;
			temp.plugin_name = a;
			temp.state = b;
			TankPluginManager::plugins.push_back(temp);
		}
		catch (std::string& e) {

		}
		line.clear();
	}
	_file.close();
}

void TankPluginManager::writePlugin()
{
	std::string s = "";
	for (std::vector<TankPlugin>::iterator i = TankPluginManager::plugins.begin(); i != TankPluginManager::plugins.end(); i++) {
		s = s + i->plugin_name + " " + i->state + "\n";
	}
	std::fstream _file;
	_file.open(TankPluginManager::config_path, std::ios::out);
	_file << s;
	_file.close();
}

void TankPluginManager::initPlugin(LPTSTR path)
{
	TankPluginManager::config_path = path;
	TankPluginManager::readPlugin();
}

void TankPluginManager::setPlugin(std::string plugin_name, std::string state)
{
	bool has_find = false;
	for (std::vector<TankPlugin>::iterator i = TankPluginManager::plugins.begin(); i != TankPluginManager::plugins.end(); i++) {
		if (i->plugin_name == plugin_name) {
			i->state = state;
			has_find = true;
			break;
		}
	}
	if (!has_find) {
		TankPlugin temp;
		temp.plugin_name = plugin_name;
		temp.state = state;
		TankPluginManager::plugins.push_back(temp);
	}

	TankPluginManager::writePlugin();
}


std::string TankPluginManager::getPluginState(std::string plugin_name)
{
	for (std::vector<TankPlugin>::iterator i = TankPluginManager::plugins.begin(); i != TankPluginManager::plugins.end(); i++) {
		if (i->plugin_name == plugin_name) {
			return i->state;
			break;
		}
	}
	return std::string();
}

void TankPluginManager::mSetConfigPath(LPTSTR path)
{
	TankPluginManager::config_path = path;
}
