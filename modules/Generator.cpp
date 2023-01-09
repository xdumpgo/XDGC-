//
// Created by xiQQ on 5/1/2020.
//

#include "Generator.h"

using namespace Modules;

Generator::Generator() = default;

std::vector<std::pair<std::string, std::string>> Generator::_patternsConversions = {};

void Generator::Init(){
    Globals::SetParams("params/(KW).txt");
    Globals::SetParams("params/(PF).txt");
    Globals::SetParams("params/(PT).txt");
    Globals::SetParams("params/(SF).txt");
    Globals::SetParams("params/(DM).txt");
    Globals::SetParams("params/(NUM).txt");

    _patternsConversions.emplace_back("(KW)","10");
    _patternsConversions.emplace_back("(PF)","11");
    _patternsConversions.emplace_back("(PT)","12");
    _patternsConversions.emplace_back("(SF)","13");
    _patternsConversions.emplace_back("(DM)","14");
    _patternsConversions.emplace_back("(NUM)","15");

    Globals::SetGenDorks(0);
}

void Generator::LoadPatterns() {
    std::ifstream patternfile("patterns.txt");
    std::string line;
    auto* patterns_ptr = Globals::GetPatterns();
    patterns_ptr->clear();
    while(std::getline(patternfile, line)){
        Generator::ConvertPattern(line);
        patterns_ptr->emplace_back(line);
    }
    patterns_ptr = nullptr;
}

void Generator::LoadParameters() {
    auto* params_ptr = Globals::GetParams();
    for(auto& param : *params_ptr){
        param.second.clear();
        std::ifstream paramsfile(param.first);
        std::string line;
        while(std::getline(paramsfile, line)){
            param.second.emplace_back(line);
        }
    }
    params_ptr = nullptr;
}

void Generator::ConvertPattern(std::string& pattern) {
        for (auto& param : Generator::_patternsConversions){
            if(pattern.find(param.first) != std::string::npos){
                Poco::replaceInPlace(pattern,param.first, param.second);
            }
        }
}

int Generator::Combinations(const std::string& pattern) {
    int combinations = 1;
    auto params_ptr = Globals::GetParams();
    for(auto& conversion : Generator::_patternsConversions){
        if(pattern.find(conversion.second) != std::string::npos){
            for(const auto& param : *params_ptr){
                if (param.first.find(conversion.first) != std::string::npos){
                    combinations *= param.second.size();
                }
            }
        }
    }
    params_ptr = nullptr;
    return combinations;
}

void Generator::SaveDorks(const std::vector<std::string>& param) {
    std::ofstream dorks("dorks.txt", std::fstream::app);
    std::ostream_iterator<std::string> output_iterator(dorks,"\n");
    std::copy(param.begin(), param.end(), output_iterator);
}

void Generator::DorksBuilder(int loop, std::vector<std::string> param, std::vector<std::string>& dorks) {
    int size = param.size();
    if (dorks.empty()){
        for (int i = 0; i < loop; i++){
            for(int z= 0; z < size; z++) {
                dorks.emplace_back(param[z]);
            }
        }
    }
    else{
        int k = 0;
        for (int i = 0; i < loop; i++){
            for(int z= 0; z < size; z++) {
                dorks[k].append(param[z]);
                k++;
            }
        }
    }

}

void Generator::run() {
    Globals::SetGenDorks(0);
    auto* patterns_ptr = Globals::GetPatterns();
    auto* params_ptr = Globals::GetParams();

    for(std::string pattern : *patterns_ptr){
        int loop = 0;
        int combinations = this->Combinations(pattern);
        //this->GetLimit(pattern, combinations);
        Globals::AddGenDorks(combinations);
        std::vector<std::string> dorks = {};
        int paramsAmount = pattern.size() / 2;

        Globals::SetStatus(GENERATING);
        for(int i = 0; i < paramsAmount; i++){
            for (const auto& conversion :_patternsConversions){
                std::string p = pattern.substr(0,2);
                if(p == conversion.second){
                    for(const auto& param : *params_ptr){
                        if (param.first.find(conversion.first) != std::string::npos){
                            if(!param.second.empty()){
                                loop = combinations / param.second.size();
                                this->DorksBuilder(loop, param.second,dorks);
                            }
                        }
                    }
                    pattern.erase(0,2);
                }
            }
        }
        this->SaveDorks(dorks);
    }
}

void Generator::InsertParam(std::string name) {
    auto conversion = Poco::NumberFormatter::format(atoi(_patternsConversions.back().second.c_str()) + 1);
    _patternsConversions.emplace_back(name, conversion);
    Globals::SetParams("params/"+name+".txt");
    std::ifstream iuF(std::string("params/"+name+".txt").c_str());
    if (!iuF.good()) {
        std::ofstream uF(std::string("params/"+name+".txt").c_str());
        uF.close();
    } else {
        iuF.close();
    }
}

void Generator::RemoveLastParam(){
    auto* params_ptr = Globals::GetParams();
    if(_patternsConversions.size() > 6 || params_ptr->size() > 6){
        _patternsConversions.pop_back();
        std::string filename = params_ptr->back().first;
        params_ptr->pop_back();
       std::filesystem::remove(filename);
    }
    params_ptr = nullptr;
}

void Generator::Stop() {
    auto* params_ptr = Globals::GetParams();
    auto* patterns_ptr = Globals::GetPatterns();
    params_ptr->clear();
    patterns_ptr->clear();
    _patternsConversions.clear();
}

//void Generator::GetLimit(std::string &pattern, int& combinations) {
//    if (pattern.find(":") != std::string::npos){
//        auto s = Poco::StringTokenizer(pattern, ":");
//        for(int i = 0; i < s.count(); i++){
//            combinations = atoi(s[i].c_str());
//        }
//        if(combinations != 0){
//            s[s.count()-1].clear();
//           pattern = Poco::cat(std::string(""),s.begin(),s.end());
//       }
//    }
//}
