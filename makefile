CC=--std=c++17

all: client clean

client: client.cpp SalariedEmployee.o WagedEmployee.o Employee.o EMap.o DatabaseManager.o
	c++ $(CC) -o EmployeeDB.exe client.cpp SalariedEmployee.o WagedEmployee.o Employee.o EMap.o DatabaseManager.o

Employee.o: ./headers/Employee.h ./implementations/Employee.cpp
	c++ $(CC) -c ./implementations/Employee.cpp

WagedEmployee.o: ./headers/WagedEmployee.h ./implementations/WagedEmployee.cpp
	c++ $(CC) -c ./implementations/WagedEmployee.cpp

SalariedEmployee.o: ./headers/SalariedEmployee.h ./implementations/SalariedEmployee.cpp
	c++ $(CC) -c ./implementations/SalariedEmployee.cpp

EMap.o: ./headers/EMap.h ./implementations/EMap.cpp
	c++ $(CC) -c ./implementations/EMap.cpp

DatabaseManager.o: ./headers/DatabaseManager.h ./implementations/DatabaseManager.cpp
	c++ $(CC) -c ./implementations/DatabaseManager.cpp

clean:
	rm *.o