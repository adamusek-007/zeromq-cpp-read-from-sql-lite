#include <iostream>
#include <sqlite3.h>
#include <zmq.hpp>
#include "readAndSend.h"
using namespace std;
class RawDataSender
{
public:
    RawDataSender()
    {
        cout << "Main object created successfully" << endl;
        main();
    }

private:
    const string address = "tcp://localhost:5555";
    const char *databaseFileName = "database.db";
    const char *sql = "SELECT * FROM sensor_data";

    string getMessage(sqlite3_stmt *stmt)
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

        std::ostringstream oss;
        oss << id << "," << timestamp << "," << temperature << "," << humidity << ","
            << pressure << "," << air_quality << "," << laser_distance << ","
            << ultrasonic_distance << "," << lightness;
        std::string message = oss.str();
        return message;

    }
    int main()
    {
        sqlite3 *db;
        char *zErrMsg = 0;
        int dbDriver;

        zmq::context_t context(1);
        zmq::socket_t socket(context, ZMQ_PUSH);
        socket.connect(address);

        dbDriver = sqlite3_open(databaseFileName, &db);

        bool retFlag;
        int retVal = checkDbDriver(dbDriver, db, retFlag);
        if (retFlag)
            return retVal;

        sqlite3_stmt *stmt;

        dbDriver = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

        if (dbDriver != SQLITE_OK)
        {
            std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
            return 1;
        }
        int counter = 0;

        while ((dbDriver = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            counter++;
            string message = getMessage(stmt);
            std::cout << counter << " Mess: " << message.c_str() << " Size: " << message.size() << std::endl;
            zmq::message_t zmq_message(message.size());
            memcpy(zmq_message.data(), message.c_str(), message.size());
            socket.send(zmq_message, zmq::send_flags::none);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        return 0;
    }
    int checkDbDriver(int dbDriver, sqlite3 *db, bool &retFlag)
    {
        if (dbDriver)
        {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return (0);
        }
        else
        {
            std::cout << "Opened database successfully" << std::endl;
            return (1);
        }
        return {};
    }
};
int main()
{
    RawDataSender main;
    return 0;
}
