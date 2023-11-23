#include <iostream>
#include <mariadb/conncpp.hpp>

void printTableData(std::shared_ptr<sql::Statement> &statement)
{
   //will attempt to run SQL statement and throw error with catch() if unsuccseful
   try {
      //SQL statement to get data from table
      std::unique_ptr<sql::ResultSet> getData(
            statement->executeQuery("SELECT id, FirstName, LastName, StudentYear FROM student_records.Students")
      ); 
      
      //CSS for table
      std::cout 
      << "<style>"
      << "table, th, td {"
      <<    "border:1px solid black;"
      << "}"
      << "</style>";

      //generates headers of table
      std::cout << "<table>";
      std::cout << "<tr>";
      std::cout << "<th>id</th>";
      std::cout << "<th>FirstName</th>";
      std::cout << "<th>LastName</th>";
      std::cout << "<th>StudentYear</th>";
      std::cout << "</tr>";

      // Loop over Result-set
      while (getData->next())
      {
         //prints the SQL statement defined in getData()
            std::cout
            << "<tr>"
            << "  <td>"
            << getData->getString("id")
            << "  </td>"
            << "  <td>"
            << getData->getString("FirstName")
            << "  </td>"
            << "  <td>"
            << getData->getString("LastName")
            << "  </td>"
            << "  <td>"
            << getData->getString("StudentYear")
            << "  </td>"
            << "</tr>"
            
            << std::endl;
      }
   }

   // Catch Exception
   catch (sql::SQLException& e) {
      std::cout << 
      "<pre>Error printing SQL statement </pre>";
   }
}

// Main Process
int main(int argc, char **argv)
{
   //required for html compatibilility
   std::cout << "Content-Type: text / html\r\n" << std::endl;
   //will attempt to connect to database and throw error with catch() if unsuccseful
   try {
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

      //print function
      printTableData(statement);

      // Close Connection
      conn->close();
   }

   // Catch Exceptions
   catch (sql::SQLException &e) {
      std::cout <<
      "<pre>503 Service Unavailable</pre>";
      return 1;
   }

   // Exit (Success)
   return 0;
}