//
// Created by zerte on 5/28/2020.
//

#ifndef XDG_EQUAL2LIKE_H
#define XDG_EQUAL2LIKE_H

#include "Tamper.h"

namespace Injection::Tamper {
    class Equal2Like : public TamperScript {
        void Tamper(std::string& query) override;
    };
}


#endif //XDG_EQUAL2LIKE_H
