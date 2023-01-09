//
// Created by zerte on 5/3/2020.
//

#ifndef XDG_INJECTION_H
#define XDG_INJECTION_H

#include <string>
#include <Poco/URI.h>
#include <cstdlib>

namespace Injection {
    enum InjectionType {
        IERR = 0,
        IUNI,
        IBLI
    };

    enum DBType {
        MYSQL = 0,
        ORACLE,
        MSSQL,
        POSTGRES,
    };

    static const char* _flag1 = "XDGV";
    static const char* _flag2 = "VGDX";

    Poco::URI BuildInjectionQuery(Poco::URI uri, int param, InjectionType iT, DBType dT, int vec, int mod, std::string payload, int andor = 0, int uc = 0, int ui = 0);

    int CountPossibleInjections();
};


#endif //XDG_INJECTION_H
