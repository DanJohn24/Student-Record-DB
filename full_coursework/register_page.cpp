#include <iostream>
#include <mariadb/conncpp.hpp>
#include <vector>
#include <cstring>
#include <string.h>

#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>

using namespace cgicc;
using namespace std;

//defines cgi. Will be used throughout program to parse into functions
Cgicc cgi;

//will encrypt strings parsed into function with SHA256
//library used is https://github.com/openssl/openssl
string CryptGen(const string str){
  unsigned char hash[SHA256_DIGEST_LENGTH];

  SHA256_CTX crpyt_string;
  SHA256_Init(&crpyt_string);
  SHA256_Update(&crpyt_string, str.c_str(), str.size());
  SHA256_Final(hash, &crpyt_string);

  stringstream ss;

  for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
    ss << hex << setw(2) << setfill('0') << static_cast<int>( hash[i] );
  }
  return ss.str();
}

//exectutes INSERT statements in InsertUser() and enters information into database 
void InsertUserExecuteStatement(std::shared_ptr<sql::PreparedStatement> &stmnt, sql::SQLString emailAddress, sql::SQLString FullName, sql::SQLString Password, sql::SQLString userType)
{
 	try {
		// Bind variables to prepared statement parameters
		// Note that the index starts at 1--not 0
		stmnt->setString(1, emailAddress);
    stmnt->setString(2, FullName);
		stmnt->setString(3, Password);
		stmnt->setString(4, userType);

		// Execute Statement
		stmnt->executeUpdate();
 	}

 	// Handle Exceptions
 	// Catch Exception
   catch (sql::SQLException& e) {
      cout << 
      "<pre>Error </pre>";
   }
}

//Prints form that allows user input
void printForm(const Cgicc& cgi)
{
	cout << "<form method=\"post\" action=\"" << "\">" << endl;
	
	cout << "<table>" << endl;

  //Where email (used for login in login_page.cgi) are inputted
  //Note: "name=" is what formData() will use to take the inputted data
	cout << "<tr><td class=\"title\">Email</td>"
			<< "<td class=\"form\">"
			<< "<input type=\"text\" name=\"email_address\">"
			<< "</td></tr>" << endl;
  
  //Where full names are inputted
  cout << "<tr><td class=\"title\">full-name</td>"
			<< "<td class=\"form\">"
			<< "<input type=\"text\" name=\"full_name\">"
			<< "</td></tr>" << endl;

  //Where passwords are inputted
	cout << "<tr><td class=\"title\">password</td>"
			<< "<td class=\"form\">"
			<< "<input type=\"password\" name=\"password\">"
			<< "</td></tr>" << endl;

	cout << "</table>" << endl;

  //used to generate radio buttons to choose user type
  /* Note: "name=" is what formData() will use to take the inputted data. 
  FormData() will take whatever is defined in "value=" of the selected radio button*/
	cout << "<tr><td class=\"title\">User type</td>"
			<< "<td class=\"form\">"
			<< "<input type=\"radio\" name=\"user_Type\" value=\"Lect\""
			<< " checked=\"checked\" />Lect"
			<< "<input type=\"radio\" name=\"user_Type\" value=\"Admin\" />Admin"
			<< "</td></tr>" << endl;

  //Submit and reset buttons generated here
	cout << "<div class=\"center\"><p>"
			<< "<input type=\"submit\" name=\"submit\"  value=\"Submit\" />"
			<< "<input type=\"reset\" value=\"Nevermind\" />"
			
			<< "</p></div></form>" << endl;
}

/* Boolean statement to ensure that only one admin can be made. 
Will return true if admin is allready in user_table and false if not  */
bool MultipleAdminCheck(std::shared_ptr<sql::Statement> &statement){
  try{
    //SELECT statement to query the user_type of every record in user_table
    std::unique_ptr<sql::ResultSet> getData(
      statement->executeQuery("SELECT user_type FROM student_records.user_table")
    );

    // Loop over Result-set
    while (getData->next())
    {
      //will check if any of the records have a user_type == "Admin"
      if ((strcmp(getData->getString("user_type"), "Admin") == 0))
      {
        //if any records have a user_type == "Admin", function will return true
        return(true);
      }
    }
    //if no records have a user_type == "Admin", function will return false
    return(false);
  }
  // Catch Exception
  catch (sql::SQLException& e) {
    cout << 
    "<pre>Error </pre>";
  }
}

