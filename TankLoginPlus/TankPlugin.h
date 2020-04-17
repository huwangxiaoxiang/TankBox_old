#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

struct TankPlugin{
	std::string plugin_name;
	std::string state;
};

class TankPluginManager
{
private:
	static LPTSTR config_path;
	static std::vector<TankPlugin> plugins;
	static void readPlugin();
	static void writePlugin();

public:
	static void initPlugin(LPTSTR path);
	static void setPlugin(std::string plugin_name, std::string state);
	static std::string getPluginState(std::string plugin_name);
	static void mSetConfigPath(LPTSTR path);
};

