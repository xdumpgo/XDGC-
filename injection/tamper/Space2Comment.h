//
// Created by zerte on 5/28/2020.
//

#ifndef XDG_SPACE2COMMENT_H
#define XDG_SPACE2COMMENT_H

#include "Tamper.h"

namespace Injection::Tamper {
    class Space2Comment : public TamperScript {
        void Tamper(std::string& query) override;
    };
};


#endif //XDG_SPACE2COMMENT_H
