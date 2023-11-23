#include <iostream>
#include <mariadb/conncpp.hpp>
#include <string.h>
#include <string>

#include "cgicc/Cgicc.h"

using namespace cgicc;
using namespace std;

Cgicc cgi;

//exectutes UPDATE statements in addLectToModule() and enters information into database 
void executeAddLectToModule(std::shared_ptr<sql::PreparedStatement> &stmnt, sql::SQLString lecture_name)
{
    //will attempt to run SQL statement and throw error with catch() if unsuccseful
    //Note: try{} is used in lots of functions so commenting here once
    try{
        // Bind variables to prepared statement parameters
        // Note that the index starts at 1--not 0
        stmnt->setString(1, lecture_name);
        
        // Execute Statement
        stmnt->executeUpdate();
    }
    // Catch Exception
    catch (sql::SQLException& e) {
        cout << 
        "<pre>Error </pre>";
    }
}

void addLectToModule(const Cgicc& formData) 
{
    try{
        //configs driver
        sql::Driver* driver = sql::mariadb::get_driver_instance();

        // Configure Connection
        sql::SQLString dbConnectionString("jdbc:mariadb://127.0.0.1:3306/student_records");

        //login credentials
        sql::Properties dbLoginCredentials({
            {"user", "db_user"},
            {"password", "db_user_password"},
        });

        // Establish Connection using dbConnectionString and dbLoginCredentials
        std::unique_ptr<sql::Connection> conn(driver->connect(dbConnectionString, dbLoginCredentials));

        //three indiviual queries depending on what module is selected by adminstratour
        std::shared_ptr<sql::PreparedStatement> science_module_stmnt(
            conn->prepareStatement(
            "UPDATE module_list_table SET assinged_user = (?) WHERE module_name = 'science';"
            )
        );

        std::shared_ptr<sql::PreparedStatement> maths_module_stmnt(
            conn->prepareStatement(
            "UPDATE module_list_table SET assinged_user = (?) WHERE module_name = 'maths';"
            )
        );

        std::shared_ptr<sql::PreparedStatement> english_module_stmnt(
            conn->prepareStatement(
            "UPDATE module_list_table SET assinged_user = (?) WHERE module_name = 'english';"
            )
        );

        //if statements to get the module choice from dropdown box using formData()
        if (formData("lect_select") == "science")
        {
            //will take the name selected by adminstratour from dropdown box and use in query
            executeAddLectToModule(science_module_stmnt, formData("lecture_name_element")); 
        }
        else if (formData("lect_select") == "maths")
        {
            executeAddLectToModule(maths_module_stmnt, formData("lecture_name_element"));  
        }
        else if (formData("lect_select") == "english")
        {
            executeAddLectToModule(english_module_stmnt, formData("lecture_name_element")); 
        }
    }   

    // Catch Exception
    catch (sql::SQLException& e) {
        cout << 
        "<pre>Error </pre>";
    }
}

//exectutes INSERT statements in AddStudentToModule and enters information into database 
void executeAddStudentToModule(std::shared_ptr<sql::PreparedStatement> &stmnt, sql::SQLString student_name)
{
    try{
        // Bind variables to prepared statement parameters
        // Note that the index starts at 1--not 0
        stmnt->setString(1, student_name);
        
        // Execute Statement
        stmnt->executeUpdate();
    }

    // Catch Exception
    catch (sql::SQLException& e) {
        cout << 
        "<pre>Error </pre>";
    }
}

void AddStudentToModule(const Cgicc& formData) 
{
    try{
        //config driver
        sql::Driver* driver = sql::mariadb::get_driver_instance();

        // Configure Connection
        sql::SQLString dbConnectionString("jdbc:mariadb://127.0.0.1:3306/student_records");

        //login credentials
        sql::Properties dbLoginCredentials({
            {"user", "db_user"},
            {"password", "db_user_password"},
        });

        // Establish Connection using dbConnectionString and dbLoginCredentials
        std::unique_ptr<sql::Connection> conn(driver->connect(dbConnectionString, dbLoginCredentials));

        //three indiviual queries depending on what module is selected by adminstratour
        std::shared_ptr<sql::PreparedStatement> science_module_stmnt(
            conn->prepareStatement(
            "INSERT INTO `science` (`student_name`) VALUES (?)"
            )
        );

        std::shared_ptr<sql::PreparedStatement> maths_module_stmnt(
            conn->prepareStatement(
            "INSERT INTO `maths` (`student_name`) VALUES (?)"
            )
        );

        std::shared_ptr<sql::PreparedStatement> english_module_stmnt(
            conn->prepareStatement(
            "INSERT INTO `english` (`student_name`) VALUES (?)"
            )
        );

        //if statements to get the module choice from dropdown box using formData()
        if (formData("student_select") == "science")
        {
            //will take the name selected by adminstratour from dropdown box and use in query
            executeAddStudentToModule(science_module_stmnt, formData("student_name_element")); 
        }
        else if (formData("student_select") == "maths")
        {
            executeAddStudentToModule(maths_module_stmnt, formData("student_name_element"));  
        }
        else if (formData("student_select") == "english")
        {
            executeAddStudentToModule(english_module_stmnt, formData("student_name_element")); 
        }
    }
    // Catch Exception
    catch (sql::SQLException& e) {
        cout << 
        "<pre>Error </pre>";
    }
}

