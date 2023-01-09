//
// Created by zerte on 4/25/2020.
//

#ifndef XDG_YAHOO_H
#define XDG_YAHOO_H

#include "Dorker.h"

class Yahoo : public Dorker {
public:
    Request* buildRequest(std::string dork, int index = 0, int page = 0) override;
    static void GatherLinks(std::string resp);

    Yahoo() : Dorker("Yahoo", std::vector<std::string> {
        "https://search.yahoo.com/search",
    }){}
};


#endif //XDG_YAHOO_H
