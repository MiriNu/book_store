#include <iostream>
#include <mysql_connection.h>
#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "actions.h"
#include "db.h"

#define NAMESPACES using namespace boost::posix_time;using namespace boost::gregorian;using namespace std;using namespace sql;

NAMESPACES;
int main(int argc, const char * argv[]) {
	showBooks();
	cout << endl;
	showOrders();
	cout << endl;
	showCustomers();
	cout << endl;
	showSuppliers();
	cout << endl;
	showPurchases("2017-03-04", "2017-06-04");
	cout << endl;
	showBooksOnSale();
	return 0;
}
