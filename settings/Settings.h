#ifndef XDG_SETTINGS_
#define XDG_SETTINGS_

#include <string>
#include <libconfig.h++>

class Settings
{
private:
    static libconfig::Config cfg;
public:
	static void Init();

    static int GetInt(const std::string& path) {
        int value;
        if (cfg.lookupValue(path, value)) {
            return value;
        }
        return -1;
    }

    static bool GetBool(const std::string& path) {
        bool value;
        if (cfg.lookupValue(path, value)) {
            return value;
        }
        return false;
    }

    static std::string GetString(const std::string& path) {
        std::string value;
        if (cfg.lookupValue(path, value)) {
            return value;
        }
        return "";
    }

    static void Set(const std::string& path, int value) {
        cfg.lookup(path) = value;
        cfg.writeFile("xdg.cfg");
	}

    static void Set(const std::string& path, bool value) {
        cfg.lookup(path) = value;
        cfg.writeFile("xdg.cfg");
    }

    static void Set(const std::string& path, const std::string& value) {
        cfg.lookup(path) = value;
        cfg.writeFile("xdg.cfg");
    }
};


#endif