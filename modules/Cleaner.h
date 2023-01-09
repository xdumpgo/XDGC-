//
// Created by zerte on 5/7/2020.
//

#ifndef XDG_CLEANER_H
#define XDG_CLEANER_H

#include "Module.h"

using namespace Modules;

namespace Modules {
    class Cleaner : public Module<Cleaner> {
    private:

    public:
        void run() override;
    };
}


#endif //XDG_CLEANER_H
