//
// Created by zerte on 5/28/2020.
//

#include "Equal2Like.h"
#include <Poco/String.h>

namespace Injection::Tamper {
    void Equal2Like::Tamper(std::string &query) {
        Poco::replaceInPlace(query, "=", "LIKE");
    }
}