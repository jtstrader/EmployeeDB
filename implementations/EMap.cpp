#include "../headers/EMap.h"

// constructors //
EMap::EMap() {
    
}

EMap::EMap(const EMap& old_eMap) {
    *this = old_eMap;
}

EMap::EMap(const std::map<unsigned int, long>& old_map) {
    *this = old_map;
}

EMap::~EMap() {

}
//////////////////

//// PUBLIC FUNCTIONS ////

// tester function to print the map to check on current byte offsets
// to ensure that saving is occuring properly.
void EMap::printMap() {
    EMap::iterator it = this->begin();
    std::cout<<"----EMap----"<<std::endl;
    for(; it != this->end(); ++it) {
        std::cout<<"- "<<it->first<<"  "<<it->second<<" -"<<std::endl;
    }
}

// public function that is used to set the name of the binary file
// for the database. Ideally grabbed from the command line or some
// environmental variable.
void EMap::setFileName(char* fileName) {
    this->fileName = fileName;
}

// public load function that is called to create the initial map
// and initialize the db from the binary file.
void EMap::load() {
    std::fstream load(fileName, std::ios::in | std::ios::binary);
    if(!load.is_open()) {
        load.close();
        std::fstream create(fileName, std::ios::out | std::ios::binary); create.close();
        return;
    }
    else {
        // load in data into map
        loadAllRecords(load);
        load.close();
    }
}

// add an employee to the map and binary file from 
// user input from the client file. Will append to the end
// if not deleted records are found, otherwise, it will 
// overwrite a deleted record.
bool EMap::add_employee(WagedEmployee& emp) {

    int type = emp.getEmployeePayTypeInt();
    if(type != 1)
        return false;

    // add new data to temporary employee list
    std::fstream outFile(fileName, std::ios::in | std::ios::out | std::ios::binary);
    if(emp.getEmployeeId() == -1) {
        emp.setEmployeeId(this->currMaxId+1);
        this->currMaxId++;
    }
    
    if(this->find(emp.getEmployeeId()) != this->end()) {
        std::cout<<"EMap Error: duplicate non-UINT_MAX key. Exiting add_employee..."<<std::endl;
        currMaxId--;
        return false;
    }

    EMap::iterator it = this->find(UINT_MAX);
    if(it != this->end()) {
        // remove UINT_MAX placeholder record and create new data for the map
        outFile.seekp(it->second);
        this->erase(it);
        this->insert(std::pair<unsigned int, long>(emp.getEmployeeId(), outFile.tellp()));
    }
    else {
        // insert new pair into map
        outFile.seekp(0, std::ios::end);
        this->insert(std::pair<unsigned int, long>(emp.getEmployeeId(), outFile.tellp()));
    } 

    bool active = true;
    outFile.write((char*)&type, sizeof(type));
    outFile.write((char*)&emp, sizeof(emp));
    outFile.write((char*)&active, sizeof(active));
    outFile.close(); // save data
    return true; // assume return true == OK status
}

// add an employee to the map and binary file from 
// user input from the client file. Will append to the end
// if not deleted records are found, otherwise, it will 
// overwrite a deleted record.
bool EMap::add_employee(SalariedEmployee& emp) {

    int type = emp.getEmployeePayTypeInt();
    if(type != 2)
        return false;

    // add new data to temporary employee list
    std::fstream outFile(fileName, std::ios::in | std::ios::out | std::ios::binary);
    if(emp.getEmployeeId() == -1) {
        emp.setEmployeeId(this->currMaxId+1);
        currMaxId++;
    }
    if(this->find(emp.getEmployeeId()) != this->end()) {
        std::cout<<"EMap Error: duplicate non-UINT_MAX key. Exiting add_employee..."<<std::endl;
        currMaxId--;
        return false;
    }

    EMap::iterator it = this->find(UINT_MAX);
    if(it != this->end()) {
        // remove UINT_MAX placeholder record and create new data for the map
        outFile.seekp(it->second);
        this->erase(it);
        this->insert(std::pair<unsigned int, long>(emp.getEmployeeId(), outFile.tellp()));
    }
    else {
        // insert new pair into map
        outFile.seekp(0, std::ios::end);
        this->insert(std::pair<unsigned int, long>(emp.getEmployeeId(), outFile.tellp()));
    } 

    bool active = true;
    outFile.write((char*)&type, sizeof(type));
    outFile.write((char*)&emp, sizeof(emp));
    outFile.write((char*)&active, sizeof(active));
    outFile.close(); // save data
    return true; // assume return true == OK status
}

