#include<iostream>
#include<fstream>
#include "./headers/SalariedEmployee.h"
#include "./headers/WagedEmployee.h"
#include "./headers/EMap.h"
#include "./headers/DatabaseManager.h"

// menus
void dbSelect(std::string path = "", std::string log = "");
void dbMenu(EMap& em, std::string db_name);

// db select menu switch functions
char dbSelectOption();
bool createdb(DatabaseManager& dbm);
void listdbs(DatabaseManager& dbm);
bool deletedb(DatabaseManager& dbm);
std::string connectdb(DatabaseManager& dbm);

// main menu switch functions
char dbMenuOptions(std::string& db_name);
void createEmployee(EMap& em);
void listEmployees(EMap& em);
void deleteEmployee(EMap& em);
void importEmployees(EMap& em);
void exportEmployees(EMap& em);


// utils
int checkArgs(int argc, char** argv, std::string& ddrp, std::string& log);
void displayManual();
std::string smartCenterHeader(std::string text, int width);
bool findInvalid(std::string input);
bool verifyLogIntegrity();
void appendToLog(std::string newdb_name);
FilterType getFilterType();
std::string getFilterCMP(FilterType ftype);

int main(int argc, char** argv) {
    std::string ddrp = "", log = "";
    int code = checkArgs(argc, argv, ddrp, log);
    if(code == -1) return 1; // argument misuse
    else if(code == -2) return 0; // help menu

    // main function to select, delete, and/or add databases. all database functions are run within this method.
    else if(code == 1) dbSelect(); // no arguments defined
    else if(code == 2) dbSelect(ddrp, ""); // ddrp defined
    else if(code == 3) dbSelect("", log); // log defined
    else if(code == 4) dbSelect(ddrp, log); // ddrp and log defined 
    return 0;
}

// check all inputted arguments to determine if the user wishes to define a ddrp/log file
int checkArgs(int argc, char** argv, std::string& ddrp, std::string& log) {
    if(argc == 1) {
        return 1; // basic confirm
    }
    for(int i=1; i<argc; i++) {
        if(std::strcmp(argv[i], "--help") == 0) {
            std::cout<<std::endl;
            displayManual();
            return -2; // manual code
        }
        else if(std::strcmp(argv[i], "-d") == 0 || std::strcmp(argv[i], "--ddrp") == 0) {
            // make sure there is no flag misuse
            if(i == argc-1) {
                std::cerr<<"\nEmployeeDB: Incorrect use of arguments, you must define a file after a flag!"<<std::endl;
                std::cout<<"EmployeeDB: usage: ./EmployeeDB.exe -d PATH ..."<<std::endl<<std::endl;
                return -1;
            }
            else if(std::strcmp(argv[i+1], "-l") == 0 || std::strcmp(argv[i+1], "--log") == 0) {
                std::cerr<<"\nEmployeeDB: Incorrect use of arguments, you cannot have two arguments next to each other!"<<std::endl;
                std::cout<<"EmployeeDB: usage: ./EmployeeDB.exe -d PATH -l FILE"<<std::endl<<std::endl;
                return -1;
            }
            else {
                ddrp = argv[i+1];
                if(ddrp[ddrp.size()-1] != '/') ddrp+="/";
                i++;
            }
        }
        else if(std::strcmp(argv[i], "-l") == 0 || std::strcmp(argv[i], "--log") == 0) {
            // make sure there is no flag misuse
            if(i == argc-1) {
                std::cerr<<"\nEmployeeDB: Incorrect use of arguments, you must define a file after a flag!"<<std::endl;
                std::cout<<"EmployeeDB: usage: ./EmployeeDB.exe -l FILE ..."<<std::endl<<std::endl;
                return -1;
            }
            else if(std::strcmp(argv[i+1], "-d") == 0 || std::strcmp(argv[i+1], "--ddrp") == 0) {
                std::cerr<<"\nEmployeeDB: Incorrect use of arguments, you cannot have two arguments next to each other!"<<std::endl;
                std::cout<<"EmployeeDB: usage: ./EmployeeDB.exe -d PATH -l FILE"<<std::endl<<std::endl;
                return -1;
            }
            else {
                log = argv[i+1];
                i++;
            }
        }
        else {
            std::cerr<<"\nEmployeeDB: Incorrect use of arguments. Use ./EmployeeDB.exe --help for more information."<<std::endl<<std::endl;
            return -1; // flag error code
        }
    }
    if(ddrp == "" && log == "") {
        return 1; // basic confirm
    }
    else if(ddrp != "" && log == "") {
        return 2; // ddrp defined, log default
    }
    else if(ddrp == "" && log != "") {
        return 3; // ddrp default, log defined
    }
    else if(ddrp != "" && log != "") {
        return 4; // ddrp defined, log defined
    }
    else
        return -404; // remove compiler warning message
}

