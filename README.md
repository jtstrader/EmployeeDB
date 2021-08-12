# Employee Binary DB (Local)
This is a local binary file used as a database of employees. The employees are based off of a mock secure system which has clearance levels used to designate which projects an employee can access. There is a generalized employee class to be inherited from, that contains the basic employee information, getters/setters, and overloads in a struct. By default the > and < operators are overloaded to compare employee ids, though this can be changed to support any comparison between two employees.

This project is rather extensive for what it is meant to do and perhaps even more complex than it needs to be. Note this was created for teaching purposes and for showing the power of C++ inheritance, polymorphism, binary data, typedef-ing, etc. to those interested in C++'s capabilities. 

# Extending std::multimap<T>
C++'s `std::multimap<T>` is a powerful tool used to organize data, while allowing single keys to hold multiple values. This is particularly useful for this project in keeping data organized into a single container.

Data in the binary file is organized like a hard drive, in where when data is "deleted" the data still exists in the file, and is instead just set to be overwritten by whatever new data is added. Because there are two major types of employees (waged and salaried), there is a type header that comes before every employee. Then the employee record comes, and then the boolean value that determines if the record is active or not. Realistically, the boolean *could* be stored in the employee class. However, I put the boolean as a separate entity in the binary file to help understanding of the get/put pointers and the power of using seeks in binary files.

The multimap, therefore, is particulary useful for this. Using an `unsigned int` as the key value, I set all deleted record keys in the multimap to C++'s `UINT_MAX` variable from the `climits` library. Because UINT_MAX can store multiple values (since it is a part of the multimap), all deleted records can be referenced instantly by pointing to the UINT_MAX key and pulling whatever value we so desire from the list.

The map is extended in order to provide a multitude of methods that can directly access the data of the map and perform operations without having to make the map a parameter throughout the program. As well as this, it allows the merging of a multimap (of similar type) into the the EMap class meaning that any multimap previously defined with employee data can be created into an EMap, with all needed functions to read, write, and manipulate data.

Going up the heirarchy we have DatabaseManager which maintains the multiple databases that a user can create. This also extends the multimap and follows a similar procedure for saving and deleting/overwriting records. However, the DatabaseManager is an extension of a `std::multimap<std::string, long>`, relating database names to their offsets. Therefore, records ready to be overwritten are stored as "bangs" (!).

# Searching and Filtering
Employee ID's are the primary searching key for this dataset. Searching for employees based on other qualifications (such as pay type) have an average O(n) runtime. Searching for employees based on id has a runtime of O(1).

Listing employee records uses the searching function as a filter and returns a list of output instead of an individual record. The employee listing function can be sorted through some keywords/options to help refine the search, such as listing only Salaried employees. Printing, and by extension filtering, has a runtime of O(n).

***TODO***: Searching through databases is not implemented and probably will not be for the time being, but the next update to DatabaseManager is planned for more information to be added about database files (particularly a header to the database file that will keep track of all present records in the database which can be relayed to the user from the DatabaseManager).