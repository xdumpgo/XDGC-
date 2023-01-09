//
// Created by zerte on 4/25/2020.
//

#include "Aol.h"
#include <gumbo-query/Document.h>
#include <gumbo-query/Node.h>
#include <Poco/String.h>

Request *Aol::buildRequest(std::string dork, int index, int page) {
    Poco::URI uri(_bases[index]);
    uri.addQueryParameter("s_chn", "prt_bon");
    uri.addQueryParameter("q", dork);
    uri.addQueryParameter("nojs", "1");
    uri.addQueryParameter("b", Poco::NumberFormatter::format(page * 100));
    uri.addQueryParameter("pz", "100");
    uri.addQueryParameter("bct", "0");
    uri.addQueryParameter("xargs", "0");
    uri.addQueryParameter("v_t", "na");

    return new Request(uri, Utils::random_ua(), reinterpret_cast<ResponseHandler>(Aol::GatherLinks));
}

void Aol::GatherLinks(std::string resp) {
    CDocument doc;
    doc.parse(resp);

    CSelection sel = doc.find("span.fz-ms.fw-m.fc-12th.wr-bw.lh-17");
    for (int i=0; i < sel.nodeNum(); i++) {
        auto a = sel.nodeAt(i).text();
        a = Poco::replaceInPlace(a, "<b>", "");
        a = Poco::replaceInPlace(a, "</b>", "");
        a = Poco::replaceInPlace(a, "\r", "");
        a = Poco::replaceInPlace(a, "\n", "");
        if (a.find("...")) { continue; }
        Globals::AddUrl("http://" + a);
    }
}
