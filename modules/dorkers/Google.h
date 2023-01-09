//
// Created by zerte on 4/24/2020.
//

#ifndef XDG_GOOGLE_H
#define XDG_GOOGLE_H

#include "Dorker.h"

class Google : public Dorker {
public:
    Request* buildRequest(std::string dork, int index = 0, int page = 0) override;
    static void GatherLinks(std::string resp);

    Google() : Dorker("Google", std::vector<std::string> {
        "https://google.com/search",
        "https://google.co.uk/search",
        "https://google.ru/search",
        "https://google.fr/search",
    }, httplib::Headers {
            { "User-Agent", Utils::random_ua() },
            { "Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8" },
            {"TE","Trailers"},
    }){}
};


#endif //XDG_GOOGLE_H
