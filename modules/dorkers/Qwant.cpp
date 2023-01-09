//
// Created by xiQQ on 4/25/2020.
//

#include "Qwant.h"
#include <gumbo-query/Document.h>
#include <gumbo-query/Node.h>
#include <Poco/String.h>

Request *Qwant::buildRequest(std::string dork, int index, int page) {
    Poco::URI uri(_bases[index]);

    uri.addQueryParameter("count", "10");
    uri.addQueryParameter("offset", Poco::NumberFormatter::format(page * 10));
    uri.addQueryParameter("q", dork);
    uri.addQueryParameter("t", "web");
    uri.addQueryParameter("device", "desktop");
    uri.addQueryParameter("safesearch", "0");
    uri.addQueryParameter("locale", "en_US");
    uri.addQueryParameter("uiv", "4");

    return new Request(uri, Utils::random_ua(), reinterpret_cast<ResponseHandler>(Qwant::GatherLinks));
}

void Qwant::GatherLinks(std::string resp) {
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
