#ifndef XDG_UTILS_H
#define XDG_UTILS_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <functional>
#include <cassert>
#include <regex>
#include <direct.h>
#include <filesystem>
#include <fstream>
#include <windows.h>
#include <unordered_map>

#include <injection/Injection.h>

namespace Utils {
    enum ConsoleColor {
        DARK_BLUE = 1,
        DARK_GREEN,
        DARK_CYAN,
        DARK_RED,
        DARK_PURPLE,
        BROWN,
        LIGHT_GRAY,
        DARK_GRAY,
        LIGHT_BLUE,
        LIGHT_GREEN,
        LIGHT_CYAN,
        LIGHT_RED,
        LIGHT_PURPLE,
        YELLOW,
        WHITE,
    };

    static std::vector<std::string> explode(const std::string &s, const char &c) {
        std::string buff;
        std::vector<std::string> v;

        for (auto n : s) {
            if (n != c) buff += n;
            else if (n == c && !buff.empty()) {
                v.push_back(buff);
                buff = "";
            }
        }
        if (!buff.empty()) v.push_back(buff);

        return v;
    }

    static HANDLE GetConsoleHandle() {
        return GetStdHandle(STD_OUTPUT_HANDLE);
    }

    static void logInfo(const std::string& msg) {
        std::cout << "\t[";
        SetConsoleTextAttribute(GetConsoleHandle(), LIGHT_BLUE);
        std::cout << "II";
        SetConsoleTextAttribute(GetConsoleHandle(), WHITE);
        std::cout << "] " << msg << std::endl;
    }

    static void logWarn(const std::string& msg) {
        std::cout << "\t[";
        SetConsoleTextAttribute(GetConsoleHandle(), YELLOW);
        std::cout << "!!";
        SetConsoleTextAttribute(GetConsoleHandle(), WHITE);
        std::cout << "] " << msg << std::endl;
    }

    static void logError(const std::string& msg) {
        std::cout << "\t[";
        SetConsoleTextAttribute(GetConsoleHandle(), LIGHT_RED);
        std::cout << "!!";
        SetConsoleTextAttribute(GetConsoleHandle(), WHITE);
        std::cout << "] " << msg << std::endl;
    }

    static std::string GetInput() {
        std::string s;
        std::cin >> s;
        return s;
    }

    static std::string gen_random(const int len) {
        static const char *alphanum = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        std::string s;
        for (int i = 0; i < len; ++i) {
            s += alphanum[rand() % (strlen(alphanum) - 1)];
        }
        return s;
    }

    static void hex_str(std::string &s) {
        std::stringstream ss;
        ss << std::hex << s;
        s = ss.str();
    }

    static std::string random_ua() {
        static const char *ualist[] = {
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.157 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36",
                "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36",
                "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.157 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36",
                "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36",
                "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.71 Safari/537.36",
                "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/21.0.1180.83 Safari/537.1",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.100 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.88 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.108 Safari/537.36",
                "Mozilla/5.0 (Windows NT 5.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36",
                "Mozilla/5.0 (Windows NT 6.2; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36",
                "Mozilla/5.0 (Windows NT 6.3; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.131 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.132 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3440.106 Safari/537.36",
                "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.88 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36",
                "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.100 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.186 Safari/537.36",
                "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36",
                "Mozilla/5.0 (Linux; Android 6.0.1; RedMi Note 5 Build/RB3N5C; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/68.0.3440.91 Mobile Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.117 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.102 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/76.0.3809.100 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.110 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.77 Safari/537.36",
                "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36",
                "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36",
                "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.108 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.163 Safari/537.36",
                "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.143 Safari/537.36",
                "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.112 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.100 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.84 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.181 Safari/537.36",
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.142 Safari/537.36",
                "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36"
        };

        return ualist[rand() % 49];
    }

    static int count_char(const std::string &str, char needle) {
        int counter = 0;
        for (char i : str) {
            if (i == needle) {
                counter++;
            }
        }
        return counter;
    }

    template<class Container>
    void string_split(const std::string &str, Container &cont, char delim = ' ') {
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delim)) {
            cont.push_back(token);
        }
    }

    inline std::string format(const char *fmt, ...) {
        int size = 512;
        char *buffer = nullptr;
        buffer = new char[size];
        va_list vl;
                va_start(vl, fmt);
        int nsize = vsnprintf(buffer, size, fmt, vl);
        if (size <= nsize) { //fail delete buffer and try again
            delete[] buffer;
            buffer = nullptr;
            buffer = new char[nsize + 1]; //+1 for /0
            nsize = vsnprintf(buffer, size, fmt, vl);
        }
        std::string ret(buffer);
                va_end(vl);
        delete[] buffer;
        return ret;
    }

    static std::string ExtractValue(std::string data) {
        std::replace(data.begin(), data.end(), '\r', ' ');
        std::replace(data.begin(), data.end(), '\n', ' ');

        int beg = data.find(Injection::_flag1);
        int end = data.find(Injection::_flag2);

        if ((beg == -1 || end == -1) || beg == end) {
            return "";
        }

        auto x = beg + 4;
        auto v = end - x;

        auto a = data.substr(x, v);

        return a;
    }

    static std::string get_current_dir() {
        char buff[FILENAME_MAX]; //create string buffer to hold path
        _getcwd(buff, FILENAME_MAX);
        std::string current_working_dir(buff);
        return current_working_dir;
    }

/*! note: delimiter cannot contain NUL characters
 */
    template<typename Range, typename Value = typename Range::value_type>
    std::string Join(Range const &elements, const char *const delimiter) {
        std::ostringstream os;
        auto b = begin(elements), e = end(elements);

        if (b != e) {
            std::copy(b, prev(e), std::ostream_iterator<Value>(os, delimiter));
            b = prev(e);
        }
        if (b != e) {
            os << *b;
        }

        return os.str();
    }

    static BOOL DirectoryExists(const char* dirName) {
        DWORD attribs = ::GetFileAttributesA(dirName);
        if (attribs == INVALID_FILE_ATTRIBUTES) {
            return false;
        }
        return (attribs & FILE_ATTRIBUTE_DIRECTORY);
    }

    static void CreateParamsFiles(){
        std::string paths[7] = {"params/(KW).txt","params/(PF).txt","params/(PT).txt","params/(SF).txt","params/(DM).txt","params/(NUM).txt","patterns.txt"};
        for(const auto& path : paths){
            std::ifstream iuF(path.c_str());
            if (!iuF.good()) {
                std::ofstream uF(path.c_str());
                uF.close();
            } else {
                iuF.close();
            }
        }
    }
}

#endif // !XDG_UTILS_H