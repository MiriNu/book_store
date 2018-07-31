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
	showPurchases("2017-03-27", "2017-08-06");
	cout << endl;
	showBooksOnSale();
	cout << endl;
	searchBook("American Gods", "Neil Gaiman");
	cout << endl;
	showSuppliersOfBook("American Gods", "Neil Gaiman");
	cout << endl;
	booksSold("American Gods", "Neil Gaiman", "2017-06-18");
	cout << endl;
	booksPurchased(4, "2017-12-01");
	cout << endl;
	mostPurchasesCustomer("2016-12-21");
	cout << endl;
	mostOrdersSuplier("2016-01-01");
	cout << endl;
	ordersMade("2016-01-01", "2019-01-02");
	cout << endl;
	ordersMadePurchases("2016-01-01", "2019-01-02");
	cout << endl;
	totalDiscountCustomer(10, "2016-01-01");
	cout << endl;
	sumRevenue(2017);
	cout << endl;
	customersAdded("2016-01-01");
	cout << endl;
	totalPaidSupplier(8, "2016-01-01", "2019-01-02");
	cout << endl;
	totalSoldSeller(1, "2016-01-01", "2019-01-02");
	cout << endl;
	top10Books("2017-01-01", "2018-01-02");
	return 0;
}
