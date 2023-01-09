//
// Created by zerte on 4/15/2020.
//

#include <utils/Utils.h>
#include <utils/hwid.h>

#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/CipherKey.h>
#include <Poco/Crypto/Cipher.h>

#include <rapidjson/prettywriter.h> // for stringify JSON
#include <rapidjson/document.h>

#include <shellapi.h>
#include <WinSock2.h>

#include "quartzauth.h"

#define DEFAULT_BUFLEN 512

std::string QuartzAuth::_username;
std::string QuartzAuth::_password;
std::string QuartzAuth::_expiry;
std::string QuartzAuth::_key;
std::string QuartzAuth::_vkey;
std::string QuartzAuth::_version;


QuartzAuth::Result QuartzAuth::DoTransaction(Transaction &trans) {
    trans.Session = Utils::gen_random(30);
    trans.Salt = Utils::gen_random(20);
    trans.HWID = Fingerprint();
    trans.Key = _key;
    trans.VKey = _vkey;
    trans.Version = _version;
    trans.Hash = "";

    Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
    Poco::Crypto::CipherKey AesKey("AES-256-CBC", "ZM7!JYGLSC8u4MA$A+*?vUgY7#E2$ZsK", "", 1);
    Poco::Crypto::Cipher::Ptr pCipherAES256 = factory.createCipher(AesKey);
    //return (text, Poco::Crypto::Cipher::ENC_BASE64);

    auto serialized = trans.Serialize();
    std::string payload = pCipherAES256->encryptString(serialized, Poco::Crypto::Cipher::ENC_BASE64_NO_LF);

    WSADATA              wsaData;
    SOCKET               SendingSocket;
    // Server/receiver address
    SOCKADDR_IN          ServerAddr, ThisSenderInfo;
    // Server/receiver port to connect to
    unsigned int         Port = 3331;
    int  RetCode;
    int BytesSent, nlen;
    char recvbuf[DEFAULT_BUFLEN] = {0};

    // Initialize Winsock version 2.2
    WSAStartup(MAKEWORD(2,2), &wsaData);

    // Create a new socket to make a client connection.
    // AF_INET = 2, The Internet Protocol version 4 (IPv4) address family, TCP protocol
    SendingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(SendingSocket == INVALID_SOCKET)
    {
        // Do the clean up
        WSACleanup();
        // Exit with error
        return CONNECTION_FAILED;
    }

    // Set up a SOCKADDR_IN structure that will be used to connect
    // to a listening server on port 5150. For demonstration
    // purposes, let's assume our server's IP address is 127.0.0.1 or localhost

    // IPv4
    ServerAddr.sin_family = AF_INET;
    // Port no.
    ServerAddr.sin_port = htons(Port);
    // The IP address
    ServerAddr.sin_addr.s_addr = inet_addr("37.228.132.179");

    // Make a connection to the server with socket SendingSocket.
    RetCode = connect(SendingSocket, (SOCKADDR *) &ServerAddr, sizeof(ServerAddr));
    if(RetCode != 0)
    {
        Utils::logError("Failed to connect to licensing server.");
        // Close the socket
        closesocket(SendingSocket);
        // Do the clean up
        WSACleanup();
        // Exit with error
        return CONNECTION_FAILED;
    }

    // At this point you can start sending or receiving data on
    // the socket SendingSocket.

    // Some info on the receiver side...
    getsockname(SendingSocket, (SOCKADDR *)&ServerAddr, (int *)sizeof(ServerAddr));

    // Sends some data to server/receiver...
    BytesSent = send(SendingSocket, payload.c_str(), strlen(payload.c_str()), 0);

    if(BytesSent == SOCKET_ERROR) {
        closesocket(SendingSocket);
        WSACleanup();
        return CONNECTION_FAILED;
    }
    else
    {
        // Some info on this sender side...
        // Allocate the required resources
        memset(&ThisSenderInfo, 0, sizeof(ThisSenderInfo));
        nlen = sizeof(ThisSenderInfo);

        getsockname(SendingSocket, (SOCKADDR *)&ThisSenderInfo, &nlen);
    }

    if( shutdown(SendingSocket, SD_SEND) != 0) {
        closesocket(SendingSocket);
        WSACleanup();
        return CONNECTION_FAILED;
    }

    do {
        RetCode = recv(SendingSocket, recvbuf, DEFAULT_BUFLEN, 0);
    } while (RetCode > 0);

    if(closesocket(SendingSocket) != 0)
        return CONNECTION_FAILED;

    // When your application is finished handling the connection, call WSACleanup.
    if(WSACleanup() != 0)
        return CONNECTION_FAILED;

    auto decrypted = pCipherAES256->decryptString(recvbuf, Poco::Crypto::Cipher::ENC_BASE64_NO_LF);
    trans = Transaction::Deserialize(decrypted);
    if (trans.Status == "success") {
        return OK;
    } else {
        if (trans.Message == "update_available") {
            ShellExecute(nullptr, nullptr, reinterpret_cast<LPCSTR>(trans.Data.c_str()), nullptr, nullptr , SW_SHOW );
            return UPDATE_AVAILABLE;
        }
    }
    return UNKNOWN_ERROR;
}

