#pragma once
#include<iostream>
#include<cstdlib>
#include<cstring>


typedef char String[50];
typedef char PayGrade[5];
enum CLEARANCE{NONE, CONFIDENTIAL, SECRET, TOP_SECRET};
enum TYPE{EMPLOYEE, WAGED, SALARIED};
struct EmployeeData {
    String employee_name;
    String employee_position;
    PayGrade employee_pay_grade;
    unsigned int employee_id;
    unsigned int employee_age;
    CLEARANCE employee_clearance;
};


class Employee {
    public:
        Employee();
        Employee(const Employee&); // copy constructor
        Employee(const EmployeeData&);
        ~Employee();

        // getters
        std::string getEmployeeName();
        std::string getEmployeePosition();
        unsigned int getEmployeeId();
        unsigned int getEmployeeAge();
        std::string getClearanceLevel();
        std::string getPayGrade();
        std::string getPayType();

        int getPayTypeInt(); // get integer version of enum return

        // setters
        void setEmployeeName(std::string);
        void setEmployeePosition(std::string);
        void setEmployeeId(unsigned int);
        void setEmployeeAge(unsigned int);
        void setClearanceLevel(CLEARANCE);
        void setPayGrade(std::string);
        void setPayType(TYPE);

        // friend operators
        friend std::ostream& operator << (std::ostream&, const Employee&);
        friend std::istream& operator >> (std::istream&, Employee&);
        friend bool operator > (const Employee&, const Employee&);
        friend bool operator < (const Employee&, const Employee&);
        friend bool operator >= (const Employee&, const Employee&);
        friend bool operator <= (const Employee&, const Employee&);
        friend bool operator == (const Employee&, const Employee&);
        friend bool operator != (const Employee&, const Employee&);

    protected:
        EmployeeData empdat;
        TYPE emp_type = EMPLOYEE;
        std::string formatOut(std::string key, std::string value) const;
        std::string formatCA25(std::string ca) const;
        std::string getClearance(CLEARANCE clearance) const;
};