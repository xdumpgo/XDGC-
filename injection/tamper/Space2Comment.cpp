//
// Created by zerte on 5/28/2020.
//

#include "Space2Comment.h"
#include <Poco/String.h>

namespace Injection::Tamper {
    void Space2Comment::Tamper(std::string &query) {
        Poco::replace(query, " ", "/**/");
    }
}
