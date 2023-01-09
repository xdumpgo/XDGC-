//
// Created by zerte on 4/19/2020.
//

#ifndef XDG_MODULE_H
#define XDG_MODULE_H

#include <string>
#include <utility>
#include <vector>
#include <httplib.h>
#include <deque>
#include <chrono>
#include <utils/ThreadPool.h>
#include <date/date.h>
#include <modules/dorkers/Bing.h>
#include <modules/dorkers/Aol.h>
#include <modules/dorkers/Google.h>
#include <modules/dorkers/MyWebSearch.h>
#include <modules/dorkers/Yahoo.h>
#include <modules/dorkers/DuckDuckGo.h>
#include <modules/dorkers/Ecosia.h>
#include <modules/dorkers/Qwant.h>
#include <modules/dorkers/startpage.h>
#include <modules/dorkers/Yandex.h>

#include <Poco/ThreadPool.h>
#include <Poco/Runnable.h>

#include <boost/asio/io_service.hpp>

namespace Modules {
    static void InitDorkers() {
        Dorkers.emplace_back(new Google());
        Dorkers.emplace_back(new Bing());
        Dorkers.emplace_back(new Aol());
        Dorkers.emplace_back(new MyWebSearch());
        //Dorkers.emplace_back(new Yahoo());
        Dorkers.emplace_back(new DuckDuckGo());
        Dorkers.emplace_back(new Ecosia());
        //Dorkers.emplace_back(new Qwant());
        Dorkers.emplace_back(new Startpage());
        Dorkers.emplace_back(new Yandex());
    }

    template <typename T>
    class Module : public Poco::Runnable {
    private:
        static T* _instance;
        std::string _name;
        bool _running = false;

    protected:
        Module(){};
        ThreadPool *_pool{};
        std::chrono::system_clock::time_point _timer;
        std::ofstream _output;
    public:
        static T* GetInstance() {
            if (!_instance) {
                _instance = new T;
            }
            return _instance;
        }

        std::string GetName() {
            return this->_name;
        }

        bool GetRunning() {
            return this->_running;
        }

        void SetRunning(bool running) {
            this->_running = running;
        }

        std::string GetRuntime() {
            auto time = date::make_time(std::chrono::system_clock::now() - _timer);
            return Utils::format("%02d:%02d:%02d", time.hours().count(), time.minutes().count(), time.seconds().count());
        }

        int GetWorkerCount() {
            return this->_pool->Size();
        }

        int GetWorkCount() {
            return this->_pool->JobsRemaining();
        }

        virtual void run() = 0;

        void Stop() {
            free(_pool);
            this->SetRunning(false);
        }

        void AddToQueue(Request *request) {
            //this->_pool->queueWork(request);
        }

        std::ofstream GetFileStream() {
            return this->_output;
        }
    };

    template<typename T>
    T* Module<T>::_instance= NULL;
}

#endif //XDG_MODULE_H
