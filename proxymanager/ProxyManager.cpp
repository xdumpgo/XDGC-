#include "ProxyManager.h"

#include <fstream>
#include <utils/Utils.h>
#include <settings/Settings.h>

ProxyManager* PManager;

ProxyManager::ProxyManager() {
    mt = std::mt19937(rd());
}

void ProxyManager::SetProxyType(ProxyType _type) {
	this->_proxyType = _type;
}

ProxyType ProxyManager::GetProxyType() {
	return this->_proxyType;
}

int ProxyManager::LoadProxies(const std::string& path) {
	std::ifstream proxyfile(path);
	std::string line;
	this->proxies.clear();
	while (std::getline(proxyfile, line)) {
        std::vector<std::string> parts;
        Utils::string_split(line, parts, ':');
	    if (parts.size() == 4 && strlen(this->_auth.first.c_str()) == 0) {
            this->_auth.first = parts[2];
            this->_auth.second = parts[3];
	    }
		this->proxies.emplace_back(Proxy(parts[0], parts[1]));
	}
	return this->proxies.size();
}

void ProxyManager::Reset() {
    for (auto & p : this->proxies) {
        //p.setLocked(false);
        p.setAlive(true);
    }
}

Proxy* ProxyManager::GetRandomProxy() {
	if (this->proxies.empty()) {
		return new Proxy();
	}
	else if (this->proxies.size() == 1) {
		return &this->proxies[0];
	}

	if (this->GetLivingProxies() == 0 ) {
	    this->Reset();
	}

    std::uniform_real_distribution<double> dist(0, this->proxies.size()-1);
	while (auto p = &this->proxies[dist(mt)]) {
	    if (!p->isLocked() && p->GetAlive()) {
	        p->setLocked(true);
	        return p;
	    }
	}
	return nullptr;
}

Proxy ProxyManager::GetProxy(int n) {
    return this->proxies[n];
}

int ProxyManager::GetProxyCount() {
	return this->proxies.size();
}

std::pair<std::string, std::string> ProxyManager::GetAuth() {
    return this->_auth;
}

std::string ProxyManager::Get(const Poco::URI& uri, const httplib::Headers& headers) {
    std::string path(uri.getPathAndQuery());
    if (path.empty()) path = "/";

    httplib::Client cli(uri.getHost(), uri.getPort());

    cli.set_follow_location(true);
    cli.set_keep_alive_max_count(1);
    cli.set_timeout_sec(Settings::GetInt("core.timeouts"));

    Proxy* proxy;
    int retries = 0;
    while (retries < 3) {
        if (this->GetProxyCount()) {
            proxy = this->GetRandomProxy();
            cli.set_proxy(proxy->GetHost().c_str(), proxy->GetPort());
            if (!this->GetAuth().first.empty()) {
                cli.set_proxy_basic_auth(this->GetAuth().first.c_str(), this->GetAuth().second.c_str());
            }
        }

        auto resp = cli.Get(path.c_str(), headers);
        if (proxy) proxy->setLocked(false);

        if (Globals::GetStatus() != Status::DUMPING)
            retries++;

        Globals::IncreaseRequestCount(1);
        if (resp) {
            auto a = resp->body;
            resp.reset();
            return a;
        }
        else  {
            resp.reset();
            Globals::IncreaseErrorCount(1);
            //if (proxy) proxy->setAlive(false);
        }

    }
    return "";
}

std::string ProxyManager::Post(const Poco::URI& uri, const httplib::Headers& headers, const std::string &ua, const std::string& data, const std::string& contenttype) {
    std::string path(uri.getPathAndQuery());
    if (path.empty()) path = "/";

    httplib::Client cli(uri.getHost(), uri.getPort());
    cli.set_follow_location(true);
    cli.set_keep_alive_max_count(1);
    cli.set_timeout_sec(Settings::GetInt("core.timeouts"));

    Proxy* proxy;
    int retries = 0;
    while (retries < 3) {
        if (this->GetProxyCount()) {
            proxy = this->GetRandomProxy();
            cli.set_proxy(proxy->GetHost().c_str(), proxy->GetPort());
            if (!this->GetAuth().first.empty()) {
                cli.set_proxy_basic_auth(this->GetAuth().first.c_str(), this->GetAuth().second.c_str());
            }
        }

        auto resp = cli.Post(path.c_str(), headers, data, contenttype.c_str());
        if (proxy) proxy->setLocked(false);

        if (Globals::GetStatus() != Status::DUMPING)
            retries++;

        Globals::IncreaseRequestCount(1);
        if (resp) {
            auto a = resp->body;
            resp.reset();
            return a;
        }
        else  {
            resp.reset();
            Globals::IncreaseErrorCount(1);
            //if (proxy) proxy->setAlive(false);
        }
    }
    return "";
}

