//
// Created by zerte on 4/24/2020.
//

#include "Google.h"
#include <gumbo-query/Document.h>
#include <gumbo-query/Node.h>

Request* Google::buildRequest(std::string dork, int index, int page) {
    Poco::URI url(this->_bases[index]);

    url.addQueryParameter("q", dork);
    url.addQueryParameter("oq", dork);
    url.addQueryParameter("num", "100");
    url.addQueryParameter("hl", "en");
    url.addQueryParameter("filter", "0");
    url.addQueryParameter("start", Poco::NumberFormatter::format(page * 100));

    return new Request(url, Utils::random_ua(), reinterpret_cast<ResponseHandler>(Google::GatherLinks));
}

void Google::GatherLinks(std::string resp) {
    CDocument doc;
    doc.parse(resp);

    CSelection sel = doc.find("div.r a");
    for (int i=0; i < sel.nodeNum(); i++) {
        auto url = sel.nodeAt(i).attribute("href");
        if (url.find("google") == std::string::npos)
            Globals::AddUrl(url);
    }
}
