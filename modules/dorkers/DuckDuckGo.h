//
// Created by zerte on 4/25/2020.
//

#ifndef XDG_DUCKDUCKGO_H
#define XDG_DUCKDUCKGO_H

#include "Dorker.h"

class DuckDuckGo : public Dorker {
public:
    Request* buildRequest(std::string dork, int index = 0, int page = 0) override;
    static void GatherLinks(std::string resp);

    DuckDuckGo() : Dorker("DuckDuckGo", std::vector<std::string> {
        "https://duckduckgo.com/html",
    }){}
};


#endif //XDG_DUCKDUCKGO_H
