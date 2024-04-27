#include <iostream>
#include <sqlite3.h>

int main()
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("./database.db", &db);

    if (rc)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return (0);
    }
    else
    {
        std::cout << "Opened database successfully" << std::endl;
    }

    const char *sql = "SELECT * FROM sensor_data";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    std::cout << "ID | TIMESTAMP | TEMPERATURE | HUMIDITY | PRESSURE | AIR_QUALITY | LASER_DISTANCE | ULTRASONIC_DISTANCE | LIGHTNESS" << std::endl;
    std::cout << "----------------------------------------------------------------------------------------" << std::endl;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *timestamp = sqlite3_column_text(stmt, 1);
        double temperature = sqlite3_column_double(stmt, 2);
        double humidity = sqlite3_column_double(stmt, 3);
        double pressure = sqlite3_column_double(stmt, 4);
        double air_quality = sqlite3_column_double(stmt, 5);
        double laser_distance = sqlite3_column_double(stmt, 6);
        double ultrasonic_distance = sqlite3_column_double(stmt, 7);
        double lightness = sqlite3_column_double(stmt, 8);

        std::cout << id << " | " << timestamp << " | " << temperature << " | " << humidity << " | " << pressure << " | "
                  << air_quality << " | " << laser_distance << " | " << ultrasonic_distance << " | " << lightness << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
