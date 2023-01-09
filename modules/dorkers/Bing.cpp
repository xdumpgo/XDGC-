//
// Created by zerte on 4/25/2020.
//

#include "Bing.h"
#include <gumbo-query/Document.h>
#include <gumbo-query/Node.h>
#include <Poco/String.h>

Request *Bing::buildRequest(std::string dork, int index, int page) {
    Poco::URI url(this->_bases[index]);
    url.addQueryParameter("q", dork);
    url.addQueryParameter("first", Poco::NumberFormatter::format(page * 15));

    return new Request(url, Utils::random_ua(), reinterpret_cast<ResponseHandler>(Bing::GatherLinks));
}

void Bing::GatherLinks(std::string resp) {
    CDocument doc;
    doc.parse(resp);

    CSelection sel = doc.find("div.b_algo cite");
    for (int i=0; i < sel.nodeNum(); i++) {
        auto url = sel.nodeAt(i).text();
        Poco::replaceInPlace(url, "<strong>", "");
        Poco::replaceInPlace(url, "</strong>", "");
        Poco::replaceInPlace(url, "\r", "");
        Poco::replaceInPlace(url, "\n", "");

        Globals::AddUrl(url);
    }
}
