//
// Created by zerte on 4/25/2020.
//

#ifndef XDG_MYWEBSEARCH_H
#define XDG_MYWEBSEARCH_H

#include "Dorker.h"

class MyWebSearch : public Dorker{
public:
    Request* buildRequest(std::string dork, int index = 0, int page = 0) override;
    static void GatherLinks(std::string resp);

    MyWebSearch() : Dorker("MyWebSearch", std::vector<std::string> {
        "https://search.mywebsearch.com/mywebsearch/GGmain.jhtml",
    }){}
};


#endif //XDG_MYWEBSEARCH_H
