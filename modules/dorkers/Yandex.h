//
// Created by XQAD on 4/27/2020.
//

#ifndef XDG_YANDEX_H
#define XDG_YANDEX_H

#include "Dorker.h"

class Yandex : public Dorker {
public:
    Request* buildRequest(std::string dork, int index = 0, int page = 0) override;
    static void GatherLinks(std::string resp);

    Yandex() : Dorker("Yandex", std::vector<std::string>{
            "https://yandex.com/search/",
    }){}
};

#endif //XDG_YANDEX_H