// display the "man" page of the program (./man)
void displayManual() {
    std::fstream man("man", std::ios::in);
    std::string line;
    while(getline(man, line))
        std::cout<<line<<std::endl;
    man.close();
}

//// DATABASE SELECTOR FUNCTIONS ////

// highest order menu for selecting databases
void dbSelect(std::string path, std::string log) {
    DatabaseManager* dbm;
    if(path == "" && log == "")
        dbm = new DatabaseManager;
    else
        dbm = new DatabaseManager(path, log); 
    char option;
    do {
        option = dbSelectOption();
        std::string connected = "";
        switch(option) {
            case 'a': createdb(*dbm); break;
            case 'b': listdbs(*dbm); break;
            case 'c': connected = connectdb(*dbm); break;
            case 'd': deletedb(*dbm); break;
            case 'x': break;
            default:
                std::cerr<<"Invalid command. Please try again."<<std::endl<<std::endl;
                break;
        }
        if(connected != "") {
            // database locked on, start db menu
            EMap em; em.setFileName(dbm->getDB_NAME_WITH_PATH()); em.load();
            dbMenu(em, dbm->getDB_NAME_NO_PATH());
        }
    } while(option != 'x');
}

char dbSelectOption() {
    std::cout<<smartCenterHeader("DATABASE MANAGEMENT SYSTEM", 42);
    std::cout<<"* a. Create New Database                 *\n";
    std::cout<<"* b. List Current Databases              *\n";
    std::cout<<"* c. Connect To A Database               *\n";
    std::cout<<"* d. Delete A Database                   *\n";
    std::cout<<"* x. Exit                                *\n";
    std::cout<<"******************************************\n\n";
    std::cout<<"Enter an option: ";
    std::string input; getline(std::cin, input);
    return input[0];
}

// create a database. If a valid name is input into the system,
// a database file will be created in the db/ directory and
// used for initialization later (in the connect() method)
bool createdb(DatabaseManager& dbm) {
    std::string input;
    while(true) {
        std::cout<<"-- Database Creation --\n";
        std::cout<<"-  1. Spaces and special characters are not allowed.\n";
        std::cout<<"-    1a. Allowed special characters: -, _, ., numbers 1-9\n";
        std::cout<<"-  2. Must be under 50 characters.\n";
        std::cout<<"-  3. Cannot have a duplicate database name.\n";
        std::cout<<"-- Enter your new database name (Enter -1 to Exit): ";
        getline(std::cin, input);
        if(input == "") {
            std::cout<<"\nERR: You cannot create a file with an empty name! Please input a valid database name."<<std::endl<<std::endl;
            continue;
        }
        if(input.substr(0, 2) == "-1")
            break;
        else {
            int code = dbm.createdb(input);
            if(code == 1)
                return true;
            else if(code == -1) {
                std::cout<<"\nERR: Your database name has invalid characters. Please only use A-Z, a-z, and NO SPACES."<<std::endl<<std::endl;
                continue;
            }
            else if(code == -2) {
                std::cout<<"\nERR: Your database name is over 50 characters long! Please limit the name to 50 characters or less."<<std::endl<<std::endl;
                continue;
            }
            else if(code == -3) {
                std::cout<<"\nERR: A database with that name already exists. Please enter a different name."<<std::endl<<std::endl;
                continue;
            }
        }
    }
    return false;
}

void listdbs(DatabaseManager& dbm) {
    dbm.listDatabases();
}

bool deletedb(DatabaseManager& dbm) {
    while(true) {
        std::string input;
        dbm.listDatabases();
        std::cout<<"- Enter a database name to delete (Enter -1 to Exit): ";
        getline(std::cin, input);
        if(input.substr(0, 2) == "-1")
            break;
        else if(input == "") {
            std::cout<<"\nERR: You cannot delete a database with no name! Please enter an existing database."<<std::endl<<std::endl;
            continue;
        }
        else {
            int code= dbm.deletedb(input);
            if(code == 1)
                return true;
            else if(code == -1) {
                std::cout<<"\nOK: Log file created. No databases to initialize. Exiting initialization section..."<<std::endl<<std::endl;
                continue;
            }
            else if(code == -2) {
                std::cout<<"\nWARN: Log file empty. No database has been initialized. Exiting initialization section..."<<std::endl<<std::endl;
                continue;
            }
            else if(code == -3) {
                std::cout<<"\nERR: That database name has invalid characters. Please only use A-Z, a-z, and NO SPACES."<<std::endl<<std::endl;
                continue;
            }
            else if(code == -4) {
                std::cout<<"\nERR: That database could not be found. Please input an existing database to delete."<<std::endl<<std::endl;
                continue;
            }
        }
    }
    return false;
}

