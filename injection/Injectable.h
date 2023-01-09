#ifndef XDG_INJECTION_
#define XDG_INJECTION_

#include <string>
#include <Poco/URI.h>
#include "Injection.h"

namespace Injection {
    class Injectable {
    private:
        Poco::URI _url;
        InjectionType _itype;
        DBType _dtype;
        int _vec;
        int _mod;
        int _param;
    public:
        explicit Injectable(const std::string &url, int param = 0, InjectionType iType = IERR, DBType dType = MYSQL, int vec=0, int mod=0);
        explicit Injectable(const Poco::URI &uri, int param = 0, InjectionType iType = IERR, DBType dType = MYSQL, int vec=0, int mod=0);

        Poco::URI GetUrl();
        InjectionType GetType();
        DBType GetDBType();
        int GetVectorIndex();
        int GetModIndex();
        int GetParamIndex();

        static Injectable* ParseString(const std::string &formatted);
        std::string ToString() const;
    };
}

#endif