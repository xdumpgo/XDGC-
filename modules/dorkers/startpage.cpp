//
// Created by xiQQ on 4/27/2020.
//

#include "startpage.h"
#include <gumbo-query/Document.h>
#include <gumbo-query/Node.h>
#include <Poco/String.h>

Request *Startpage::buildRequest(std::string dork, int index, int page) {
    Poco::URI uri(_bases[index]);

    uri.addQueryParameter("abp", "-1");
    uri.addQueryParameter("language", "english");
    uri.addQueryParameter("pg", "0");
    uri.addQueryParameter("lui", "english");
    uri.addQueryParameter("query", dork);
    uri.addQueryParameter("cat", "web");
    uri.addQueryParameter("page", Poco::NumberFormatter::format(page + 1));

    return new Request(uri, Utils::random_ua(), reinterpret_cast<ResponseHandler>(Startpage::GatherLinks));
}

void Startpage::GatherLinks(std::string resp) {
    CDocument doc;
    doc.parse(resp);

    CSelection sel = doc.find("a.w-gl__result-url");
    for (int i=0; i < sel.nodeNum(); i++) {
        auto url = sel.nodeAt(i).attribute("href");

        Globals::AddUrl(url);
    }
}
