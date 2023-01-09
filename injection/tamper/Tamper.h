//
// Created by zerte on 5/28/2020.
//

#ifndef XDG_TAMPER_H
#define XDG_TAMPER_H

#include <string>
#include <vector>

namespace Injection::Tamper {
    class TamperScript {
    public:
        virtual void Tamper(std::string& query) = 0;
    };

    extern std::vector<TamperScript*> _tampers;

    void Init();

    std::string TamperQuery(std::string query);
}


#endif //XDG_TAMPER_H