/* Boolean statement to ensure no duplicate login emails are inputted into the database.
Will return true if duplicate user is detected and false if not  */
bool duplicateUsernameCheck(std::shared_ptr<sql::Statement> &statement , sql::SQLString emailAddress){
  try{
    //SELECT statement to query the login_email of every record in user_table
    std::unique_ptr<sql::ResultSet> getData(
      statement->executeQuery("SELECT login_email FROM student_records.user_table")
    );

    // Loop over Result-set
    while (getData->next())
    {
      //will check if the "login_email" field of any records is equal to the parsed in value emailAddress
      if ((strcmp(getData->getString("login_email"), emailAddress) == 0))
      {
        //if the "login_email" field of any records is equal to the parsed in value emailAddress, will return true
        return(true);
      }
    }
    //if the "login_email" field of any records is not equal to the parsed in value emailAddress, will return false
    return(false);
  }
  // Catch Exception
  catch (sql::SQLException& e) {
    cout << 
    "<pre>Error </pre>";
  }
}

//resets all HTML elements. Found it reduces issues across page
void resetAll(){
  html::reset(); 	head::reset(); 		body::reset();
	title::reset(); 	h1::reset(); 		h4::reset();
	comment::reset(); 	td::reset(); 		tr::reset(); 
	table::reset();	cgicc::div::reset(); 	p::reset(); 
	a::reset();		h2::reset(); 		colgroup::reset();
}

/* takes input from user input and parses to InsertUserExecuteStatement()
also calls duplicateUsernameCheck() and MultipleAdminCheck() when users register */
void InsertUser(const Cgicc& formData) 
{
  //will attempt to run SQL statement and throw error with catch() if unsuccseful
  //Note: try{} is used in lots of functions so commenting here once
  try{
    resetAll();

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

    //INSERT statement to enter user information into database
    std::shared_ptr<sql::PreparedStatement> stmnt(
      conn->prepareStatement(
        "INSERT INTO `user_table` (`login_email`,`name`,`password`, `user_type`) VALUES (?, ?, ?, ?)"
      )
    );

    //create a statement. Will be used as an argument
    std::shared_ptr<sql::Statement> statement(conn->createStatement());

    //defines const_form_iterator variables and takes information from form with formData.getElement()
    const_form_iterator name = formData.getElement("email_address");
    const_form_iterator full_name = formData.getElement("full_name");
    const_form_iterator password = formData.getElement("password");
    const_form_iterator radioButtons = formData.getElement("user_Type");

    /* checks const_form_iterator variables are not empty with isEmpty().
    this is to ensure that null values are not entered into the database  */
    if(!name->isEmpty() && !password->isEmpty() && !radioButtons->isEmpty() && !full_name->isEmpty())
    {
      //parses inputted email address to duplicateUsernameCheck() (and statement) to ensure it is not a duplicate
      if (!duplicateUsernameCheck(statement, formData("email_address")))
      {
        //if the email address is not duplicate will check what radio button is selected
        if (formData("user_type") == "Admin" )
        {
          //if the admin radio button is selected MultipleAdminCheck() will be checked to ensure that there is not already an admin registerd
          if (MultipleAdminCheck(statement) == true)
          {
            //if MultipleAdminCheck() returns true the user will not be registered and the error below will be displated
            cout << "<h2>admin account allready exists</h2>";
          }
          else if (!MultipleAdminCheck(statement))
          {
            //if MultipleAdminCheck() returns false, all the user information from the form will be parsed to InsertUserExecuteStatement() and entered into database
            //cryptGen() is used to hash the inputted password
            InsertUserExecuteStatement(stmnt, formData("email_address"), formData("full_name"), CryptGen(formData("password")), formData("user_Type"));
            resetAll();
          }
        }
        else if (formData("user_type") == "Lect" )
        {
          //if the lecturer radio button is selected all the user information from the form will be parsed to InsertUserExecuteStatement() and entered into database
          //cryptGen() is used to hash the inputted password
          InsertUserExecuteStatement(stmnt, formData("email_address"), formData("full_name"), CryptGen(formData("password")), formData("user_Type"));
          resetAll();
        }
      }
      else
      {
        //if the email is a duplicate of a record in the system duplicateUsernameCheck() will return true and show the error below
        cout << "<h2>Email address is already in system</h2>";
      }
    }
    else
    {
      //if any fields are empty, the error below will be displayed
      cout << "<h2>Please input data into fields to register</h2>";
    }
  }
  // Catch Exception
  catch (sql::SQLException& e) {
    cout << 
    "<pre>Error </pre>";
  }
}

// Main Process
int main(int argc, char **argv)
{
	resetAll();

  //required for html compatibilility
	cout << "Content-Type: text / html\r\n" << std::endl;
  
  //calls printForm() to display register form
	printForm(cgi);

  //calls insertUser()
	InsertUser(cgi);

  resetAll();

}