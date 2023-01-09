#ifndef XDG_PROXYMANAGER_
#define XDG_PROXYMANAGER_

#include "Proxy.h"
#include <utility>
#include <vector>
#include <string>
#include <random>
#include <Poco/URI.h>
#include <utils/Utils.h>
#include <atomic>

enum ProxyType {
	HTTP,
	SOCKS4,
	SOCKS5
};

typedef bool (*ResponseHandler)(std::string);

struct Request {
    Poco::URI _url;
    std::string _ua;
    std::string _method;
    std::string _data;
    std::string _contenttype;
    httplib::Headers _headers;
    ResponseHandler _handler;

    struct Done {
        std::atomic_bool _flag = false;
        Done() = default;
    };

    Request()= default;

    Request(const Poco::URI& u, std::string ua, ResponseHandler h = nullptr, std::string method = "GET", std::string data = "", std::string contenttype = "", httplib::Headers headers = {{"User-Agent", Utils::random_ua()}}) {
        _url = u;
        _ua = std::move(ua);
        _method = std::move(method);
        _handler = h;
        _headers = std::move(headers);
    }
};

class ProxyManager
{
private:
	std::vector<Proxy> proxies;
	ProxyType _proxyType;
    std::random_device rd;
    std::mt19937 mt;
    std::pair<std::string,std::string> _auth;
public:
	ProxyManager();

	int LoadProxies(const std::string& path);

	void SetProxyType(ProxyType _type);
	ProxyType GetProxyType();

	Proxy* GetRandomProxy();
	Proxy GetProxy(int n);

	int GetProxyCount();
	int GetLivingProxies();

	std::pair<std::string,std::string> GetAuth();

    std::string TestLivingUrl(const Poco::URI& uri);

    std::string Handle(Request req);

    std::string Get(const Poco::URI& uri, const httplib::Headers& headers);
    std::string SSLGet(const Poco::URI& uri, const httplib::Headers& headers);
    std::string Post(const Poco::URI& uri, const httplib::Headers& headers, const std::string& ua = "Mozilla/5.0 (X11; Linux x86_64; rv:74.0) Gecko/20100101 Firefox/74.0", const std::string& data = "", const std::string& contenttype = "");
    std::string SSLPost(const Poco::URI& uri, const httplib::Headers& headers, const std::string& ua = "Mozilla/5.0 (X11; Linux x86_64; rv:74.0) Gecko/20100101 Firefox/74.0", const std::string& data = "", const std::string& contenttype = "");

	void Reset();
};

extern ProxyManager* PManager;

#endif