// deletes and employee by marking its space in the binary
// file as "free" to be overwritten. The "active" boolean
// variable must be set to false.
bool EMap::delete_employee(unsigned int employee_id) {
    // set id to -1 and change bool handler to "false"
    std::fstream outFile(fileName, std::ios::in | std::ios::out | std::ios::binary);
    EMap::iterator it = this->find(employee_id);

    if(it == this->end()) {
        outFile.close();
        return false; // record not found
    }
    else {
        // set record id to UINT_MAX
        long offset = it->second;
        this->erase(it); // remove item from map
        this->insert(std::pair<unsigned int, long>(UINT_MAX, offset)); // reinsert with UINT_MAX key

        // go to offset and set bool value to false, indicating record is free
        long delOffset = gotoActiveVar(outFile, offset);
        bool active = false;
        outFile.seekp(delOffset);
        outFile.write((char*)&active, sizeof(active));
        outFile.close();
        return true;
    }
}

// calls on read_employee_print to print all employees
// currently stored in the map
bool EMap::read_employees_print() {
    for(EMap::iterator it=this->begin(); it!=this->end(); ++it) {
        if(it->first == UINT_MAX)
            continue;
        read_employee_print(it->first);
    }
    return true; // assuming nothing fails
}

// reads a single employee and prints in basic format.
// polymorphs employee into Employee& objects and prints
// them as such (will NOT have a wage/salary value)
bool EMap::read_employee_print(unsigned int employee_id) {
    EMap::iterator it = this->find(employee_id);
    if(it == this->end())
        return false;
    else {
        long offset = it->second;
        std::fstream inFile(this->fileName, std::ios::in | std::ios::binary);
        inFile.seekg(offset);
        
        int type;
        inFile.read((char*)&type, sizeof(type));
        if(type == 1) {
            // waged
            WagedEmployee we;
            inFile.read((char*)&we, sizeof(we));
            std::cout<<we<<std::endl;
        }
        else if(type == 2) {
            // salaried
            SalariedEmployee se;
            inFile.read((char*)&se, sizeof(se));
            std::cout<<se<<std::endl;
        }
        inFile.close();
    }
    return true;
}

// print the employee list in a table format. Can filter output using the
// FilterType parameter for the type of filter applied and the filterCMP 
// object to complete the comparison.
bool EMap::read_employees_print_table(FilterType ftype, std::string filterCMP) {
    std::fstream inFile(fileName, std::ios::in | std::ios::binary);
    std::cout<<"|"<<padStringCenter("ID", 8)<<"|"<<padStringCenter("Employee Name", 25)<<"|"<<padStringCenter("Position", 25)<<"|"<<padStringCenter("Clearance Level", 19)<<"|"<<std::endl;
    std::cout<<std::string(82, '-')<<std::endl;
    for(EMap::iterator it = this->begin(); it != this->end(); ++it) {
        if(it->first != UINT_MAX) {
            int type; inFile.seekg(it->second);
            inFile.read((char*)&type, sizeof(type));
            Employee* e;
            if(type == 0) continue;
            if(type == 1) {
                WagedEmployee we;
                inFile.read((char*)&we, sizeof(we));
                e = &we;
            }
            if(type == 2) {
                SalariedEmployee se;
                inFile.read((char*)&se, sizeof(se));
                e = &se;
            }
            if(filterItem(*e, ftype, filterCMP))
                std::cout<<"|"<<padStringLeft(std::to_string(e->getEmployeeId()), 8)<<"|"<<padStringCenter(e->getEmployeeName(), 25)<<"|"<<padStringCenter(e->getEmployeePosition(), 25)<<"|"<<padStringCenter(e->getEmployeeClearanceLevel(), 19)<<"|"<<std::endl;
        }
    }
    std::cout<<std::string(82, '-')<<std::endl;
    return true; // assuming nothing fails
}

//// END PUBLIC FUNCTIONS ////

//// PRIVATE FUNCTIONS ////

// load all records from the binary file into the map.
// use loadWageRecord() and loadSalaryRecord to complete this.
void EMap::loadAllRecords(std::fstream& inFile) { // assume offset 0
    int type;
    while(inFile.read((char*)&type, sizeof(int))) {
        // if(type == 0) loadEmployeeRecord(inFile);
        if(type == 1) loadWageRecord(inFile);
        if(type == 2) loadSalaryRecord(inFile);
    }
}

// load in a wage record into the map from the binary file
void EMap::loadWageRecord(std::fstream& inFile) {
    WagedEmployee we; bool active; long offset = inFile.tellp(); offset-=sizeof(int);
    inFile.read((char*)&we, sizeof(we));
    inFile.read((char*)&active, sizeof(bool));
    if(active) {
        this->insert(std::pair<unsigned int, long>(we.getEmployeeId(), offset));
        if(we.getEmployeeId()>this->currMaxId)
            this->currMaxId = we.getEmployeeId();
    }
    else
        this->insert(std::pair<unsigned int, long>(UINT_MAX, offset));
}

