//
// Created by zerte on 5/7/2020.
//

#include "Cleaner.h"
#include <Poco/RegularExpression.h>

void Cleaner::run() {
    std::vector<std::string> _urls = Globals::GetUrls();
    Globals::ClearUrls();
    std::remove("urls.txt");
    bool b;
    for (std::string & url : _urls) {
        Poco::URI _u(url);

        if (_u.getHost().empty()) {
            continue;
        }

        b = false;
        if (url.find('?') == std::string::npos || url.find('=') == std::string::npos) {
            continue;
        }

        for (auto & u : Globals::GetUrls()) {
            Poco::URI _u2(u);
            if (_u.getHost() == _u2.getHost()) {
                b = true;
                break;
            }
        }

        if (url.find(".gov") == std::string::npos)
            continue;

        if (b)
            continue;

        Globals::AddUrl(url);
    }
}
