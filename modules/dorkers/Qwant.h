//
// Created by xiQQ on 4/25/2020.
//

#ifndef XDG_QWANT_H
#define XDG_QWANT_H

#include "Dorker.h"

class Qwant : public Dorker {
public:
    Request* buildRequest(std::string dork, int index = 0, int page = 0) override;
    static void GatherLinks(std::string resp);

    Qwant() : Dorker("Qwant", std::vector<std::string>{
            "https://api.qwant.com/api/search/web",
    }){}
};

#endif //XDG_QWANT_H