// load in a salary record into the map from the binary file
void EMap::loadSalaryRecord(std::fstream& inFile) {
    SalariedEmployee se; bool active; long offset = inFile.tellp(); offset-=sizeof(int);
    inFile.read((char*)&se, sizeof(se));
    inFile.read((char*)&active, sizeof(bool));
    if(active) {
        this->insert(std::pair<unsigned int, long>(se.getEmployeeId(), offset));
        if(se.getEmployeeId()>this->currMaxId)
            this->currMaxId = se.getEmployeeId();
    }
    else
        this->insert(std::pair<unsigned int, long>(UINT_MAX, offset));
}

// seeks to a given record in the fstream parameter and
// grabs the type, the record itself, and then stops at
// the offset where the active var is stored in the binary file.
// This is for editing the active var if a record is deleted
long EMap::gotoActiveVar(std::fstream& outFile, long offset) {
    outFile.seekg(offset);
    int type;
    outFile.read((char*)&type, sizeof(type));
    
    if(type == 0) {
        Employee e;
        outFile.read((char*)&e, sizeof(e));
    }
    else if(type == 1) {
        WagedEmployee we;
        outFile.read((char*)&we, sizeof(we));
    }  
    else if(type == 2) {
        SalariedEmployee se;
        outFile.read((char*)&se, sizeof(se));
    }

    // return where fstream now at "active" variable offset
    return outFile.tellg();
}

// import employees to the binary file, inputted by the user.
// returns false if the file is not found.
bool EMap::importEmployees(std::string fileName) {
    std::fstream inFile(fileName, std::ios::in);
    if(!inFile.is_open())
        return false;
    else {
        int type;
        while(inFile>>type) {
            // if(type == 0) {
            //     Employee e;
            //     inFile>>e;
            //     this->add_employee(e);
            // } deprecated
            if(type == 1) {
                WagedEmployee we;
                inFile>>we;
                this->add_employee(we);
            }
            else if(type == 2) {
                SalariedEmployee se;
                inFile>>se;
                this->add_employee(se);
            }
        }
    }
    return true;
}

// export employees to a file inputted by the user.
// duplicate files are under no circumstance allowed.
// returns false if the file already exists
bool EMap::exportEmployees(std::string exportFile) {
    if(exportFile.length() <= 4 || exportFile == ".txt")
        exportFile+=".txt";
    else if(exportFile.substr(exportFile.length()-4, 4) != ".txt")
        exportFile+=".txt"; // file must have .txt extension

    // make sure that a file does not already exist
    std::fstream exists(exportFile, std::ios::in);
    if(exists.is_open()) // file exists
        return false;
    else {
        exists.close();
        exists.open(exportFile, std::ios::out);
        std::fstream inFile(fileName, std::ios::in | std::ios::binary);
        for(EMap::iterator it = this->begin(); it != this->end(); ++it) {
            if(it->first != UINT_MAX) {
                inFile.seekg(it->second);
                int type;
                inFile.read((char*)&type, sizeof(type));
                
                if(type == 1) {
                    WagedEmployee we;
                    inFile.read((char*)&we, sizeof(we));
                    exists<<we;
                }
                else if(type == 2) {
                    SalariedEmployee se;
                    inFile.read((char*)&se, sizeof(se));
                    exists<<se;
                }
            }
        }
    }
    return true;
}

// pad strings to the left of a table column using a width value.
// uses <iomanip> setw() to ensure that the spacing is maintained.
std::string EMap::padStringLeft(std::string text, int width) {
    if(text.length() < width) {
        std::stringstream ss;
        ss << std::setw(width) << text;
        return ss.str();
    }
    else
        return text;
}

// pad strings to the center of a table using a width value. 
// uses <iomanip> setw() to ensure that the width of the string
// is maintained no matter if the string has an odd or even length
std::string EMap::padStringCenter(std::string text, int width) {
    std::string repeat = std::string((width-text.length()) / 2, ' ');
    std::stringstream ss;
    ss << std::setw(width) << repeat + text + repeat;
    return ss.str(); 
}

// filter items for the print_table function. return true if the item coincides with
// the filterCmp (compare). If the item does not follow this, return false, which will
// skip over printing the item in the print_table function
bool EMap::filterItem(Employee& emp, FilterType ftype, std::string cmp) {
    if(cmp == "") return true; // no comparison to make
    else if(ftype == ID) return std::to_string(emp.getEmployeeId()) == cmp;
    else if(ftype == NAME) return emp.getEmployeeName().find(cmp) != std::string::npos;
    else if(ftype == POSITION) return emp.getEmployeePosition().find(cmp) != std::string::npos;
    else if(ftype == AGE) return std::to_string(emp.getEmployeeAge()) == cmp;
    else if(ftype == GRADE) return emp.getEmployeePayGrade() == cmp;
    else if(ftype == CLEARANCE_LEVEL) return emp.getEmployeeClearanceLevel() == cmp;
    else return false; // err
}

//// END PRIVATE FUNCTIONS ////