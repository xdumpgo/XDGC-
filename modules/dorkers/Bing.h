//
// Created by zerte on 4/25/2020.
//

#ifndef XDG_BING_H
#define XDG_BING_H

#include "Dorker.h"

class Bing : public Dorker {
public:
    Request* buildRequest(std::string dork, int index = 0, int page = 0) override;
    static void GatherLinks(std::string resp);

    Bing() : Dorker("Bing", std::vector<std::string> {
        "https://www.bing.com/search",
    }) {};
};

#endif //XDG_BING_H
