//
// Created by zerte on 4/24/2020.
//

#include "Dorker.h"

std::vector<Dorker*> Dorkers;

std::string Dorker::getName() {
    return this->_name;
}

std::vector<std::string> Dorker::getBases() {
    return this->_bases;
}

bool Dorker::getEnabled() {
    return Settings::GetBool("scraper.dorkers." + this->_name);
}

void Dorker::setEnabled(bool n) {
    Settings::Set("scraper.dorkers." + this->_name, n);
}
