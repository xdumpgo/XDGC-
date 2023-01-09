//
// Created by zerte on 4/25/2020.
//

#include "MyWebSearch.h"
#include <gumbo-query/Document.h>
#include <gumbo-query/Node.h>
#include <Poco/String.h>

Request *MyWebSearch::buildRequest(std::string dork, int index, int page) {
    Poco::URI uri(_bases[index]);
    uri.addQueryParameter("p2","^MYWEBSEARCHDEFAULT^^^");
    uri.addQueryParameter("ln", "en");
    uri.addQueryParameter("tpr", "hpsb");
    uri.addQueryParameter("trs", "wtt");
    uri.addQueryParameter("searchfor",dork);
    uri.addQueryParameter("st", "hp");

    return new Request(uri, Utils::random_ua(), reinterpret_cast<ResponseHandler>(MyWebSearch::GatherLinks));
}

void MyWebSearch::GatherLinks(std::string resp) {
    CDocument doc;
    doc.parse(resp);

    CSelection sel = doc.find("a.algo-title");
    for (int i=0; i < sel.nodeNum(); i++) {
        auto url = sel.nodeAt(i).attribute("href");

        Globals::AddUrl(url);
    }
}
