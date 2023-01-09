//
// Created by zerte on 4/24/2020.
//

#ifndef XDG_DORKER_H
#define XDG_DORKER_H

#include <proxymanager/ProxyManager.h>
#include <settings/Settings.h>
#include <string>
#include <utility>
#include <vector>
#include <Poco/NumberFormatter.h>
#include <Poco/URI.h>

class Dorker {
protected:
    std::string _name;
    std::vector<std::string> _bases;
    httplib::Headers _headers;

    Dorker(std::string name, std::vector<std::string> bases, httplib::Headers headers = {}) {
        _name = std::move(name);
        _bases = std::move(bases);
        _headers = std::move(headers);
        //_enabled = Settings::GetBool(format("scraper.dorkers.%s", _name.c_str()));
    }

public:
    std::string getName();
    std::vector<std::string> getBases();
    bool getEnabled();
    void setEnabled(bool n);

    virtual Request* buildRequest(std::string dork, int index = 0, int page = 0) = 0;
};

extern std::vector<Dorker*> Dorkers;

#endif //XDG_DORKER_H
