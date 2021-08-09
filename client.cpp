#include<iostream>
#include<fstream>
#include "./headers/SalariedEmployee.h"
#include "./headers/WagedEmployee.h"
#include "./headers/EMap.h"

// switch functions
char menu();
void createEmployee(EMap& em);
void listEmployees(EMap& em);
void deleteEmployee(EMap& em);
void importEmployees(EMap& em);
void exportEmployees(EMap& em);

// utils
FilterType getFilterType();
std::string getFilterCMP(FilterType ftype);

int main(int argc, char** argv) {

    EMap em;
    em.setFileName("out.dat");
    em.load();

    char option;
    do {
        option = menu();
        switch(option) {
            case 'a':
                createEmployee(em);
                break;
            case 'b':
                listEmployees(em);
                break;
            case 'c':
                break;
            case 'd':
                deleteEmployee(em);
                break;
            case 'e':
                importEmployees(em);
                break;
            case 'f':
                exportEmployees(em);
                break;
            case 'g':
                break;
            case 'm':
                em.printMap();
                break;
            default:
                std::cerr<<"Invalid command. Please try again."<<std::endl<<std::endl;
                break;
        }
    } while(option != 'g');

    return 0;
}


// main menu returns a single char to be read by switch in main function
char menu() {
    std::cout<<"******STRADER CORP EMPLOYEE DATABSE******\n";
    std::cout<<"* a. Create New Employee                *\n";
    std::cout<<"* b. List Employees                     *\n";
    std::cout<<"* c. Search For An Employee             *\n";
    std::cout<<"* d. Delete An Employee                 *\n";
    std::cout<<"* e. Import New Employees               *\n";
    std::cout<<"* f. Export Employees                   *\n";
    std::cout<<"* g. Save and Exit                      *\n";
    std::cout<<"* h. Exit Without Saving                *\n";
    std::cout<<"*****************************************\n\n";
    std::cout<<"Enter an option: ";
    std::string input; getline(std::cin, input);
    return input[0];
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
        do {
            std::cout<<"- Enter the employee's hourly wage (Enter -1 to Exit): ";
            getline(std::cin, payS);
            if(payS.substr(0, 2) == "-1")
                return;
            try {
                float payF = std::stof(payS);
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
        do {
            std::cout<<"- Enter the employee's yearly salary (Enter -1 to Exit): ";
            getline(std::cin, payS);
            if(payS.substr(0, 2) == "-1")
                return;
            try {
                float payF = std::stof(payS);
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
                return;
            }
            filterCMP = getFilterCMP(ftype);
            if(filterCMP != "BRK") {
                em.read_employees_print_table(ftype, filterCMP);
                return;
            }
        }
    }
    else {
        em.read_employees_print_table();
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
                std::cout<<"\nERR: File could not be opened. Please insert a valid file name."<<std::endl;
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
            std::cout<<"\nERR: File cannot have no name! Please define a name for your file."<<std::endl<<std::endl;
        }
        if(input.substr(0, 2) == "-1")
            return;
        else {
            if(em.exportEmployees(input)) // try to export
                return;
            else
                std::cout<<"\nERR: File already exists! Please use a file name for a file that does not exist."<<std::endl;
        }
    }
}

// utilities //

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