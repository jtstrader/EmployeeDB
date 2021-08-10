#pragma once
#include "Employee.h"

class SalariedEmployee : public Employee {
    public:
        SalariedEmployee();
        ~SalariedEmployee();

        // getters
        float getSalary();
        float getBiWeeklyPay();

        // setters
        void setSalary(float salary);

        // operator overloads
        friend std::ostream& operator << (std::ostream&, SalariedEmployee&);
        friend std::istream& operator >> (std::istream&, SalariedEmployee&);

        // pretty print
        std::string pprint();

    protected:
        float salary;
};
