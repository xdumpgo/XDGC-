#include "Proxy.h"
#include "utils/Utils.h"
#include <antireversal/antidebug.h>

#include <utility>

Proxy::Proxy() {
	this->_alive = true;
	this->_inUse = false;
}

Proxy::Proxy(std::string host, const std::string& port) : Proxy(std::move(host), atoi(port.c_str())){}

Proxy::Proxy(std::string host, int port) : Proxy() {
    if (Anti::Debugging::DebugObjectCheck()) {
        exit(0);
    }
    this->_host = std::move(host);
    this->_port = port;
}

Proxy::Proxy(const std::string& address) : Proxy() {
	std::vector<std::string> parts = Utils::explode(address, ':');
	this->_host = parts[0];
	this->_port = atoi(parts[1].c_str());
}

std::string Proxy::GetHost() {
    return this->_host;
}

int Proxy::GetPort() {
    return this->_port;
}

bool Proxy::isLocked() const {
    return this->_inUse;
}

void Proxy::setLocked(bool l) {
    this->_inUse = l;
}

bool Proxy::GetAlive() const {
    return this->_alive;
}

void Proxy::setAlive(bool a) {
    this->_alive = a;
}
