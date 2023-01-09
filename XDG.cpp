#include <httplib.h>
#include <settings/Settings.h>
#include <proxymanager/ProxyManager.h>
#include <auth/quartzauth.h>
#include <modules/Scraper.h>
#include <modules/Exploiter.h>
#include <modules/Generator.h>
#include <modules/Cleaner.h>
#include <injection/tamper/Tamper.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>

void PrintBanner();
void ShowSettings();
void ShowData();
void ShowGenerator();
void ShowUtils();

[[noreturn]] static void StatusThread() {
    while(true) {
        std::string str;
        switch(Globals::GetStatus()) {
            case STARTUP:
                str = "XDG V2 | [Startup]";
                break;
            case IDLE:
                str = Utils::format("XDG V2 | [Idle] [Proxies: %d] [Dorks: %d] [Urls: %d] [Injectables: %d]", PManager->GetProxyCount(), Globals::GetDorkCount(), Globals::GetUrlCount(), Globals::GetInjectableCount());
                break;
            case SCRAPING:
                str = Utils::format("XDG | [Scraper] [%s] [%d/%d] [T:%d] [R:%d] [E:%d] ~> [%d]", Modules::Scraper::GetInstance()->GetRuntime().c_str(), Globals::GetIndex(), Globals::GetEnd(),
                                    Modules::Scraper::GetInstance()->GetWorkerCount(), Globals::GetRequestCount(), Globals::GetErrorCount(), Globals::GetUrlCount());
                break;
            case GENERATING:
                str = Utils::format("XDG | [Generator] [Patterns: %d] [Parameters: %d] [Generated %d Dorks]", Globals::GetPatterns()->size(), Globals::GetParams()->size(), Globals::GetGenDorks());
                break;
            case EXPLOITING:
                str = Utils::format("XDG | [Exploiter] [%s] [%d/%d] [T:%d] [R:%d] [E:%d] ~> [%d]", Modules::Exploiter::GetInstance()->GetRuntime().c_str(), Globals::GetIndex(), Globals::GetEnd(),
                                    Modules::Exploiter::GetInstance()->GetWorkerCount(), Globals::GetRequestCount(), Globals::GetErrorCount(), Globals::GetInjectionCount());
                break;
            case DUMPING:
                break;
        }
        //fprintf(stdout, "\r[%d/%d] [R:%d] [E:%d] Scraping... [%d]", Globals::GetIndex(), Globals::GetEnd(), Globals::GetRequestCount(), Globals::GetErrorCount(), Globals::GetUrlCount());
        SetConsoleTitleA(str.c_str());
        Sleep(250);
    }
}

[[ noreturn ]] static void OutputThread() {
    std::ofstream uFile("urls.txt", std::fstream::app);
    std::ofstream iFile("injectables.txt", std::fstream::app);
    while (true) {
        if (!Globals::_output.empty()) {
            Output output;
            if (Globals::_output.try_pop(output))
                switch(output.type) {
                    case Output::URLS:
                        uFile << output.data << std::endl;
                        break;
                    case Output::INJECTABLES:
                        iFile << output.data << std::endl;
                        break;
                    case Output::DUMPS:

                        break;
                }
        }
        Sleep(50);
    }
}

