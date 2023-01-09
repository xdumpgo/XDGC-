#ifndef XDG_PROXY_
#define XDG_PROXY_

#include "Globals.h"

class Proxy
{
private:
	std::string _host;
	int _port;
	bool _inUse;
	bool _alive;
public:
	Proxy();
	Proxy(const std::string& address);
	Proxy(std::string host, int port);
	Proxy(std::string host, const std::string& port);

	std::string GetHost();
	int GetPort();
	bool GetAlive() const;
	void setAlive(bool a);
	bool isLocked() const;
	void setLocked(bool l);
};

#endif