// connect to a database
std::string connectdb(DatabaseManager& dbm) {
    while(true) {
        std::string input;
        dbm.listDatabases();
        std::cout<<"- Enter a database name to connect (Enter -1 to Exit): ";
        getline(std::cin, input);
        if(input.substr(0, 2) == "-1")
            break;
        else if(input == "") {
            std::cout<<"\nERR: You cannot connect to a database with no name! Please enter an existing database."<<std::endl<<std::endl;
            continue;
        }
        else {
            int code = dbm.connectdb(input);
            if(code == 1)
                return input;
            else if(code == -1) {
                continue;
            }
            else if(code == -2) {
                std::cout<<"\nWARN: Log file empty. No database has been initialized. Exiting initialization section..."<<std::endl<<std::endl;
                continue;
            }
            else if(code == -3) {
                std::cout<<"\nERR: Database '"<<input<<"' does not exist. Please enter an existing database."<<std::endl<<std::endl;
                continue;
            }
        }
    }
    return "";
}

//// END DATABASE SELECTOR FUNCTIONS ////


//// DATABASE SPECIFIC FUNCTIONS ////
void dbMenu(EMap& em, std::string db_name) {
    char option;
    do {
        option = dbMenuOptions(db_name);
        switch(option) {
            case 'a': createEmployee(em); break;
            case 'b': listEmployees(em); break;
            case 'd': deleteEmployee(em); break;
            case 'e': importEmployees(em); break;
            case 'f': exportEmployees(em); break;
            case 'x': break;

            // testing/utilities
            case 'm':
                em.printMap();
                break;
            default:
                std::cerr<<"Invalid command. Please try again."<<std::endl<<std::endl;
                break;
        }
    } while(option != 'x');
}

// main menu returns a single char to be read by switch in main function
char dbMenuOptions(std::string& db_name) {
    std::cout<<smartCenterHeader(db_name, 42);
    std::cout<<"* a. Create New Employee                 *\n";
    std::cout<<"* b. List Employees                      *\n";
    std::cout<<"* c. Search For An Employee              *\n";
    std::cout<<"* d. Delete An Employee                  *\n";
    std::cout<<"* e. Import New Employees                *\n";
    std::cout<<"* f. Export Employees                    *\n";             
    std::cout<<"* x. Exit                                *\n";
    std::cout<<"******************************************\n\n";
    std::cout<<"Enter an option: ";
    std::string input; getline(std::cin, input);
    return input[0];
}

// center the header of a text block with the * character
std::string smartCenterHeader(std::string db_name, int width) {
    return db_name.length() % 2 == 0
    ?(std::string((width-db_name.length())/2, '*')+db_name+std::string((width-db_name.length())/2, '*')+'\n')
    :(std::string(((width-db_name.length())/2)+1, '*')+db_name+std::string((width-db_name.length())/2, '*')+'\n');
}

