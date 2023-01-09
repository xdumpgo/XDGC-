//
// Created by zerte on 4/20/2020.
//

#ifndef XDG_SCRAPER_H
#define XDG_SCRAPER_H

#include "Module.h"
#include <functional>
#include <regex>

namespace Modules {
    class Scraper : public Module<Scraper> {
    public:
        Scraper();

        void run() override;
    };
}

#endif //XDG_SCRAPER_H