QuartzAuth::Result QuartzAuth::GetAuth() {
    switch (Heartbeat()) {
        case OK:
            break;
        case UPDATE_AVAILABLE:
            return UPDATE_AVAILABLE;
        case UNKNOWN_ERROR:
            break;
    }
    std::string token;
    std::string email;
    while(true) {
        system("cls");
        Utils::logInfo("Welcome, please authenticate.");
        printf("\t[0] Login\r\n");
        printf("\t[1] Register\r\n");
        printf("\t[2] Redeem\r\n");

        switch(atoi(Utils::GetInput().c_str())) {
            case 0:
                system("cls");
                printf("\tUsername: ");
                _username = Utils::GetInput();
                printf("\tPassword: ");
                _password = Utils::GetInput();


                switch (QuartzAuth::Authenticate(_username, _password)) {
                    case QuartzAuth::OK:
                        return QuartzAuth::OK;
                    case INVALID_CREDENTIALS:
                        Utils::logError("Invalid Username / Password");
                        break;
                    case INVALID_HWID:
                        Utils::logError("Invalid HWID");
                        break;
                    case CONNECTION_FAILED:
                        break;
                    default:
                        Utils::logError("Unknown Error");
                }

                break;
            case 1:
                system("cls");
                printf("\tUsername: ");
                _username = Utils::GetInput();
                printf("\tPassword: ");
                _password = Utils::GetInput();
                printf("\tEmail: ");
                email = Utils::GetInput();
                printf("\tToken: ");
                token = Utils::GetInput();

                switch (QuartzAuth::Register(_username, _password, email, token)) {
                    case QuartzAuth::OK:
                        return QuartzAuth::OK;
                    case INVALID_TOKEN:
                        Utils::logError("Invalid Token");
                        break;
                    case CONNECTION_FAILED:
                        Utils::logError("Failed to connect to licensing server.");
                        break;
                    default:
                        Utils::logError("Unknown Error");
                }

                break;
            case 2:
                while(true) {
                    system("cls");
                    printf("\tUsername: ");
                    _username = Utils::GetInput();
                    printf("\tPassword: ");
                    _password = Utils::GetInput();
                    printf("\tToken: ");
                    token = Utils::GetInput();

                    int ret = QuartzAuth::Renew(_username, _password, token);
                    if (ret == QuartzAuth::OK) {
                        break;
                    } else if (ret == QuartzAuth::CONNECTION_FAILED) {
                        Utils::logError("Failed to connect to licensing server.");
                    } else if (ret == QuartzAuth::INVALID_CREDENTIALS) {
                        Utils::logError("Invalid Username / Password.");
                    } else if (ret == QuartzAuth::INVALID_TOKEN) {
                        Utils::logError("Invalid HWID.");
                    } else {
                        Utils::logError("Unknown Error.");
                    }
                    Sleep(500);
                }
                break;
            default:
                Utils::logError("Invalid selection");
        }
    Sleep(500);
    }
}

