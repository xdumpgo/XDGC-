//
// Created by zerte on 4/25/2020.
// /*still not finished*/

#include "DuckDuckGo.h"
#include <gumbo-query/Document.h>
#include <gumbo-query/Node.h>
#include <Poco/String.h>
// q=a&s=80&nextParams=&v=l&o=json&dc=59&api=%2Fd.js&kl=wt-wt
Request *DuckDuckGo::buildRequest(std::string dork, int index, int page) {
    Poco::URI uri(_bases[index]);
    uri.addQueryParameter("s", Poco::NumberFormatter::format((page * 50) + 30));
    uri.addQueryParameter("dc", Poco::NumberFormatter::format((page * 30) + 1));
    uri.addQueryParameter("o", "json");
    uri.addQueryParameter("api", "/d.js");
    uri.addQueryParameter("v", "l");
    uri.addQueryParameter("kl", "wt-wt");

    return new Request(uri, Utils::random_ua(), reinterpret_cast<ResponseHandler>(DuckDuckGo::GatherLinks), "POST", Utils::format("q=%s&b=&kl=&df=", dork.c_str()), "application/x-www-form-urlencoded");
}

void DuckDuckGo::GatherLinks(std::string resp) {
    CDocument doc;
    doc.parse(resp);

    CSelection sel = doc.find("a.result__url");
    for (int i=0; i < sel.nodeNum(); i++) {
        auto url = sel.nodeAt(i).attribute("href");

        Globals::AddUrl(url);
    }
}
