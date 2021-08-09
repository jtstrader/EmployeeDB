#include "../headers/Employee.h"

// public functions //
// constructors & destructor //
Employee::Employee() {
    this->empdat.employee_id = -1;
}

Employee::Employee(const Employee& emp) {
    *this = emp;
}

Employee::Employee(const EmployeeData& empdat) {
    this->empdat = empdat;
}

Employee::~Employee() {

}

// getters //
std::string Employee::getEmployeeName() {
    return this->empdat.employee_name;
}

std::string Employee::getEmployeePosition() {
    return this->empdat.employee_position;
}

unsigned int Employee::getEmployeeId() {
    return this->empdat.employee_id;
}

unsigned int Employee::getEmployeeAge() {
    return this->empdat.employee_age;
}

std::string Employee::getEmployeeClearanceLevel() {
    return getClearance(this->empdat.employee_clearance);
}

std::string Employee::getEmployeePayGrade() {
    return this->empdat.employee_pay_grade;
}

std::string Employee::getEmployeePayType() {
    switch(this->emp_type) {
        case EMPLOYEE: return "EMPLOYEE";
        case WAGED: return "WAGED";
        case SALARIED: return "SALARIED";
        default: return "ERR";
    }
}

int Employee::getEmployeePayTypeInt() {
    return this->emp_type;
}

// setters //
void Employee::setEmployeeName(std::string emp_name) {
    strncpy(this->empdat.employee_name, emp_name.c_str(), 49);
    this->empdat.employee_name[49] = '\0';
}

void Employee::setEmployeePosition(std::string emp_position) {
    strncpy(this->empdat.employee_position, emp_position.c_str(), 49);
    this->empdat.employee_position[49] = '\0';
}

void Employee::setEmployeeId(unsigned int id) {
    this->empdat.employee_id = id;
}

void Employee::setEmployeeAge(unsigned int age) {
    this->empdat.employee_age = age;
}

void Employee::setEmployeeClearanceLevel(CLEARANCE clearance) {
    this->empdat.employee_clearance = clearance;
}

void Employee::setEmployeePayGrade(std::string payType) {
    strncpy(this->empdat.employee_pay_grade, payType.c_str(), 4);
    this->empdat.employee_pay_grade[4] = '\0';
}

void Employee::setEmployeePayType(TYPE type) {
    this->emp_type = type;
}

// operators
std::ostream& operator << (std::ostream& stream, const Employee& emp) {
    const EmployeeData* ed = &(emp.empdat);
    stream<<"---------------------------------------------\n";
    stream<<emp.formatOut("Employee ID: ", std::to_string(ed->employee_id));
    stream<<emp.formatOut("Employee Name: ", emp.formatCA25(std::string(ed->employee_name)));
    stream<<emp.formatOut("Position: ", std::string(ed->employee_position));
    stream<<emp.formatOut("Age: ", std::to_string(ed->employee_age));
    stream<<emp.formatOut("Clearance Level: ", emp.getClearance(ed->employee_clearance));
    stream<<"---------------------------------------------\n";
    return stream;
}

std::istream& operator >> (std::istream& stream, Employee& emp) {
    std::string burn;
    stream>>emp.empdat.employee_id; 
    getline(stream, burn);
    getline(stream, burn); strncpy(emp.empdat.employee_name, burn.c_str(), 50);
    getline(stream, burn); strncpy(emp.empdat.employee_position, burn.c_str(), 50);
    stream>>emp.empdat.employee_age; getline(stream, burn);
    getline(stream, burn);

    if(burn == "NONE")
        emp.empdat.employee_clearance = NONE;
    else if(burn == "CONFIDENTIAL")
        emp.empdat.employee_clearance = CONFIDENTIAL;
    else if(burn == "SECRET")
        emp.empdat.employee_clearance = SECRET;
    else if(burn == "TOP_SECRET")
        emp.empdat.employee_clearance = TOP_SECRET;
    else
        emp.empdat.employee_clearance = NONE;
    getline(stream, burn); strncpy(emp.empdat.employee_pay_grade, burn.c_str(), 4);
    return stream;
}

bool operator > (const Employee& e1, const Employee& e2) {
    return e1.empdat.employee_id > e2.empdat.employee_id;
}

bool operator < (const Employee& e1, const Employee& e2) {
    return e1.empdat.employee_id < e2.empdat.employee_id;
}

bool operator >= (const Employee& e1, const Employee& e2) {
    return e1.empdat.employee_id >= e2.empdat.employee_id;
}

bool operator <= (const Employee& e1, const Employee& e2) {
    return e1.empdat.employee_id <= e2.empdat.employee_id;
}

bool operator == (const Employee& e1, const Employee& e2) {
    return e1.empdat.employee_id == e2.empdat.employee_id;
}

bool operator != (const Employee& e1, const Employee& e2) {
    return e1.empdat.employee_id != e2.empdat.employee_id;
}

// private functions //
std::string Employee::formatOut(std::string key, std::string value) const {
    // size of card == 45 characters
    //   - 2 dedicated spaces to side bars
    //   - 2 dedicated spaces to space between words and side bar
    // total size that can be used for text == 45 - 4 = 41
    std::string out = "| " + key + value;
    for(int i=0; i<41-key.length()-value.length(); i++) out += " ";
    out += " |\n";
    return out;
}

std::string Employee::formatCA25(std::string ca) const {
    if(ca.length() >= 27)
        return (ca.substr(0, 23)) + "...";
    else
        return ca;
}

std::string Employee::getClearance(CLEARANCE clearance) const {
    switch(clearance) {
        case NONE:
            return "NONE";
        case CONFIDENTIAL:
            return "CONFIDENTIAL";
        case SECRET:
            return "SECRET";
        case TOP_SECRET:
            return "TOP SECRET";
        default:
            return "NOT DEFINED";
    }
}