// create employee. cover all bases, catch user input errors and 
// make sure that certain strings are within the required length
void createEmployee(EMap& em) {
    std::string name = "", position = "", grade = "", payS = "", ageS = "";
    unsigned int id, age; int clearance; float payF;
    std::cout<<"------ADD EMPLOYEE------"<<std::endl;
    
    // employee_name
    do {
        std::cout<<"- Enter name of the employee (Enter -1 to Exit): ";
        getline(std::cin, name);
        if(name == "")
            std::cout<<"\nERR: Name not input correctly."<<std::endl<<std::endl;
        else if(name.length() >= 50)
            std::cout<<"\nERR: Name cannot be larger than 50 characters."<<std::endl<<std::endl;
        else if(name.substr(0, 2) == "-1")
            return;
    } while(name.length() >= 50 || name == "");
    
    // employee_age
    do {
        std::cout<<"- Enter the age of the employee (Enter -1 to Exit): ";
        getline(std::cin, ageS);
        try {
            age = std::stoul(ageS);
            break;
        }
        catch(const std::invalid_argument& e) {
            std::cout<<"\nERR: Please enter a valid age..."<<std::endl<<std::endl;
        }
    } while(true);

    // employee_position
    do {
        std::cout<<"- Enter the employee's position (Enter -1 to Exit): ";
        getline(std::cin, position);
        if(position == "")
            std::cout<<"\nERR: Position not input correctly."<<std::endl<<std::endl;
        else if(position.length() >= 50)
            std::cout<<"\nERR: Position title cannot be larger than 50 characters."<<std::endl<<std::endl;
        else if(name.substr(0, 2) == "-1")
            return;
    } while(position.length() >= 50 || position == "");

    // employee_pay_grade
    do {
        std::cout<<"- Enter the employee's pay grade (Enter -1 to Exit): ";
        getline(std::cin, grade);
        if(grade == "")
            std::cout<<"\nERR: Grade not input correctly."<<std::endl<<std::endl;
        else if(grade.length() >= 5)
            std::cout<<"\nERR: Pay grade cannot be larger than 4 characters."<<std::endl<<std::endl;
        else if(grade.substr(0, 2) == "-1")
            return;
    } while(grade.length()>=5 || grade == "");
    
    // employee_clearance
    char option = '\0'; std::string burn = "";
    do {
        std::cout<<"- Select the employee's clearance (Enter -1 to Exit): "<<std::endl;
        std::cout<<"  1. NONE"<<std::endl;
        std::cout<<"  2. CONFIDENTIAL"<<std::endl;
        std::cout<<"  3. SECRET"<<std::endl;
        std::cout<<"  4. TOP SECRET"<<std::endl;
        std::cout<<"- Select an option: ";
        getline(std::cin, burn); option = burn[0];
    
        if(option != '1' && option != '2' && option != '3' && option != '4')
            std::cout<<"\nERR: Improper selection. Please enter 1, 2, 3, or 4."<<std::endl<<std::endl;
        else if(burn.substr(0, 2) == "-1")
            return;
        else
            break;
    } while(true);
    clearance = ((int)option)-49;

    // TYPE
    option = '\0';
    do {
        std::cout<<"- Select the employee's pay type (Enter -1 to Exit): "<<std::endl;
        std::cout<<"  1. WAGES"<<std::endl;
        std::cout<<"  2. SALARY"<<std::endl;
        std::cout<<"- Select an option: ";
        getline(std::cin, burn); option = burn[0];
    
        if(option != '1' && option != '2')
            std::cout<<"\nERR: Improper selection. Please enter 1 or 2."<<std::endl<<std::endl;
        else if(burn.substr(0, 2) == "-1")
            return;
        else
            break;
    } while(true);

    
    // waged employee
    if(option == '1') {
        // hourly wages
        float payF = 0;
        do {
            std::cout<<"- Enter the employee's hourly wage (Enter -1 to Exit): ";
            getline(std::cin, payS);
            if(payS.substr(0, 2) == "-1")
                return;
            try {
                payF = std::stof(payS);
                break;
            }
            catch(const std::invalid_argument& e) {
                std::cout<<"\nERR: Please insert a valid number..."<<std::endl<<std::endl;
            }
        } while(true);

        WagedEmployee we;
        we.setEmployeeName(name); we.setEmployeePosition(position);
        we.setEmployeeAge(age); we.setEmployeePayGrade(grade);
        we.setEmployeeClearanceLevel((CLEARANCE)clearance); we.setWage(payF); 
        em.add_employee(we);
    }

    // salaried employee
    else {
        // yearly salary
        float payF = 0;
        do {
            std::cout<<"- Enter the employee's yearly salary (Enter -1 to Exit): ";
            getline(std::cin, payS);
            if(payS.substr(0, 2) == "-1")
                return;
            try {
                payF = std::stof(payS);
                break;
            }
            catch(const std::invalid_argument& e) {
                std::cout<<"\nERR: Please insert a valid number..."<<std::endl<<std::endl;
            }
        } while(true);

        SalariedEmployee se;
        se.setEmployeeName(name); se.setEmployeePosition(position);
        se.setEmployeeAge(age); se.setEmployeePayGrade(grade);
        se.setEmployeeClearanceLevel((CLEARANCE)clearance); se.setSalary(payF);
        em.add_employee(se);
    }
}

