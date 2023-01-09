#ifndef __QALib_API
#define __QALib_API
#include <string>
#include <utility>

struct Transaction {
    std::string Status;
    std::string Message;
    std::string Username;
    std::string Email;
    std::string Password;
    std::string Expiry;
    std::string HWID;
    std::string Data;
    std::string Session;
    std::string Salt;
    std::string PacketType;
    std::string Key;
    std::string VKey;
    std::string Hash;
    std::string Version;

    std::string Serialize() const;
    static Transaction Deserialize(std::string string);
};

class QuartzAuth {
private:
    static std::string _username;
    static std::string _password;
    static std::string _expiry;
    static std::string _version;
    static std::string _key;
    static std::string _vkey;
public:
    enum Result {
        OK,
        INVALID_CREDENTIALS,
        INVALID_HWID,
        INVALID_TOKEN,
        CONNECTION_FAILED,
        UPDATE_AVAILABLE,
        UNKNOWN_ERROR,
    };

    static void Init(std::string key, std::string vkey, std::string version) {
        _key = std::move(key);
        _vkey = std::move(vkey);
        _version = std::move(version);
    }
    static Result Heartbeat();
    static Result Authenticate(const std::string& username, const std::string& password);
    static Result Register(const std::string& username, const std::string& password, const std::string& email, const std::string& token);
    static Result Renew(const std::string& username, const std::string& password, const std::string& token);
    static std::string Var(const std::string& name);
    static Result GetAuth();

    static std::string GetUsername();
    static std::string GetExpiryStr();

    static inline Result DoTransaction(Transaction &trans);
};

#endif