#pragma once
#include "Employee.h"

class WagedEmployee : public Employee {
    public:
        WagedEmployee();
        WagedEmployee(const WagedEmployee&); // copy constructor
        WagedEmployee(const EmployeeData&);
        ~WagedEmployee();

        // getters
        float getWage();
        float getWeeklyPay(int hoursWorked);
        float getBiWeeklyPay(int hoursWeek1, int hoursWeek2);

        // setters
        void setWage(float wage);
    
    protected:
        float wage;
};