#ifndef XDG_GLOBALS_
#define XDG_GLOBALS_

#include <httplib.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <atomic>
#include <string>
#include <ctime>
#include <deque>
#include <mutex>

#include <concurrentqueue/concurrentqueue.h>
#include <concurrent_vector.h>
#include <concurrent_queue.h>
#include <injection/Injectable.h>

enum Status {
    STARTUP,
    IDLE,
    SCRAPING,
    GENERATING,
    EXPLOITING,
    DUMPING,
};


struct Output {
    enum OType {
        URLS,
        INJECTABLES,
        DUMPS
    };

    std::string data;
    OType type;
    Output()= default;

    Output(std::string data, OType type) : data(std::move(data)), type(type) { }
};

struct Globals
{
    static concurrency::concurrent_queue<Output> _output;
private:
	static std::atomic_int _requests;
	static std::atomic_int _errors;
	static std::atomic_int _index;
	static std::atomic_int _end;

    static concurrency::concurrent_vector<std::string> _urls;
    static std::vector<std::string> _dorks;
    //static std::deque<Injection::Injectable> _injectables;
    static concurrency::concurrent_vector<Injection::Injectable> _injectables;

    static Status _status;

    static int _genDorks;
    static std::vector<std::string> _patterns;
    static std::vector<std::pair<std::string, std::vector<std::string>>> _params;
public:
	static int GetRequestCount() { return Globals::_requests; }
	static void IncreaseRequestCount(int n);
	static int GetErrorCount() { return Globals::_errors; }
	static void IncreaseErrorCount(int n);
	static int GetIndex();
	static void IncreaseIndex(int n);
	static int GetEnd();
	static void SetEnd(int n);
    static void ResetCounters();

	static void AddUrl(const std::string& url);
	static int GetUrlCount();
	static int GetDorksCount();
	static int GetInjectableCount();
	static std::vector<std::string> GetDorks();
	static std::vector<std::string> GetUrls();
    static std::vector<Injection::Injectable> GetInjectables();
	static void ClearUrls();
	static void LoadUrls(const std::string& path);
	static void AddInjection(Injection::Injectable injection);
	static int GetInjectionCount();

	static std::string GetDork(int i);
	static void LoadDorks(const std::string& path);
	static int GetDorkCount();

	static Status GetStatus();
	static void SetStatus(Status status);

    static int GetGenDorks();
    static void AddGenDorks(int value);
    static int GetPatternsCount();
    static void SetGenDorks(int value);
    static void SetParams(const std::string& name);
    static std::vector<std::string>* GetPatterns();
    static std::vector<std::pair<std::string, std::vector<std::string>>>* GetParams();
};

#endif