// list the employees and employ a filter if desired by the user
void listEmployees(EMap& em) {
    std::cout<<"Would you like to use a filter? [Y/N]: ";
    std::string input;
    getline(std::cin, input);
    if(tolower(input[0]) == 'y') {
        FilterType ftype = NOF; std::string filterCMP = "";
        while(true) {
            ftype = getFilterType();
            if(ftype == NOF) {
                em.read_employees_print_table();
                break;
            }
            filterCMP = getFilterCMP(ftype);
            if(filterCMP != "BRK") {
                em.read_employees_print_table(ftype, filterCMP);
                break;
            }
        }
    }
    else {
        em.read_employees_print_table();
    }

    // ask if user wishes to inspect an employee or return to the main menu
    while(true) {
        std::cout<<"If you wish to pull up an Employee card, enter their ID here (Enter -1 to Exit): ";
        std::string input; getline(std::cin, input);
        
        if(input == "") {
            std::cout<<"\nERR: That is not a valid option. Please enter a valid ID or -1 to exit."<<std::endl<<std::endl;
            continue;
        }
        else if(input.substr(0, 2) == "-1")
            return;

        try {
            unsigned int id = std::stoul(input);
            if(em.read_employee_record(id)) return;
            else {
                std::cout<<"\nERR: That employee_id does not exit. Please enter a valid ID or -1 to exit."<<std::endl<<std::endl;
                continue;
            }
        }
        catch(const std::invalid_argument& e) {
            std::cout<<"\nERR: That is not a valid option. Please enter a valid ID or -1 to exit."<<std::endl<<std::endl;
            continue;
        }
    }
}

// use EMap::delete_employee() to remove an employee from the binary file
void deleteEmployee(EMap& em) {
    em.read_employees_print_table();
    while(true) {
        std::cout<<"Enter The Employee ID you wish to delete (Enter -1 to Exit): ";
        unsigned int id; std::string input;
        getline(std::cin, input);
        if(input.substr(0, 2) == "-1")
            return;
        else {
            try {
                id = std::stoul(input);
                em.delete_employee(id);
                break;
            }
            catch(const std::invalid_argument& e) {
                std::cout<<"Please input a valid id."<<std::endl;
            }
        }
    }   
}

// use EMap::importEmployees() to import employees from a user-defined .txt file
void importEmployees(EMap& em) {
    // import employees from new file, read in data, and write new employees to EMap
    std::string input;
    while(true) {
        std::cout<<"Input .txt file to insert data from (Enter -1 to Exit): ";
        getline(std::cin, input);
        if(input.substr(0, 2) == "-1")
            return;
        else {
            if(em.importEmployees(input)) // try to read in file
                return;
            else
                std::cout<<"\nERR: File could not be opened. Please insert a valid file name."<<std::endl<<std::endl;
        }
    }   
}

// use EMap::exportEmployees() to export employees from user-defined binary file to a .txt file
void exportEmployees(EMap& em) {
    // export employees to a .txt file. File overwritting is, under no circumstances, allowed
    std::string input;
    while(true) {
        std::cout<<"Input the .txt file you wish to write out to. Make sure the file does not already exist! (Enter -1 to Exit): ";
        getline(std::cin, input);
        if(input == "") {
            std::cout<<"\nERR: File must have a name! Please define a name for your file."<<std::endl<<std::endl;
        }
        else if(input.substr(0, 2) == "-1")
            return;
        else {
            if(em.exportEmployees(input)) // try to export
                return;
            else
                std::cout<<"\nERR: File already exists! Please use a file name for a file that does not exist."<<std::endl<<std::endl;
        }
    }
}

// get the FilterType to be used by the listEmployees() function
// the FilterType is also needed to determine the formatting for the 
// getFilterCMP() function
FilterType getFilterType() {
    FilterType ftype; std::string input; int option;
    while(true) {
        std::cout<<"- Please select your desired filter type..."<<std::endl;
        std::cout<<"  1. ID"<<std::endl;
        std::cout<<"  2. NAME"<<std::endl;
        std::cout<<"  3. POSITION/JOB TITLE"<<std::endl;
        std::cout<<"  4. AGE"<<std::endl;
        std::cout<<"  5. PAY GRADE"<<std::endl;
        std::cout<<"  6. CLEARANCE LEVEL"<<std::endl;
        std::cout<<"- Enter your desired option (Enter -1 to Exit): ";
        getline(std::cin, input);

        if(input.substr(0, 2) == "-1")
            return NOF; // NO FILTER
        try {
            option = std::stoi(input);
            if(option < 1 || option > 6) {
                std::cout<<"\nERR: Please input a valie between 1-6, or -1 to exit..."<<std::endl<<std::endl;
                continue;
            }
            return ((FilterType)(option-1));
        }
        catch(const std::invalid_argument& e) {
            std::cout<<"\nERR: Please input a value between 1-6, or -1 to exit..."<<std::endl<<std::endl;
        }
    }
    return ftype;
}

