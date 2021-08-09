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


    protected:
        float salary;
};
