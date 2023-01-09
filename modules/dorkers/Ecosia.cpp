//
// Created by xiQQ on 4/25/2020.
//

#include "Ecosia.h"
#include <gumbo-query/Document.h>
#include <gumbo-query/Node.h>
#include <Poco/String.h>

Request *Ecosia::buildRequest(std::string dork, int index, int page) {
    Poco::URI uri(_bases[index]);

    uri.addQueryParameter("q", dork);
    uri.addQueryParameter("p", Poco::NumberFormatter::format(page));
    uri.addQueryParameter("c", "en");

    return new Request(uri, Utils::random_ua(), reinterpret_cast<ResponseHandler>(Ecosia::GatherLinks));
}

void Ecosia::GatherLinks(std::string resp) {
    CDocument doc;
    doc.parse(resp);

    CSelection sel = doc.find("a.result-snippet-link");
    for (int i=0; i < sel.nodeNum(); i++) {
        auto url = sel.nodeAt(i).attribute("href");

        Globals::AddUrl(url);
    }
}