int main()
{
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 14;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Courier New");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    std::thread s(StatusThread);
    std::thread o(OutputThread);
    s.detach();
    o.detach();

    srand(time(nullptr));
    Utils::logInfo("Starting up XDG v2...");

    if (!Utils::DirectoryExists("params")){
        mkdir("params");
    }

    Utils::CreateParamsFiles();

    std::ifstream iuF("urls.txt");
    if (!iuF.good()) {
        std::ofstream uF("urls.txt");
        uF.close();
    } else {
        iuF.close();
    }

    std::ifstream ipF("proxies.txt");
    if (!ipF.good()){
        std::ofstream pF("proxies.txt");
        pF.close();
    } else {
        ipF.close();
    }

    std::ifstream idF("dorks.txt");
    if (!idF.good()) {
        std::ofstream dF("dorks.txt");
        dF.close();
    } else {
        idF.close();
    }

    PManager = new ProxyManager();
    Modules::InitDorkers();
    Settings::Init();
    Injection::Tamper::Init();
    Globals::SetStatus(IDLE);

    while (true) {
        system("cls");
        PrintBanner();
        Utils::logInfo(Utils::format("Welcome to XDumpGO"));
        Utils::logInfo(Utils::format("Your license is good until: Forever"));
        Utils::logInfo(Utils::format("Currently supporting %d injections\r\n", Injection::CountPossibleInjections()));

        printf("\t[0] Settings\r\n");
        printf("\t[1] Data\r\n");
        printf("\t[2] Utils\r\n");
        printf("\t[3] Dork Generator\r\n");
        printf("\t[4] Scraper\r\n");
        printf("\t[5] Exploit\r\n");
        printf("\t[6] Dump\r\n");
        printf("\t[7] Stop\r\n");

        printf("\r\n\r\n\t~> ");
        std::thread statusThread;
        switch (atoi(Utils::GetInput().c_str()))
        {
        case 0: // Settings
            ShowSettings();
            break;
        case 1: // Data
            ShowData();
            break;
        case 2:
            ShowUtils();
            break;
        case 3:
            ShowGenerator();
            break;
        case 4:
            if (Globals::GetStatus() != IDLE) {
                Utils::logError("You already have a module running, please stop it first.");
            } else {
                Modules::Scraper::GetInstance()->run();
            }
            break;
        case 5:
            if (Globals::GetStatus() != IDLE) {
                Utils::logError("You already have a module running, please stop it first.");
            } else {
                Modules::Exploiter::GetInstance()->run();
            }
            break;
        case 7:
            switch (Globals::GetStatus()) {
            case SCRAPING:
                Modules::Scraper::GetInstance()->Stop();
                break;
            case EXPLOITING:
                Modules::Exploiter::GetInstance()->Stop();
                break;
            default:
                Utils::logError("You have nothing running.");
                break;
            }
            Globals::SetStatus(IDLE);
            break;
        default:
            Utils::logError("Invalid entry");
            Sleep(750);
        }
    }
}

void InsertParam(){
    system("cls");
    PrintBanner();

    std::cout << "Enter the name of your parameter:" << std::endl;
    printf("\r\n\t[99] <- Back\r\n\r\n\t~> ");
    std::string name = Utils::GetInput();

    switch (atoi(name.c_str())) {
        case 99:
            return;
        default:
            Modules::Generator::InsertParam(name);
            return;
    }
}

void ShowGenerator(){
    Modules::Generator::Init();
    while(true){
        system("cls");
        PrintBanner();

        auto* params_ptr = Globals::GetParams();
        for (const auto& param : *params_ptr){
            std::string name;
            std::cout << std::endl << "\t[" << param.second.size() << "]" << param.first.substr(7,param.first.size() - 11) << std::endl;
        }
        params_ptr = nullptr;

        std::cout << std::endl << "\t[1]" << "Load Patterns" << std::endl;
        std::cout << "\t[2]" << "Load Parameters" << std::endl;
        std::cout << "\t[3]" << "Generate" << std::endl;

        std::cout << std::endl << "\t[4]" << "Insert Parameter" << std::endl;
        std::cout << "\t[5]" << "Remove Parameter" << std::endl;
        printf("\r\n\t[99] <- Back\r\n\r\n\t~> ");

        switch (atoi(Utils::GetInput().c_str())){
            case 1:
                Modules::Generator::LoadPatterns();
                break;
            case 2:
                Modules::Generator::LoadParameters();

                break;
            case 3:
                Modules::Generator::GetInstance()->run();
                break;
            case 4:
                InsertParam();
                break;
            case 5:
                Modules::Generator::RemoveLastParam();
                break;
            case 99:
                Modules::Generator::Stop();
                Globals::SetStatus(IDLE);
                return;
            default:
                Utils::logError("Invalid selection");
                Sleep(500);
                break;
        }
    }
}


void ShowTechniques() {
    while (true) {
        system("cls");
        PrintBanner();

        std::cout << std::endl << "\t[0]" << std::setw(15)  << " Error Based" << "(" << std::boolalpha << Settings::GetBool("exploiter.err") << ")" << std::endl;
        std::cout << std::endl << "\t[1]" << std::setw(15)  << " Union Based" << "(" << std::boolalpha << Settings::GetBool("exploiter.uni") << ")" << std::endl;
        std::cout << std::endl << "\t[2]" << std::setw(15)  << " Blind Based" << "(" << std::boolalpha << Settings::GetBool("exploiter.bli") << ")" << std::endl;
        std::cout << std::endl << "\t[3]" << std::setw(15)  << " Stacked Based" << "(DISABLED)" << std::endl;

        printf("\r\n\t[99] <- Back\r\n\r\n\t~> ");

        switch (atoi(Utils::GetInput().c_str()))
        {
        case 0:
            Settings::Set("exploiter.err", !Settings::GetBool("exploiter.err"));
            break;
        case 1:
            Settings::Set("exploiter.uni", !Settings::GetBool("exploiter.uni"));
            break;
        case 2:
            Settings::Set("exploiter.bli", !Settings::GetBool("exploiter.bli"));
            break;
        case 99:
            return;
        default:
            Utils::logError("Invalid selection");
            Sleep(500);
            break;
        }
    }
}

