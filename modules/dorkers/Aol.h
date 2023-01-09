//
// Created by zerte on 4/25/2020.
//

#ifndef XDG_AOL_H
#define XDG_AOL_H

#include "Dorker.h"

class Aol : public Dorker {
public:
    Request* buildRequest(std::string dork, int index = 0, int page = 0) override;
    static void GatherLinks(std::string resp);

    Aol() : Dorker("AOL", std::vector<std::string>{
        "https://search.aol.com/aol/search"
    }){}
};


#endif //XDG_AOL_H
