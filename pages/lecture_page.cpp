#include <iostream>
#include <mariadb/conncpp.hpp>
#include <vector>
#include <cstring>
#include <string.h>
#include <string>

#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

using namespace cgicc;
using namespace std;

Cgicc cgi;

//prints module_list_table from database which shows all the modules and the assinged lecturer
void printModuleList(std::shared_ptr<sql::Statement> &statement)
{
   //will attempt to run SQL statement and throw error with catch() if unsuccseful
   //Note: try{} is used in lots of functions so commenting here once
   try {
      //SQL statement to get data from module_list_table
      std::unique_ptr<sql::ResultSet> getData(
            statement->executeQuery("SELECT module_name, assinged_user FROM student_records.module_list_table")
      ); 
  
      //CSS for table
      std::cout 
      << "<style>"
      << "table, th, td {"
      <<    "border:1px solid black;"
      << "}"
      << "</style>";

      //generates headers of table
      std::cout << "<table name =\"teacher_assingment_table\">";
      std::cout << "<tr>";
      std::cout << "<th>subject</th>";
      std::cout << "<th>assinged teacher name</th>";
      std::cout << "</tr>";

      // Loop over Result-set
      while (getData->next())
      {
         //prints the SQL statement defined in getData()
            std::cout
            << "<tr>"
            << "  <td>"
            << getData->getString("module_name")
            << "  </td>"
            << "  <td>"
            << getData->getString("assinged_user")
            << "  </td>"
            << "</tr>"
            
            << std::endl;
      }

      cout << "</table>";
   }

   // Catch Exception
    catch (sql::SQLException& e) {
        cout << 
        "<pre>Error </pre>";
    }
}

//prints the tables "maths", "english" and "science". Which shows all the students and their marks for modules
void printAllModules(std::shared_ptr<sql::Statement> &statement)
{
   try {
      //SQL statement to get data from each specific module table
      //Note: SQL statements use DISTINCT. This eliminates duplicates in the displayed table on the browser
      std::unique_ptr<sql::ResultSet> getMathsData(
            statement->executeQuery("SELECT DISTINCT student_name, mark FROM student_records.maths")
      ); 

      std::unique_ptr<sql::ResultSet> getScienceData(
            statement->executeQuery("SELECT DISTINCT student_name, mark FROM student_records.science")
      ); 

      std::unique_ptr<sql::ResultSet> getEnglishData(
            statement->executeQuery("SELECT DISTINCT student_name, mark FROM student_records.english")
      ); 
      
      //CSS for table
      std::cout 
      << "<style>"
      << "table, th, td {"
      <<    "border:1px solid black;"
      << "}"
      << "</style>";

      //generates headers of table
      std::cout << "<table name=\"student_mark_table\">";
      std::cout << "<tr>";
      std::cout << "<th>subject</th>";
      std::cout << "<th>Student name</th>";
      std::cout << "<th>Mark</th>";
      std::cout << "</tr>";

      // Loop over Result-set
      while (getMathsData->next())
      {
        //prints the table "maths"
        std::cout
        << "<tr>"
        << "  <td>maths</td>"
        << "  <td>"
        << getMathsData->getString("student_name")
        << "  </td>"
        << "  <td>"
        << getMathsData->getString("mark")
        << "  </td>"
        << "</tr>"
        
        << std::endl;
      }

      while (getScienceData->next())
      {
        //prints the table "science"
        std::cout
        << "<tr>"
        << "  <td>science</td>"
        << "  <td>"
        << getScienceData->getString("student_name")
        << "  </td>"
        << "  <td>"
        << getScienceData->getString("mark")
        << "  </td>"
        << "</tr>"
        
        << std::endl;
      }

      while (getEnglishData->next())
      {
        //prints the table "english"
        std::cout
        << "<tr>"
        << "  <td>english</td>"
        << "  <td>"
        << getEnglishData->getString("student_name")
        << "  </td>"
        << "  <td>"
        << getEnglishData->getString("mark")
        << "  </td>"
        << "</tr>"
        
        << std::endl;
      }

      cout << "</table>";
   }
   // Catch Exception
    catch (sql::SQLException& e) {
        cout << 
        "<pre>Error </pre>";
    }
}

