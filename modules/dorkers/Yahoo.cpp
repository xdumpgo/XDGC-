//
// Created by zerte on 4/25/2020.
//

#include "Yahoo.h"
#include <gumbo-query/Document.h>
#include <gumbo-query/Node.h>
#include <Poco/String.h>

Request *Yahoo::buildRequest(std::string dork, int index, int page) {
    Poco::URI uri(_bases[index]);

    uri.addQueryParameter("p", dork);
    uri.addQueryParameter("fr", "sfp");
    uri.addQueryParameter("fr2", "sb-top-search");
    uri.addQueryParameter("b", Poco::NumberFormatter::format((page * 100) + 1));
    uri.addQueryParameter("pz", "100");
    uri.addQueryParameter("bct", "0");
    uri.addQueryParameter("xargs", "0");

    return new Request(uri, Utils::random_ua(), reinterpret_cast<ResponseHandler>(Yahoo::GatherLinks));
}

void Yahoo::GatherLinks(std::string resp) {
    CDocument doc;
    doc.parse(resp);

    CSelection sel = doc.find("h3.title.ov-h a");
    for (int i=0; i < sel.nodeNum(); i++) {
        auto url = sel.nodeAt(i).attribute("href");

        Globals::AddUrl(url);
    }
}
