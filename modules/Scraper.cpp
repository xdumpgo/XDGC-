//
// Created by zerte on 4/20/2020.
//

#include "Scraper.h"

using namespace Modules;

Scraper::Scraper() = default;

void Scraper::run() {
    this->SetRunning(true);
    _timer = std::chrono::system_clock::now();
    _pool = new ThreadPool(Settings::GetInt("core.threads"));
    //_output.open("urls.txt", std::fstream::app);
    Globals::ResetCounters();
    Globals::SetEnd(Globals::GetDorkCount());
    Globals::SetStatus(SCRAPING);
    std::thread a([&]() {
        for (const auto& dork : Globals::GetDorks()) {
            for (auto Dorker : Dorkers) {
                if (Dorker->getEnabled()) {
                    for (int k = 0; k < Dorker->getBases().size(); k++) {
                        for (int j = 0; j < Settings::GetInt("scraper.pages"); j++) {
                            this->_pool->AddJob([Dorker, dork, k, j]() {
                                auto r = Dorker->buildRequest(dork, k, j);
                                r->_handler(PManager->Handle(*r));
                            });
                        }
                    }
                }
            }
            Globals::IncreaseIndex(1);
        }
        this->_pool->WaitAll();
        Scraper::GetInstance()->Stop();
        Globals::SetStatus(IDLE);
    });
    a.detach();
}