#include<iostream>
#include<fstream>
#include "./headers/SalariedEmployee.h"
#include "./headers/WagedEmployee.h"
#include "./headers/EMap.h"

char menu();
void createEmployee(EMap& em);
void readEmployeeList(EMap& em);
void deleteEmployee(EMap& em);
void importEmployees(EMap& em);
void exportEmployees(EMap& em);

int main(int argc, char** argv) {

    EMap em;
    em.setFileName("out.dat");
    em.load();

    char option;
    do {
        option = menu();
        switch(option) {
            case 'a': {
                createEmployee(em);
                break;
            }
            case 'b': {
                em.read_employees_print_table();
                break;
            }
            case 'c': {
                break;
            }
            case 'd': {
                deleteEmployee(em);
                break;
            }
            case 'e': {
                importEmployees(em);
                break;
            }
            case 'f': {
                exportEmployees(em);
                break;
            }
            case 'g': {
                break;
            }
            case 'm': {
                em.printMap();
                break;
            }
            default: {
                std::cerr<<"Invalid command. Please try again."<<std::endl<<std::endl;
                break;
            }
        }
    } while(option != 'g');

    return 0;
}

char menu() {
    std::cout<<"******STRADER CORP EMPLOYEE DATABSE******\n";
    std::cout<<"* a. Create New Employee                *\n";
    std::cout<<"* b. List All Current Employees         *\n";
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
            std::cout<<"\n ERR: Please enter a valid age..."<<std::endl<<std::endl;
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
        we.setEmployeeAge(age); we.setPayGrade(grade);
        we.setClearanceLevel((CLEARANCE)clearance); we.setWage(payF); 
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
        se.setEmployeeAge(age); se.setPayGrade(grade);
        se.setClearanceLevel((CLEARANCE)clearance); se.setSalary(payF);
        em.add_employee(se);
    }
}

void readEmployeeList(EMap& em) {
    em.printMap();
}

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
                std::cout<<"ERR: File could not be opened. Please insert a valid file name."<<std::endl;
        }
    }   
}

void exportEmployees(EMap& em) {
    // export employees to a .txt file. File overwritting is, under no circumstances, allowed
    std::string input;
    while(true) {
        std::cout<<"Input the .txt file you wish to write out to. Make sure the file does not already exist! (Enter -1 to Exit): ";
        getline(std::cin, input);
        if(input.substr(0, 2) == "-1")
            return;
        else {
            if(em.exportEmployees(input)) // try to export
                return;
            else
                std::cout<<"ERR: File already exists! Please use a file name for a file that does not exist."<<std::endl;
        }
    }
}