void ShowData() {
    while (true) {
        system("cls");
        PrintBanner();
        std::cout << std::endl << "\t[0]" << std::setw(15)  << " Load Proxies" << "(" << PManager->GetProxyCount() << ")" << std::endl;
        std::string _t("HTTP");
        switch (PManager->GetProxyType())
        {
        case HTTP:
            _t = "HTTP";
        case SOCKS4:
            _t = "SOCKS4";
        case SOCKS5:
            _t = "SOCKS5";
        }
        std::cout << std::endl << "\t[1]" << std::setw(15) << " Proxy Type" << "(" << _t << ")" << std::endl;
        std::cout << std::endl << "\t[2]" << std::setw(15) << " Load Dorks" << "(" << Globals::GetDorkCount() << ")" << std::endl;
        std::cout << std::endl << "\t[3]" << std::setw(15) << " Load Urls" << "(" << Globals::GetUrlCount() << ")" << std::endl;

        printf("\r\n\t[99] <- Back\r\n\r\n\t~> ");
        switch (atoi(Utils::GetInput().c_str()))
        {
        case 0:
                PManager->LoadProxies("proxies.txt");
            break;
        case 1:
            switch (PManager->GetProxyType())
            {
            case HTTP:
                PManager->SetProxyType(SOCKS4);
                break;
            case SOCKS4:
                PManager->SetProxyType(SOCKS5);
                break;
            case SOCKS5:
                PManager->SetProxyType(HTTP);
                break;
            }
        case 2:
            Globals::LoadDorks("dorks.txt");
            break;
        case 3:
            Globals::LoadUrls("urls.txt");
            break;
        case 99:
            return;
        default:
            break;
        }

    }
}

void ShowDorkers() {
    while (true) {
        system("cls");
        PrintBanner();

        for (int i=0; i < Dorkers.size(); i++) {
            std::cout << std::endl << "\t[" << i << "] " << std::setw(15) << Dorkers[i]->getName() << "(" << std::boolalpha << Dorkers[i]->getEnabled() << ")" << std::endl;
        }

        std::cout << std::endl << std::setw(15) << "\t[99] <= Back" << std::endl;

        printf("\r\n~> ");
        int i = atoi(Utils::GetInput().c_str());
        if (i == 99) {
            return;
        } else if (i < 0 || i > Dorkers.size()) {
            Utils::logError("Invalid input");
            Sleep(750);
            continue;
        }

        Dorkers[i]->setEnabled(!Dorkers[i]->getEnabled());
    }
}