QuartzAuth::Result QuartzAuth::Authenticate(const std::string& username, const std::string& password) {
    Transaction payload{};
    payload.Username = username;
    payload.Password = password;
    payload.PacketType = "authenticate";

    QuartzAuth::Result ret = DoTransaction(payload);
    if (ret != OK) {
        return ret;
    }

    if (payload.Status == "success") {
        QuartzAuth::_username = username;
        QuartzAuth::_password = password;
        QuartzAuth::_expiry = payload.Expiry;
        return OK;
    }
    return INVALID_CREDENTIALS;
}

QuartzAuth::Result QuartzAuth::Register(const std::string& username, const std::string& password, const std::string& email, const std::string& token) {
    Transaction payload{};
    payload.Username = username;
    payload.Password = password;
    payload.Email = email;
    payload.Data = token;
    payload.PacketType = "register";

    QuartzAuth::Result ret = DoTransaction(payload);
    if (ret != OK) {
        return ret;
    }

    if (std::string(payload.Status) == "success") {
        QuartzAuth::_username = username;
        QuartzAuth::_password = password;
        QuartzAuth::_expiry = payload.Expiry;
        return OK;
    }
    return INVALID_CREDENTIALS;
}

QuartzAuth::Result QuartzAuth::Renew(const std::string& username, const std::string& password, const std::string& token) {
    Transaction payload{};
    payload.Username = username;
    payload.Password = password;
    payload.Data = token;
    payload.PacketType = "redeem";

    QuartzAuth::Result ret = DoTransaction(payload);
    if (ret != OK) {
        return ret;
    }

    if (std::string(payload.Status) == "success") {
        QuartzAuth::_username = username;
        QuartzAuth::_password = password;
        QuartzAuth::_expiry = payload.Expiry;
        return OK;
    }
    return INVALID_CREDENTIALS;
}

QuartzAuth::Result QuartzAuth::Heartbeat() {
    auto trans = Transaction();
    trans.PacketType = "heartbeat";

    return DoTransaction(trans);
}

std::string QuartzAuth::GetUsername() {
    return QuartzAuth::_username;
}

std::string QuartzAuth::GetExpiryStr() {
    return QuartzAuth::_expiry;
}

std::string Transaction::Serialize() const {
    rapidjson::StringBuffer s;

    rapidjson::Writer<rapidjson::StringBuffer> writer(s);

    writer.StartObject();
    writer.Key("key");
    writer.String(this->Key.c_str());
    writer.Key("vkey");
    writer.String(this->VKey.c_str());
    writer.Key("hwid");
    writer.String(Fingerprint().c_str());
    if (this->Username.length() > 0) {
        writer.Key("username");
        writer.String(this->Username.c_str());
        writer.Key("password");
        writer.String(this->Password.c_str());
    }
    if (this->Email.length() > 0) {
        writer.Key("email");
        writer.String(this->Email.c_str());
    }
    if (this->Data.length() > 0) {
        writer.Key("data");
        writer.String(this->Data.c_str());
    }
    writer.Key("version");
    writer.String(this->Version.c_str());
    writer.Key("packettype");
    writer.String(this->PacketType.c_str());
    writer.Key("session_id");
    writer.String(this->Session.c_str());
    writer.Key("salt");
    writer.String(this->Salt.c_str());

    writer.EndObject();

    return std::string(s.GetString());
}

Transaction Transaction::Deserialize(std::string s) {
    rapidjson::Document doc;
    doc.Parse(s.c_str());
    assert(doc.HasMember("status"));

    Transaction trans{};
    /*trans.Status = pCipherAES256->decryptString(doc["status"].GetString(), Poco::Crypto::Cipher::ENC_BASE64_NO_LF);
    trans.Expiry = pCipherAES256->decryptString(doc["expiry"].GetString(), Poco::Crypto::Cipher::ENC_BASE64_NO_LF);*/
    trans.Status = doc["status"].GetString();
    trans.Expiry = doc["expiry"].GetString();
    trans.Data = doc["data"].GetString();
    trans.Message = doc["message"].GetString();

    return trans;
}