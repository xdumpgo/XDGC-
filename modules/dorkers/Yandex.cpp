//
// Created by XQAD on 4/27/2020.
//

#include "Yandex.h"
#include <gumbo-query/Document.h>
#include <gumbo-query/Node.h>
#include <Poco/String.h>

Request *Yandex::buildRequest(std::string dork, int index, int page) {
    Poco::URI uri(_bases[index]);

    uri.addQueryParameter("text", dork);
    uri.addQueryParameter("p", Poco::NumberFormatter::format(page));

    return new Request(uri, Utils::random_ua(), reinterpret_cast<ResponseHandler>(Yandex::GatherLinks));
}

void Yandex::GatherLinks(std::string resp) {
    CDocument doc;
    doc.parse(resp);

    CSelection sel = doc.find("a.link.link_theme_normal.organic__url.link_cropped_no.i-bem");
    for (int i=0; i < sel.nodeNum(); i++) {
        auto url = sel.nodeAt(i).attribute("href");

        Globals::AddUrl(url);
    }
}