//exectutes UPDATE statements in AddStudentToModule() and enters information into database 
void executeAddStudentToModule(std::shared_ptr<sql::PreparedStatement> &stmnt, sql::SQLString mark, sql::SQLString student_name)
{
    try{
        // Bind variables to prepared statement parameters
        // Note that the index starts at 1--not 0
        stmnt->setString(1, mark);
        stmnt->setString(2, student_name);

        // Execute Statement
        stmnt->executeUpdate();
    }
    // Catch Exception
    catch (sql::SQLException& e) {
        cout << 
        "<pre>Error </pre>";
    }
}

//takes inputted information from dropdown boxes and parses to executeAddStudentToModule() to input information into database
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

        //three indiviual update queries depending on what module is selected by lecturer. "mark = (?)" and "student_name = (?)" will be taken from user input
        std::shared_ptr<sql::PreparedStatement> science_module_stmnt(
            conn->prepareStatement(
            "UPDATE science SET mark = (?) WHERE student_name = (?);"
            )
        );

        std::shared_ptr<sql::PreparedStatement> maths_module_stmnt(
            conn->prepareStatement(
            "UPDATE maths SET mark = (?) WHERE student_name = (?);"
            )
        );

        std::shared_ptr<sql::PreparedStatement> english_module_stmnt(
            conn->prepareStatement(
            "UPDATE english SET mark = (?) WHERE student_name = (?);"
            )
        );

        //if statements to get the module choice from dropdown box using formData()
        if (formData("student_module_select") == "science")
        {
            //will take the name and mark entered by lecture from dropdown box and input box and use in query
            //the statement used differs dependant on the module choice selected
            executeAddStudentToModule(science_module_stmnt, formData("mark_element"), formData("student_name_element"));
        }
        else if (formData("student_module_select") == "maths")
        {
            executeAddStudentToModule(maths_module_stmnt, formData("mark_element"), formData("student_name_element"));
        }
        else if (formData("student_module_select") == "english")
        {
            executeAddStudentToModule(english_module_stmnt, formData("mark_element"), formData("student_name_element"));
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
        //will attempt to run SQL statement and throw error with catch() if unsuccseful
        //SQL statement to get data from table
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

void printModules(std::shared_ptr<sql::Statement> &statement)
{
    try{
        //SQL statement to get modules from module_list_table
        std::unique_ptr<sql::ResultSet> getData(
            statement->executeQuery("SELECT module_name FROM student_records.module_list_table")
        );

        cout << "<body>";

        cout << "<h1>Student module selector</h1>";
        //calls printStudents() to genereate dropdown box of student names
        printStudents(statement);

        //creates select box HTML element
        //Note: "name=" is what formData() will use to take the inputted data
        cout << "<select name = \"student_module_select\" >";

        // Loop over Result-set
        while (getData->next())
        {
            //creates rows of all modules in select box using data from module_list_table
            //Note: "module_name" is taken twice due to how the select html element works
            cout << "<option value=";
            cout << getData->getString("module_name");
            cout << ">";
            cout << getData->getString("module_name");
            cout << "</option>";
        }

        //generates number input type box to input marks. This element has automatic validation (restricted to numbers and restricted to 0 to 100)
        cout << "<input type=\"number\" name=\"mark_element\" min=\"0\" max=\"100\">";

        cout << "</select>";

        //calls AddStudentToModule() function to input user inputs into database
        AddStudentToModule(cgi);
        cout << "<div class=\"center\"><p>"
        << "<input type=\"submit\" name=\"submit\"  value=\"Submit\" />"
        << "<input type=\"reset\" value=\"Nevermind\" />"
        << "</p></div></form>" << endl;

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

        // Create a Statement
        std::shared_ptr<sql::Statement> statement(conn->createStatement());

        //calls functions to create a complete form
        printModules(statement);
        printAllModules(statement);
        printModuleList(statement);
        
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