std::string ProxyManager::Handle(Request req) {
    if (req._method == "GET") {
        if (req._url.getScheme() == "http")
            return this->Get(req._url, req._headers);
        else
            return this->SSLGet(req._url, req._headers);
    } else if (req._method == "POST") {
        if (req._url.getScheme() == "http")
            return this->Post(req._url, req._headers, req._ua, req._data, req._contenttype);
        else
            return this->SSLPost(req._url, req._headers, req._ua, req._data, req._contenttype);

    }
    return std::string();
}

std::string ProxyManager::SSLGet(const Poco::URI& uri, const httplib::Headers &headers) {
    std::string path(uri.getPathAndQuery());
    if (path.empty()) path = "/";

    httplib::SSLClient cli(uri.getHost(), uri.getPort());

    cli.set_follow_location(true);
    cli.enable_server_certificate_verification(false);
    cli.set_keep_alive_max_count(1);
    cli.set_timeout_sec(Settings::GetInt("core.timeouts"));

    Proxy* proxy = nullptr;
    int retries = 0;
    while (retries < 3) {
        if (this->GetProxyCount()) {
            proxy = this->GetRandomProxy();
            cli.set_proxy(proxy->GetHost().c_str(), proxy->GetPort());
            if (!this->GetAuth().first.empty()) {
                cli.set_proxy_basic_auth(this->GetAuth().first.c_str(), this->GetAuth().second.c_str());
            }
        }

        auto resp = cli.Get(path.c_str(), headers);
        if (proxy) proxy->setLocked(false);

        if (Globals::GetStatus() != Status::DUMPING)
            retries++;

        Globals::IncreaseRequestCount(1);
        if (resp) {
            auto a = resp->body;
            resp.reset();
            return a;
        }
        else {
            resp.reset();
            Globals::IncreaseErrorCount(1);
        }
    }
    return "";
}

std::string ProxyManager::SSLPost(const Poco::URI &uri, const httplib::Headers &headers, const std::string &ua,
                                  const std::string &data, const std::string &contenttype) {
    std::string path(uri.getPathAndQuery());
    if (path.empty()) path = "/";

    httplib::SSLClient cli(uri.getHost(), uri.getPort());
    cli.set_follow_location(true);
    cli.enable_server_certificate_verification(false);
    cli.set_keep_alive_max_count(1);
    cli.set_timeout_sec(Settings::GetInt("core.timeouts"));

    Proxy* proxy;
    int retries = 0;
    while (retries < 3) {
        if (this->GetProxyCount()) {
            proxy = this->GetRandomProxy();
            cli.set_proxy(proxy->GetHost().c_str(), proxy->GetPort());
            if (!this->GetAuth().first.empty())
                cli.set_proxy_basic_auth(this->GetAuth().first.c_str(), this->GetAuth().second.c_str());
        }

        auto resp = cli.Post(path.c_str(), headers, data, contenttype.c_str());
        if (proxy) proxy->setLocked(false);

        if (Globals::GetStatus() != Status::DUMPING)
            retries++;

        Globals::IncreaseRequestCount(1);
        if (resp) {
            auto a = resp->body;
            resp.reset();
            return a;
        }
        else  {
            resp.reset();
            Globals::IncreaseErrorCount(1);
            //if (proxy) proxy->setAlive(false);
        }
    }
    return "";
}

int ProxyManager::GetLivingProxies() {
    int i = 0;
    for (auto & a : this->proxies) {
        if (a.GetAlive()) i++;
    }
    return i;
}

std::string ProxyManager::TestLivingUrl(const Poco::URI &uri) {
    auto url = Poco::URI(uri);
    auto params = url.getQueryParameters();
    for (auto & param : params) {
        param.second = param.second + "'";
    }

    if (url.getScheme() == "https") {
        return SSLGet(uri, {{"User-Agent", Utils::random_ua()}});
    } else {
        return Get(uri, {{"User-Agent", Utils::random_ua()}});
    }
}
