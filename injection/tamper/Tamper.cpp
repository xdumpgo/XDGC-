//
// Created by zerte on 5/28/2020.
//

#include "Tamper.h"
#include "Space2Comment.h"
#include "Equal2Like.h"

namespace Injection::Tamper {
    std::vector<TamperScript*> _tampers;

    void Init() {
        _tampers.emplace_back(new Space2Comment());
        _tampers.emplace_back(new Equal2Like());
    }

    std::string TamperQuery(std::string query) {
        for (auto & tamper : _tampers) {
            tamper->Tamper(query);
        }
        return query;
    }
};