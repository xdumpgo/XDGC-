//
// Created by XQAD on 4/25/2020.
//

#ifndef XDG_ECOSIA_H
#define XDG_ECOSIA_H

#include "Dorker.h"

class Ecosia : public Dorker {
public:
    Request* buildRequest(std::string dork, int index = 0, int page = 0) override;
    static void GatherLinks(std::string resp);

    Ecosia() : Dorker("Ecosia", std::vector<std::string>{
        "https://www.ecosia.org/search",
    }){}
};

#endif //XDG_ECOSIA_H
