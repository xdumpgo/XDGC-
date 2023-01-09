#include "Injectable.h"
#include <utils/Utils.h>

using namespace Injection;

Poco::URI Injectable::GetUrl() {
    return this->_url;
}

Injectable* Injectable::ParseString(const std::string& formatted) {
    auto parts = Utils::explode(formatted, ' ');

    return new Injectable(parts[0], atoi(parts[1].c_str()), (InjectionType)atoi(parts[2].c_str()), (DBType)atoi(parts[3].c_str()), atoi(parts[4].c_str()), atoi(parts[5].c_str()));;
}

std::string Injectable::ToString() const {
    //         url param injT dbT vec mod
    return Utils::format("%s %d %d %d %d %d", this->_url.toString().c_str(), this->_param, this->_itype, this->_dtype, this->_vec, this->_mod);
}

Injectable::Injectable(const std::string& url, int param, InjectionType iType, DBType dType, int vec, int mod)
: Injectable(Poco::URI(url), param, iType, dType, vec, mod) {}

Injectable::Injectable(const Poco::URI &uri, int param, InjectionType iType, DBType dType, int vec, int mod) {
    this->_url = uri;
    this->_itype = iType;
    this->_dtype = dType;
    this->_vec = vec;
    this->_mod = mod;
    this->_param = param;
}

InjectionType Injectable::GetType() {
    return this->_itype;
}

DBType Injectable::GetDBType() {
    return this->_dtype;
}

int Injectable::GetVectorIndex() {
    return this->_vec;
}

int Injectable::GetModIndex() {
    return this->_mod;
}

int Injectable::GetParamIndex() {
    return this->_param;
}
