g++ week_10_demo.cpp -o test_file.cgi -lmariadbcpp -lcgicc 
sudo chown www-data test_file.cgi 
sudo cp test_file.cgi /usr/lib/cgi-bin/

#use chmod 777 build_script.sh to make exe