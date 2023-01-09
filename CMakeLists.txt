cmake_minimum_required(VERSION 3.16)
project(XDG)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

find_package(Poco REQUIRED Foundation Net NetSSL JSON ZIP Data)
find_package(RapidJSON CONFIG REQUIRED)
find_package(unofficial-breakpad CONFIG REQUIRED)
find_package(unofficial-concurrentqueue CONFIG REQUIRED)
find_package(Cpp-Taskflow CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(unofficial-mongoose CONFIG REQUIRED)

link_directories(C:/vcpkg/installed/x64-windows/lib)
include_directories(C:/vcpkg/installed/x64-windows/include)

add_definitions(-DXDG_DEBUG)
add_definitions(-DWIN32_LEAN_AND_MEAN)
add_definitions(-DCPPHTTPLIB_OPENSSL_SUPPORT)

add_executable(XDG
        Globals.cpp
        Globals.h
        injection/Injectable.cpp
        injection/Injectable.h
        proxymanager/Proxy.cpp
        proxymanager/Proxy.h
        proxymanager/ProxyManager.cpp
        proxymanager/ProxyManager.h
        settings/Settings.cpp
        settings/Settings.h
        utils/Utils.h
        XDG.cpp
        auth/quartzauth.cpp
        auth/quartzauth.h
        utils/base64.cpp
        utils/base64.h
        utils/md5.cpp
        utils/md5.h
        utils/hwid.cpp
        utils/hwid.h
        utils/ThreadPool.cpp
        utils/ThreadPool.h
        utils/url.cpp
        utils/url.h
        modules/Module.h
        modules/Scraper.cpp
        modules/Scraper.h
        modules/Generator.cpp
        modules/Generator.h
        modules/dorkers/Dorker.cpp
        modules/dorkers/Dorker.h
        modules/dorkers/Google.cpp
        modules/dorkers/Google.h
        modules/dorkers/Bing.cpp
        modules/dorkers/Bing.h
        modules/dorkers/Aol.cpp
        modules/dorkers/Aol.h
        modules/dorkers/MyWebSearch.cpp
        modules/dorkers/MyWebSearch.h
        modules/dorkers/Yahoo.cpp
        modules/dorkers/Yahoo.h
        modules/dorkers/DuckDuckGo.cpp
        modules/dorkers/DuckDuckGo.h
        modules/dorkers/Ecosia.cpp
        modules/dorkers/Ecosia.h
        modules/dorkers/Qwant.cpp
        modules/dorkers/Qwant.h
        modules/dorkers/Startpage.cpp
        modules/dorkers/Startpage.h
        modules/dorkers/Yandex.cpp
        modules/dorkers/Yandex.h
        antireversal/antidebug.h
        antireversal/antireversal.cpp
        gumbo-query/Document.cpp
        gumbo-query/Document.h
        gumbo-query/Node.cpp
        gumbo-query/Node.h
        gumbo-query/Object.cpp
        gumbo-query/Object.h
        gumbo-query/Parser.cpp
        gumbo-query/Parser.h
        gumbo-query/QueryUtil.cpp
        gumbo-query/QueryUtil.h
        gumbo-query/Selection.cpp
        gumbo-query/Selection.h
        gumbo-query/Selector.cpp
        gumbo-query/Selector.h
        modules/Exploiter.cpp
        modules/Exploiter.h
        injection/mysql/Error.h
        injection/Injection.cpp
        injection/Injection.h
        injection/mysql/Queries.h
        injection/mysql/Union.h
        modules/Cleaner.cpp
        modules/Cleaner.h
        utils/blockingqueue.h injection/tamper/Tamper.cpp injection/tamper/Tamper.h injection/tamper/Space2Comment.cpp injection/tamper/Space2Comment.h injection/tamper/Equal2Like.cpp injection/tamper/Equal2Like.h)

target_include_directories(XDG PRIVATE ${RAPIDJSON_INCLUDE_DIRS})

target_link_libraries(XDG PRIVATE Poco::Net Poco::NetSSL Poco::XML Poco::Zip Poco::Data unofficial::breakpad::libbreakpad unofficial::breakpad::libbreakpad_client libconfig++ gumbo unofficial::concurrentqueue::concurrentqueue OpenSSL::SSL OpenSSL::Crypto unofficial::mongoose::mongoose)