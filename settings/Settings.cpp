
#include "Settings.h"
#include <antireversal/antidebug.h>
#include <modules/dorkers/Dorker.h>
#include <utils/Utils.h>

libconfig::Config Settings::cfg;

void Settings::Init() {
	Utils::logInfo("Begin Settings Init");

	try {
        cfg.readFile("xdg.cfg");
    } catch (const libconfig::FileIOException &fioex) {
        Utils::logError("No config found, populating.");
    }

	if (Anti::Debugging::CheckProcessDebugFlags()) {
	    exit(0);
	}

	auto& root = cfg.getRoot();

    if (!root.exists("core")) {
        auto& core = root.add("core", libconfig::Setting::TypeGroup);
        if (!core.exists("threads")) {
            core.add("threads", libconfig::Setting::TypeInt) = 50;
        }
        if (!core.exists("timeouts")) {
            core.add("timeouts", libconfig::Setting::TypeInt) = 10;
        }
        if(!core.exists("batch")) {
            core.add("batch", libconfig::Setting::TypeBoolean) = false;
        }
    }

    if (!root.exists("scraper")) {
        auto& scraper = root.add("scraper", libconfig::Setting::TypeGroup);
        if (!scraper.exists("pages")) {
            scraper.add("pages", libconfig::Setting::TypeInt) = 2;
        }

        if (!scraper.exists("dorkers")) {
            auto& dorkers = scraper.add("dorkers", libconfig::Setting::TypeGroup);
            for (auto & Dorker : Dorkers) {
                if (!dorkers.exists(Dorker->getName())) {
                    dorkers.add(Dorker->getName(), libconfig::Setting::TypeBoolean) = false;
                }
            }
        }
    }

    if (!root.exists("exploiter")) {
        auto& exploiter = root.add("exploiter", libconfig::Setting::TypeGroup);
        if (!exploiter.exists("intensity")) {
            exploiter.add("intensity", libconfig::Setting::TypeInt) = 0;
        }

        if (!exploiter.exists("err")) {
            exploiter.add("err", libconfig::Setting::TypeBoolean) = true;
        }
        if (!exploiter.exists("uni")) {
            exploiter.add("uni", libconfig::Setting::TypeBoolean) = true;
        }
        if (!exploiter.exists("bli")) {
            exploiter.add("bli", libconfig::Setting::TypeBoolean) = false;
        }
    }

    if (!root.exists("dumper")) {
        auto& dumper = root.add("dumper", libconfig::Setting::TypeGroup);
        if (!dumper.exists("targeted")) {
            dumper.add("targeted", libconfig::Setting::TypeBoolean) = false;
        }
    }

    cfg.writeFile("xdg.cfg");
	Utils::logInfo("End Settings Init");
}
