#include <iostream>
#include <mariadb/conncpp.hpp>
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

//resets all HTML elements. Found it reduces issues across page
void resetAll(){
  html::reset(); 	head::reset(); 		body::reset();
	title::reset(); 	h1::reset(); 		h4::reset();
	comment::reset(); 	td::reset(); 		tr::reset(); 
	table::reset();	cgicc::div::reset(); 	p::reset(); 
	a::reset();		h2::reset(); 		colgroup::reset();
}

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

//takes user inputs from parsed in login details and comparing to database with SELECT statement
void loginAuth(std::shared_ptr<sql::Statement> &statement, sql::SQLString Email, sql::SQLString Password)
{
   //will attempt to run SQL statement and throw error with catch() if unsuccseful
   //Note: try{} is used in lots of functions so commenting here once
   try{
      resetAll();

      //quieries login_email, password and user_type from database be used to compare with user input
      std::unique_ptr<sql::ResultSet> getData(
            statement->executeQuery("SELECT login_email, password, user_type FROM student_records.user_table;")
      ); 

      // Loop over Result-set
      while (getData->next())
      {
         //first compares parsed in username input to every record in the database for a match
         if ((strcmp(getData->getString("login_email"), Email) == 0))
         {
            //if parsed in username input matches record in database will then proceed to compare parsed in password input to every record in the database for a match
            if ((strcmp(getData->getString("password"), Password) == 0))
            {
               //if parsed in username and password match records in the database will then get the user type of the record
               if ((strcmp(getData->getString("user_type"), "Admin") == 0))
               {   
                  //if the user type of the record is "Admin" a link to the admin page will appear
                  cout << "<a href=\"http://localhost/cgi-bin/admin_page.cgi\">click here for admin page</a>";
               }
               else if ((strcmp(getData->getString("user_type"), "Lect") == 0))
               {
                  //if the user type of the record is "Lect" a link to the lecture page will appear
                  cout << "<a href=\"http://localhost/cgi-bin/lecture_page.cgi\">click here for lect page</a>";
               }
            }
         }
      }
   }
   // Catch Exception
   catch (sql::SQLException& e) {
      cout << 
      "<pre>Error </pre>";
   }
}

//will take user input and parse it to loginAuth() for authentication
void loginPasser(const Cgicc& formData) 
{
   try{
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

      //create a statement. Will be used as an argument
      std::shared_ptr<sql::Statement> statement(conn->createStatement());

      /* takes entered inputted data from the form using formData and parses it to loginAuth() as arguments for authentication
      cryptGen() is used to hash the inputted password so that it can be compared to the database (where all passwords are stored in SHA256)*/
      loginAuth (statement, formData("name"), CryptGen(formData("password")));

      // Close Connection
      conn->close();
   }
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

   //Where usernames are inputted
   //Note: "name=" is what formData() will use to take the inputted data
   cout << "<tr><td class=\"title\">Username</td>"
      << "<td class=\"form\">"
      << "<input type=\"text\" name=\"name\">"
      << "</td></tr>" << endl;

   //Where passwords are inputted
   cout << "<tr><td class=\"title\">password</td>"
      << "<td class=\"form\">"
      << "<input type=\"password\" name=\"password\">"
      << "</td></tr>" << endl;

   cout << "</table>" << endl;

   //Submit and reset buttons generated here
   cout << "<div class=\"center\"><p>"
      << "<input type=\"submit\" name=\"submit\"  value=\"Submit\" />"
      << "<input type=\"reset\" value=\"Nevermind\" />"
      
      << "</p></div></form>" << endl;
}

int main(int argc, char **argv)
{
   //required for html compatibilility
   cout << "Content-Type: text / html\r\n" << std::endl;

   resetAll();

   //calls printForm() function
   printForm(cgi);

   //calls loginPasser() function
   loginPasser(cgi);
}