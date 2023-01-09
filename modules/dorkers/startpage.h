//
// Created by xiQQ on 4/27/2020.
//

#ifndef XDG_STARTPAGE_H
#define XDG_STARTPAGE_H

#include "Dorker.h"

class Startpage : public Dorker {
public:
    Request* buildRequest(std::string dork, int index = 0, int page = 0) override;
    static void GatherLinks(std::string resp);

    Startpage() : Dorker("StartPage", std::vector<std::string>{
            "https://www.startpage.com/sp/search",
    }){}
};

#endif //XDG_STARTPAGE_H
