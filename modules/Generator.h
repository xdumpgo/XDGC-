//
// Created by xiQQ on 5/1/2020.
//

#ifndef XDG_GENERATOR_H
#define XDG_GENERATOR_H

#include "Module.h"
#include <Poco/String.h>
#include <Poco/StringTokenizer.h>
#include <Poco/NumberFormatter.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

namespace Modules{
    class Generator : public Module<Generator>{
    public:
        Generator();
        void run() override;
        static void Stop();
        static void Init();

        static void LoadParameters();
        static void LoadPatterns();
        static void InsertParam(std::string name);
        static void RemoveLastParam();
    private:

        static std::vector<std::pair<std::string, std::string>> _patternsConversions;
        void DorksBuilder(int loop, std::vector<std::string> param, std::vector<std::string>& dorks);
        static void ConvertPattern(std::string& pattern);
        int Combinations(const std::string& pattern);
        //void GetLimit(std::string& pattern, int& combinations);
        void SaveDorks(const std::vector<std::string>& param);
    };
}
#endif //XDG_GENERATOR_H
