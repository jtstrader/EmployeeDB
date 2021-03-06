#pragma once
#include<vector>
#include<iostream> // remove later if possible
#include<map>
#include<fstream>
#include<climits>
#include<iomanip>
#include<sstream>

#include "WagedEmployee.h"
#include "SalariedEmployee.h"

enum FilterType{ID, NAME, POSITION, AGE, GRADE, CLEARANCE_LEVEL, NOF};

// Employee Map class that handles all i/o operations
// with files concerning Employee objects. Note that
// this class implements a boolean written after every
// employee to determine if the employee is valid in the
// database or if the employee can be overwritten.
//
// If an employee can be be overritten, the byte offset
// is stored but the key value is set to UINT_MAX, meaning that
// any free space in the file can be determined by setting a
// UINT_MAX key value to a certain record offset that is now populated.
class EMap : std::multimap<unsigned int, long> {
    public:
        EMap();
        EMap(const EMap&);
        EMap(const std::map<unsigned int, long>&);
        ~EMap();

        // EmployeeDB manipulation
        bool add_employee(WagedEmployee&);
        bool add_employee(SalariedEmployee&);
        bool delete_employee(unsigned int employee_id);

        bool read_employee_print(unsigned int employee_id);
        bool read_employees_print(); // read all employees and print them
        bool read_employee_record(unsigned int employee_id); // read in specific employee record

        bool read_employees_print_table(FilterType filter = ID, std::string filterCMP = ""); // raed all employees and print them in table format

        // setup & testing
        void printMap();
        void setFileName(std::string);
        void load();

        bool importEmployees(std::string fileName);
        bool exportEmployees(std::string fileName);

    private:
        void loadAllRecords(std::fstream& inFile);
        // void loadEmployeeRecord(std::fstream& inFile);
        void loadWageRecord(std::fstream& inFile);
        void loadSalaryRecord(std::fstream& inFile);

        // WagedEmployee readWagedEmployee(std::fstream& inFile);
        // SalariedEmployee readSalariedEmployee(std::fstream& inFile);
        long gotoActiveVar(std::fstream&, long offset);
        bool filterItem(Employee& emp, FilterType ftype, std::string cmp);


        std::string padStringLeft(std::string text, int amt);
        std::string padStringCenter(std::string text, int width);
        
        std::string fileName;
        int currMaxId = 0;
};