void printStudents(std::shared_ptr<sql::Statement> &statement)
{
    try{
        //SELECT statement takes all student names from student_table
        std::unique_ptr<sql::ResultSet> getData(
            statement->executeQuery("SELECT student_name FROM student_records.student_table;")
        ) ;

        cout << "<body>";
        cout << "<form>";
        //creates select box HTML element
        cout << "<select name = \"student_name_element\" >";

        // Loop over Result-set
        while (getData->next())
            {
                //creates rows of all student names in select box using data from student_table 
                //Note: "student_name" is taken twice due to how the select html element works
                cout << "<option value=";
                cout << getData->getString("student_name");
                cout << ">";
                cout << getData->getString("student_name");
                cout << "</option>";
            }
        cout << "</select>";
    }
    // Catch Exception
    catch (sql::SQLException& e) {
        cout << 
        "<pre>Error </pre>";
    }
}

void printLects(std::shared_ptr<sql::Statement> &statement)
{
    try{
        //SELECT statement takes all lect names from user_table
        std::unique_ptr<sql::ResultSet> getData(
            statement->executeQuery("SELECT name FROM student_records.user_table;")
        );

        cout << "<body>";
        cout << "<form>";
        //creates select box HTML element
        cout << "<select name = \"lecture_name_element\" >";

        // Loop over Result-set
        while (getData->next())
        {
            //creates rows in select box using data from user_table 
            cout << "<option value=";
            cout << getData->getString("name");
            cout << ">";
            cout << getData->getString("name");
            cout << "</option>";
        }
        cout << "</select>";
    }
    // Catch Exception
    catch (sql::SQLException& e) {
        cout << 
        "<pre>Error </pre>";
    }
}

/* this function will print the modules in dropdown that allow an admin to select them. 
This function also calls printStudents(), AddStudentToModule(), printLects() and addLectToModule()
and uses them to print the form used by the adminstatour. */
void printForm(std::shared_ptr<sql::Statement> &statement)
{
    try{
        //SELECT query to pull all modules, will be used by admins to bind a student to a module
        std::unique_ptr<sql::ResultSet> getModuleDataForStudents(
            statement->executeQuery("SELECT module_name FROM student_records.module_list_table")
        );

        //SELECT query to pull all modules, will be used by admins to bind a lecturer to a module
        std::unique_ptr<sql::ResultSet> getModuleDataForLects(
            statement->executeQuery("SELECT module_name FROM student_records.module_list_table")
        );

        cout << "<body>";
        
        cout << "<h1>Student module selector</h1>";
        //calls this function to show all students in a dropdown box on form
        printStudents(statement);

        //creates select box HTML element
        //Note: "name=" is what formData() will use to take the inputted data
        cout << "<select name = \"student_select\" >";

        // Loop over Result-set
        while (getModuleDataForStudents->next())
        {
            //creates rows of all modules in select box using data from module_list_table
            //Note: "module_name" is taken twice due to how the select html element works
            cout << "<option value=";
            cout << getModuleDataForStudents->getString("module_name");
            cout << ">";
            cout << getModuleDataForStudents->getString("module_name");
            cout << "</option>";
        }
        cout << "</select>";
        //calls AddStudentToModule()
        AddStudentToModule(cgi);
        
        //creates submit and reset boxes in form
        cout 
        << "<input type=\"submit\" name=\"student_submit\"  value=\"Submit\" />"
        << "<input type=\"reset\" value=\"Nevermind\" />";

        //terminates form from printStudents()
        cout << "</form>";

        cout << "<h1>Teacher module selector</h1>";
        //calls this function to show all teachers in a dropdown box on form
        printLects(statement);

        //creates select box HTML element
        cout << "<select name = \"lect_select\" >";

        // Loop over Result-set
        while (getModuleDataForLects->next())
        {
            //creates rows of all modules in select box using data from module_list_table
            cout << "<option  value=";
            cout << getModuleDataForLects->getString("module_name");
            cout << ">";
            cout << getModuleDataForLects->getString("module_name");
            cout << "</option>";
        }
        cout << "</select>";
        //calls addLectToModule()
        addLectToModule(cgi);
        //creates submit and reset boxes in form
        cout 
        << "<input type=\"submit\" name=\"lect_submit\"  value=\"Submit\" />"
        << "<input type=\"reset\" value=\"Nevermind\" />";
        
        //terminates form from printLects()
        cout << "</form>";
    }
    // Catch Exception
    catch (sql::SQLException& e) {
        cout << 
        "<pre>Error </pre>";
    }
}

//this creates a link that allows the admin to logout
void logoutFunction(const Cgicc& cgi){
  cout << "<a href=\"http://localhost/cgi-bin/login_page.cgi\">click here to logout</a>";
}

// Main Process
int main(int argc, char **argv)
{
    try{
        //required for html compatibilility
        std::cout << "Content-Type: text / html\r\n" << std::endl;

        //create SQL Driver
        sql::Driver* driver = sql::mariadb::get_driver_instance();

        // Configure Connection
        sql::SQLString dbConnectionString("jdbc:mariadb://127.0.0.1:3306/student_records");

        //login credentials
        sql::Properties dbLoginCredentials({
            {"user", "db_user"},
            {"password", "db_user_password"},
        });

        // Establish Connection using dbConnectionString and dbLoginCredentials
        std::unique_ptr<sql::Connection> conn(driver->connect(dbConnectionString, dbLoginCredentials));

        // Create a Statement that can be parsed to other functions
        std::shared_ptr<sql::Statement> statement(conn->createStatement());

        //call printForm() statement that will effectivelly generate the entire form
        printForm(statement);

        //this creates a link that allows the admin to move to lecture_page to verify changes.
        cout << "<br></br>";
        cout << "<a href=\"http://localhost/cgi-bin/lecture_page.cgi\">click here for lect page</a>";
        cout << "<br></br>";

        //calls logout function
        logoutFunction(cgi);

        // Close Connection
        conn->close();
    }
    // Catch Exception
    catch (sql::SQLException& e) {
        cout << 
        "<pre>Error </pre>";
    }
}