// get the compare value used by EMap::read_employees_print_table()
// to properly print the filtered values
std::string getFilterCMP(FilterType ftype) {
    std::string input = "";
    if(ftype == ID) {
        while(true) {
            std::cout<<"Input your desired ID (Enter -1 to Exit): ";
            getline(std::cin, input);
            if(input == "") {
                std::cout<<"\nERR: You cannot filter by an empty ID! Please input a valid ID."<<std::endl<<std::endl;
                continue;
            }
            else if(input.substr(0, 2) == "-1")
                return "BRK";
            try {
                int check = std::stoi(input);
                return input;
            }
            catch(const std::invalid_argument& e) {
                std::cout<<"\nERR: That is not a valid ID. ID's should only be numerical..."<<std::endl<<std::endl;
            }
        }
    }
    else if(ftype == NAME) {
        while(true) {
            std::cout<<"Input the name you want to filter by (Enter -1 to Exit): ";
            getline(std::cin, input);
            if(input == "") {
                std::cout<<"\nERR: You cannot filter by an empty name! Please input a valid name."<<std::endl<<std::endl;
                continue;
            }
            else if(input.substr(0, 2) == "-1")
                return "BRK";
            else
                return input;
        }
    }
    else if(ftype == POSITION) {
        while(true) {
            std::cout<<"Input the position you want to filter by (Enter -1 to Exit): ";
            getline(std::cin, input);
            if(input == "") {
                std::cout<<"\nERR: You cannot filter by an empty position! Please input a valid position."<<std::endl<<std::endl;
                continue;
            }
            else if(input.substr(0, 2) == "-1")
                return "BRK";
            else
                return input;
        }
    }
    else if(ftype == AGE) {
        while(true) {
            std::cout<<"Input the age you want to filter by (Enter -1 to Exit): ";
            getline(std::cin, input);
            if(input == "") {
                std::cout<<"\nERR: You cannot filter by an empty age! Please input a valid age."<<std::endl<<std::endl;
                continue;
            }
            else if(input.substr(0, 2) == "-1")
                return "BRK";
            try {
                int check = std::stoi(input);
                return input;
            }
            catch(const std::invalid_argument& e) {
                std::cout<<"\nERR: That is not a valid age. Age should only be numerical..."<<std::endl<<std::endl;
            }
        }
    }
    else if(ftype == GRADE) {
        while(true) {
            std::cout<<"Input the pay grade you want to filter by (Enter -1 to Exit): ";
            getline(std::cin, input);
            if(input == "") {
                std::cout<<"\nERR: You cannot filter by an empty pay grade! Please input a valid pay grade (4 characters)."<<std::endl<<std::endl;
                continue;
            }
            else if(input.length() >= 4) {
                std::cout<<"\nERR: Pay grades are not longer than 4 characters. Please input a valid pay grade."<<std::endl<<std::endl;
            }
            else if(input.substr(0, 2) == "-1")
                return "BRK";
            else
                return input;
        }
    }
    else if(ftype == CLEARANCE_LEVEL) {
        while(true) {
            char option;
            std::cout<<"- Select the employee's clearance (Enter -1 to Exit): "<<std::endl;
            std::cout<<"  1. NONE"<<std::endl;
            std::cout<<"  2. CONFIDENTIAL"<<std::endl;
            std::cout<<"  3. SECRET"<<std::endl;
            std::cout<<"  4. TOP SECRET"<<std::endl;
            std::cout<<"- Select an option: ";
            getline(std::cin, input); option = input[0];
        
            if(option != '1' && option != '2' && option != '3' && option != '4')
                std::cout<<"\nERR: Improper selection. Please enter 1, 2, 3, or 4."<<std::endl<<std::endl;
            else if(input.substr(0, 2) == "-1")
                return "BRK";
            else
                switch(option) {
                    case '1': return "NONE";
                    case '2': return "CONFIDENTIAL";
                    case '3': return "SECRET";
                    case '4': return "TOP SECRET";
                }
        }
    }
    return "";
}
//// END DATABASE SPECIFIC FUNCTIONS ////