void ShowSettings() {
    while (true) {
        system("cls");
        PrintBanner();

        std::cout << std::endl << "\t[0]" << std::setw(15)  << " Threads" << "(" << Settings::GetInt("core.threads") << ")" << std::endl;
        std::cout << std::endl << "\t[1]" << std::setw(15)  << " Timeouts"<< "(" << Settings::GetInt("core.timeouts") << ")" << std::endl;
        std::cout << std::endl << "\t[2]" << std::setw(15)  << " Pages" << "(" << Settings::GetInt("scraper.pages") << ")" << std::endl;
        std::cout << std::endl << "\t[3]" << std::setw(15)  << " AutoThreads" << "(DISABLED)" << std::endl;
        std::cout << std::endl << "\t[4]" << std::setw(15)  << " Targeted Dump" << "(" << std::boolalpha << Settings::GetBool("dumper.targeted") << ")" << std::endl;
        std::cout << std::endl << "\t[5]" << std::setw(15)  << " Batch Mode" << "(" << std::boolalpha << Settings::GetBool("core.batch") << ")" << std::endl;
        std::string intensity("Basic");
        switch (Settings::GetInt("exploiter.intensity"))
        {
        case 0:
            intensity = "Basic";
            break;
        case 1:
            intensity = "Moderate";
            break;
        case 2:
            intensity = "Excessive";
            break;
        default:
            break;
        }
        std::cout << std::endl << "\t[6]" << std::setw(15)  << "Intensity" << "(" << intensity << ")" << std::endl;
        std::cout << std::endl << "\t[7]" << std::setw(15)  << "Techniques =>" << std::endl;
        std::cout << std::endl << "\t[8]" << std::setw(15)  << "Dorkers =>" << std::endl;

        printf("\r\n\t[99] <- Back\r\n");

        printf("\r\n\t~> ");
        switch (atoi(Utils::GetInput().c_str())) {
        case 0:
            while (true) {
                system("cls");
                PrintBanner();
                std::cout << "Current Threads: " << Settings::GetInt("core.threads") << std::endl << "Set the threads for XDG to use." << std::endl << std::endl << "\t~> ";
                int input = atoi(Utils::GetInput().c_str());
                if (input > 0 && input <= 500) {
                    printf("i: %d\r\n", input);
                    Settings::Set("core.threads", input);
                    Sleep(1000);
                    break;
                }
                else {
                    Utils::logError("Please enter a value between 1 and 500.");
                    Sleep(500);
                }
            }
            break;
        case 1:
            while (true) {
                system("cls");
                PrintBanner();
                std::cout << "Current Timeouts: " << Settings::GetInt("core.timeouts") << std::endl << "Set timeouts for HTTP requests." << std::endl << std::endl << "\t~> ";
                int input = atoi(Utils::GetInput().c_str());
                if (input > 4 && input <= 20) {
                    Settings::Set("core.timeouts", input);
                    break;
                }
                else {
                    Utils::logError("Please enter a value between 5 and 20.");
                    Sleep(500);
                }
            }
            break;
        case 2:
            while (true) {
                system("cls");
                PrintBanner();
                std::cout << "Current Pages: " << Settings::GetInt("scraper.pages") << std::endl << "Pages to scrape on dorkers." << std::endl << std::endl << "\t~> ";
                int input = atoi(Utils::GetInput().c_str());
                if (input > 0 && input <= 20) {
                    Settings::Set("scraper.pages", input);
                    break;
                }
                else {
                    Utils::logError("Please enter a value between 1 and 20.");
                    Sleep(500);
                }
            }
            break;
        case 3:
            //Settings::Set("autothreads", !Settings::GetBool("autothreads"));
            break;
        case 4:
            Settings::Set("dumper.targeted", !Settings::GetBool("dumper.targeted"));
            break;
        case 5:
            Settings::Set("core.batch", !Settings::GetBool("core.batch"));
            break;
        case 6:
            switch (Settings::GetInt("exploiter.intensity"))
            {
            case 0:
                Settings::Set("exploiter.intensity", 1);
                break;
            case 1:
                Settings::Set("exploiter.intensity", 2);
                break;
            case 2:
                Settings::Set("exploiter.intensity", 0);
                break;
            }
            break;
        case 7:
            ShowTechniques();
            break;
        case 8:
            ShowDorkers();
            break;
        case 99:
            /*if (QuartzAuth::_username.empty()) {
                int i = 0;
                i = i/i;
            }*/
            return;
        }
    }
}

void ShowUtils() {
    while (true) {
        system("cls");
        PrintBanner();

        std::cout << std::endl << "\t[0] Url Cleaner" << std::endl;

        printf("\r\n\t[99] <- Back\r\n");

        printf("\r\n\t~> ");

        switch(atoi(Utils::GetInput().c_str())) {
            case 0:
                Modules::Cleaner::GetInstance()->run();
                break;
            case 99:
                return;
            default:
                Utils::logError("Invalid option");
                Sleep(250);
        }
    }
}

void PrintBanner() {
    SetConsoleTextAttribute(Utils::GetConsoleHandle(), Utils::LIGHT_PURPLE);
    std::wcout << "\t\t  db    db d8888b.  d888b       db    db .d888b. " << std::endl;
    std::wcout << "\t\t  `8b  d8' 88  `8D 88' Y8b      88    88 VP  `8D " << std::endl;
    std::wcout << "\t\t   `8bd8'  88   88 88           Y8    8P    odD' " << std::endl;
    std::wcout << "\t\t   .dPYb.  88   88 88  ooo      `8b  d8'  .88'   " << std::endl;
    std::wcout << "\t\t  .8P  Y8. 88  .8D 88.  8~       `8bd8'  j88.    " << std::endl;
    std::wcout << "\t\t  YP    YP Y8888D'  Y888P          YP    888888D " << std::endl << std::endl;
    SetConsoleTextAttribute(Utils::GetConsoleHandle(), Utils::LIGHT_GRAY);
}