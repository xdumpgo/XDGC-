#include <fstream>
#include <utility>
#include "Globals.h"

std::atomic_int Globals::_requests;
std::atomic_int Globals::_errors;
std::atomic_int Globals::_index;
std::atomic_int Globals::_end;

concurrency::concurrent_vector<std::string> Globals::_urls;
concurrency::concurrent_vector<Injection::Injectable> Globals::_injectables;
concurrency::concurrent_queue<Output> Globals::_output;

std::vector<std::string> Globals::_dorks;

int Globals::_genDorks;
std::vector<std::string> Globals::_patterns = {};
std::vector<std::pair<std::string, std::vector<std::string>>> Globals::_params = {};

Status Globals::_status = STARTUP;

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

void Globals::IncreaseRequestCount(int n) {
	Globals::_requests += n;
}

void Globals::IncreaseErrorCount(int n) {
	Globals::_errors += n;
}

int Globals::GetIndex() {
    return Globals::_index;
}

void Globals::IncreaseIndex(int n) {
    Globals::_index += n;
}

void Globals::AddUrl(const std::string& url) {
    if (url.rfind("http", 0) != 0 && url.rfind('?', 0) != 0 && url.rfind('=', 0) != 0)
        return;
    _output.push(Output(url, Output::URLS));
    _urls.push_back(url);
}

void Globals::AddInjection(Injection::Injectable injection) {
    for (auto & i : _injectables)
        if (i.GetUrl().getHost() == injection.GetUrl().getHost())
            return;
    _output.push(Output(injection.ToString(), Output::INJECTABLES));
    _injectables.push_back(injection);
}

void Globals::LoadDorks(const std::string& path) {
    std::ifstream dorkfile(path.c_str());
    std::string line;
    _dorks.clear();
    while (std::getline(dorkfile, line)) {
        _dorks.emplace_back(line);
    }
}

std::string Globals::GetDork(int i) {
    return _dorks[i];
}

int Globals::GetDorkCount() {
    return _dorks.size();
}

int Globals::GetEnd() {
    return _end;
}

void Globals::SetEnd(int n) {
    _end = n;
}

void Globals::ResetCounters() {
    _index = 0;
    _end = 0;
    _requests = 0;
    _errors = 0;
}

int Globals::GetInjectionCount() {
    return _injectables.size();
}

int Globals::GetUrlCount() {
    return _urls.size();
}

std::vector<std::string> Globals::GetUrls() {
    return {_urls.begin(), _urls.end()};
}

std::vector<Injection::Injectable> Globals::GetInjectables() {
    return {_injectables.begin(), _injectables.end()};
}

Status Globals::GetStatus() {
    return _status;
}

void Globals::SetStatus(Status status) {
    _status = status;
}


void Globals::SetParams(const std::string& name) {
    _params.emplace_back(std::make_pair(name, std::vector<std::string>()));
}

std::vector<std::pair<std::string, std::vector<std::string>>>* Globals::GetParams() {
    return &_params;
}

std::vector<std::string> *Globals::GetPatterns() {
    return &_patterns;
}

int Globals::GetPatternsCount() {
    return _patterns.size();
}

void Globals::SetGenDorks(int value) {
    _genDorks = value;
}

int Globals::GetGenDorks() {
    return _genDorks;
}

void Globals::AddGenDorks(int value) {
    _genDorks += value;
}

void Globals::ClearUrls() {
    _urls.clear();
}

void Globals::LoadUrls(const std::string& path) {
    std::ifstream dorkfile(path.c_str());
    std::string line;
    _urls.clear();
    while (std::getline(dorkfile, line)) {
        _urls.push_back(line);
    }
}

std::vector<std::string> Globals::GetDorks() {
    return Globals::_dorks;
}

int Globals::GetDorksCount() {
    return _dorks.size();
}

int Globals::GetInjectableCount() {
    return _injectables.size();
}
