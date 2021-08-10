#include "../headers/DatabaseManager.h"

DatabaseManager::DatabaseManager() {
    this->db_name = "";
    this->path = "db/";
    this->logfile = ".empdblog.dat";
    this->connected = false;
    this->initLogMap();
}

DatabaseManager::~DatabaseManager() {

}

//// GETTERS ////

// get the relative path of the database
std::string DatabaseManager::getDB_NAME_WITH_PATH() {
    return this->path + this->db_name;
}

// get the physical name of the database, NOT the relative path
std::string DatabaseManager::getDB_NAME_NO_PATH() {
    return this->db_name;
}

//// CDC FUNCTIONS ////

// create a database. If a valid name is input into the system,
// a database file will be created in the db/ directory and
// used for initialization later (in the connect() method)
int DatabaseManager::createdb(std::string db_name) {
    std::string input;
    // check for invalid characters
    if(findInvalid(db_name))
        return -1;
    else if(db_name.length()>=50)
        return -2;
    else {
        // confirm file does not already exist
        std::fstream exists(this->path+db_name, std::ios::in | std::ios::binary);
        if(exists.is_open()) {
            exists.close();
            return -3;
        }
        else if(this->find(this->path+db_name) != this->end()) {
            exists.close();
            return -3;
        }
        else {
            exists.close();
            this->appendToLog(this->path+db_name);
            return 1; // file does not exist and has valid characters, database can be created by EMap::load()
        }
    }
}

// delete a database by going to its location in the file and changing all present
// characters to empty strings (essentially removing the line from the file)
// then remove the file entirely from the file system using the remove() function
int DatabaseManager::deletedb(std::string db_name) {
    if(!this->checkLogStatus())
        return -1;

    // no databases in log file
    if(this->size() == 0)
        return -2;

    if(findInvalid(db_name))
        return -3;

    DatabaseManager::iterator it = this->find(this->path+db_name);
    if(it == this->end())
        return -4; // database could not be found
    
    // otherwise, remove database from log file and from filesystem
    std::fstream log(this->path + this->logfile, std::ios::in | std::ios::out | std::ios::binary);
    
    long offset = it->second;
    log.seekp(offset); // go to the offset of the found record
    char empty[54];
    empty[0] = '!'; empty[1] = '\0'; // store the bang
    log.write((char*)&empty, sizeof(empty)); // free up data
    std::remove((this->path+db_name).c_str()); // remove file from system
    
    // adjust map
    this->erase(it);
    this->insert(std::pair<std::string, long>("!", offset));

    log.close();
    return 1;
}

// connect to a database
int DatabaseManager::connectdb(std::string db_name) {
    if(!this->checkLogStatus())
        return -1;

    // no databases in log file
    if(this->size() == 0)
        return -2;

    DatabaseManager::iterator find = this->find(this->path+db_name);
    if(find == this->end())
        return -3;
    // finally, initialize a db by returning value
    this->db_name = db_name;
    this->verifyDatabaseIntegrity(db_name); // guarantee that file is not corrupted/deleted
    return true;
}
//// END OF CDC FUNCTIONS ////

//// UTILITIES ////

// check the status of the log file to ensure it exists
void DatabaseManager::initLogMap() {
    this->checkLogStatusSilent();
    std::fstream log(this->path+this->logfile, std::ios::in | std::ios::binary);

    // gather all available databases into map
    long offset = log.tellg(); char read[54];
    while(log.read((char*)&read, sizeof(read))) {
        this->insert(std::pair<std::string, long>(std::string(read), offset));
        offset = log.tellg();
    }
    log.close();
}

// check the status of the log file (actively)
bool DatabaseManager::checkLogStatus() {
    std::fstream log(this->path + this->logfile, std::ios::in | std::ios::binary);
    if(!log.is_open()) {
        log.close();
        log.open(this->path + this->logfile, std::ios::out | std::ios::binary);
        log.close(); // log file created
        return false;
    }
    log.close();
    return true;
}

// silently check the status of the log file to ensure it exists
bool DatabaseManager::checkLogStatusSilent() {
    std::fstream log(this->path + this->logfile, std::ios::in | std::ios::binary);
    if(!log.is_open()) {
        log.close();
        log.open(this->path + this->logfile, std::ios::out | std::ios::binary);
        log.close(); // log file created
        return false;
    }
    log.close();
    return true;
}

// make sure that the database binary exists
bool DatabaseManager::verifyDatabaseIntegrity(std::string db_name) {
    std::fstream check(this->path+db_name, std::ios::in);
    if(!check.is_open()) {
        check.close();
        std::cout<<"\nWARN: Database file was corrupted and/or deleted. Creating database file now..."<<std::endl<<std::endl;
        check.open(this->path+db_name, std::ios::out);
        check.close(); // log file created
        return false;
    }
    check.close();
    return true;
}

// append a new database name to the log file
void DatabaseManager::appendToLog(std::string newdb_name) {
    this->checkLogStatus(); // create log if log file not already created
    std::fstream log(this->path + this->logfile, std::ios::in | std::ios::out | std::ios::binary); // open in in/out to avoid truncating data!!!
    
    // look for empty database slots to overwrite first
    DatabaseManager::iterator it = this->find("!");
    if(it != this->end()) {
        log.seekp(it->second);
        this->erase(it); // remove banged record from map
    }
    else
        log.seekp(0, std::ios::end);

    this->insert(std::pair<std::string, long>(newdb_name, log.tellp())); // keep track of new data in map
    
    // write out binary data to log
    char send[54]; std::strncpy(send, (newdb_name).c_str(), 53);
    send[53] = '\0';
    log.write((char*)&send, sizeof(send));
    log.close();
}

// check if a string has non A-Z a-z characters (for creating databases)
bool DatabaseManager::findInvalid(std::string input) {
    for(char c : input) {
        if(!((c >= 'A' && c <= 'Z')||(c >= 'a' && c <= 'z')))
            return true;
    }
    return false;
}

// list all databases currently in the map
void DatabaseManager::listDatabases() {
    std::cout<<"---- DATABASES ----"<<std::endl;

    // only write out databases not yet deleted (no bangs)
    for(std::map<std::string, long>::iterator it = this->begin(); it != this->end(); ++it)
            std::cout<<((it->first)!="!"?"- "+it->first+"\n":"");
}
