#pragma once
#include<stdlib.h>
#include<map>
#include<string>
#include<iostream>
#include<fstream>
#include<cstring>

// Manage all things related to the db/ directory. Make sure that all necessary files are present,
// ensure that new database entries are logged properly, and store all relevant data into a map
// that can be used to perform operations later (such as delete, connect, confirm no duplicates, etc.)
//
// Uses a multimap to allow multiple values for the bang key (!) which designate deleted databases that
// are free to have their names overwritten
class DatabaseManager : std::multimap<std::string, long> {
    public:
        // constructors & destructor //
        DatabaseManager();
        ~DatabaseManager();

        // CDC (create, delete, connect)
        int  createdb(std::string db_name); // returns 1 if a db is successfully created
        int  deletedb(std::string db_name); // returns 1 if a db is successfully deleted
        int connectdb(std::string db_name); // returns 1 if successfully connected

        // utilities
        bool checkLogStatus();
        bool checkLogStatusSilent();
        void initLogMap();
        void appendToLog(std::string newdb_name);
        bool findInvalid(std::string input);
        void listDatabases();
        bool verifyDatabaseIntegrity(std::string db_name);

        // getters
        std::string getDB_NAME_WITH_PATH();
        std::string getDB_NAME_NO_PATH();
        bool getConnectionStatus();

        // setters
        void setDB_NAME(std::string db_name);
        void setConnectionStatus(bool connected);

    private:
        std::string db_name; // the currently selected database
        std::string path; // the path to the selected database
        std::string logfile; // the log file that keeps track of all databases
        bool connected; // defaults to false
};