g++ index.cpp -o index.cgi -lmariadbcpp -lcgicc -lcrypto
g++ register_page.cpp -o register_page.cgi -lmariadbcpp -lcgicc -lcrypto
g++ login_page.cpp -o login_page.cgi -lmariadbcpp -lcgicc -lcrypto
g++ admin_page.cpp -o admin_page.cgi -lmariadbcpp -lcgicc -lcrypto
g++ lecture_page.cpp -o lecture_page.cgi -lmariadbcpp -lcgicc -lcrypto

#compiling openssl code example
g++ encryption_test.cpp -o encryption_test.cgi -lmariadbcpp -lcgicc -lcrypto



sudo chown www-data index.cgi 
sudo chown www-data register_page.cgi
sudo chown www-data login_page.cgi
sudo chown www-data admin_page.cgi
sudo chown www-data lecture_page.cgi
sudo chown www-data encryption_test.cgi


sudo cp index.cgi /usr/lib/cgi-bin/
sudo cp register_page.cgi /usr/lib/cgi-bin/
sudo cp login_page.cgi /usr/lib/cgi-bin/
sudo cp admin_page.cgi /usr/lib/cgi-bin/
sudo cp lecture_page.cgi /usr/lib/cgi-bin/
sudo cp encryption_test.cgi /usr/lib/cgi-bin/

#use chmod 777 build_script.sh to make exe