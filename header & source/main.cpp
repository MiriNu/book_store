#include <iostream>
#include <mysql_connection.h>
#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <string>
#include <limits>
#include "actions.h"
#include "db.h"

#define NAMESPACES using namespace boost::posix_time;using namespace boost::gregorian;using namespace std;using namespace sql;

NAMESPACES;

void printMenue() {
	cout << endl;
	cout << "Enter the number of the action you wish to take" << endl;
	cout << "1. Show all available books in inventory" << endl;
	cout << "2. Show all open orders" << endl;
	cout << "3. Show all customers who made a purchase" << endl;
	cout << "4. Show all suppliers" << endl;
	cout << "5. Show all purchases between given dates" << endl;
	cout << "6. Show all books available for global sale" << endl;
	cout << "7. Check if a given book is available in the inventory" << endl;
	cout << "8. Show all suppliers of a given book" << endl;
	cout << "9. How many books from a certain type where sold from since a given date" << endl;
	cout << "10. How many books and different book types were purchased by a certain customer since given date" << endl;
	cout << "11. Show the details of the customer who bought the most since a given date" << endl;
	cout << "12. Show the details of the supplier who sold us the most books since a given date" << endl;
	cout << "13. Amount of orders made and how many books ordered between given dates" << endl;
	cout << "14. Amount of orders made and how many books ordered between given dates that were made by customers and were sold" << endl;
	cout << "15. Show the total discount a customer received since a certain date" << endl;
	cout << "16. Sum revenue in Q1, Q2, Q3, Q4 in a given year" << endl;
	cout << "17. Show how many customers were added since a given date" << endl;
	cout << "18. Total amount paid to a given supplier between given dates" << endl;
	cout << "19. Total amount a given seller earned between given dates" << endl;
	cout << "20. Show the top 10 most sold books between given dates" << endl;
	cout << "99. Exit" << endl;
	cout << endl;
}

void waitToContinue() {
	cout << endl;
	cout << "Press ENTER to continue" << endl;
	cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
}

int main(int argc, const char * argv[]) {
	string fromD, tilD, title, author;
	unsigned int id;
	int num;

	while (1) {
		printMenue();
		cin >> num;
		switch (num) {
		case 1:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			showBooks();
			break;
		case 2:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			showOrders();
			break;
		case 3:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			showCustomers();
			break;
		case 4:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			showSuppliers();
			break;
		case 5:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Enter the start date in the format yyyy-mm-tt" << endl;
			cin >> fromD;
			cout << "Enter the end date in the format yyyy-mm-tt" << endl;
			cin >> tilD;
			showPurchases(fromD, tilD);
			break;
		case 6:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			showBooksOnSale();
			break;
		case 7:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Enter the book's title" << endl;
			cin >> title;
			cout << "Enter the book's author" << endl;
			cin >> author;
			searchBook(title, author);
			break;
		case 8:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Enter the book's title" << endl;
			cin >> title;
			cout << "Enter the book's author" << endl;
			cin >> author;
			showSuppliersOfBook(title, author);
			break;
		case 9:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Enter the book's title" << endl;
			cin >> title;
			cout << "Enter the book's author" << endl;
			cin >> author;
			cout << "Enter the start date in the format yyyy-mm-tt" << endl;
			cin >> fromD;
			booksSold(title, author, fromD);
			break;
		case 10:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Enter the customers's ID" << endl;
			cin >> id;
			cout << "Enter the start date in the format yyyy-mm-tt" << endl;
			cin >> fromD;
			booksPurchased(id, fromD);
			break;
		case 11:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Enter the start date in the format yyyy-mm-tt" << endl;
			cin >> fromD;
			mostPurchasesCustomer(fromD);
			break;
		case 12:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Enter the start date in the format yyyy-mm-tt" << endl;
			cin >> fromD;
			mostOrdersSuplier(fromD);
			break;
		case 13:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Enter the start date in the format yyyy-mm-tt" << endl;
			cin >> fromD;
			cout << "Enter the end date in the format yyyy-mm-tt" << endl;
			cin >> tilD;
			ordersMade(fromD, tilD);
			break;
		case 14:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Enter the start date in the format yyyy-mm-tt" << endl;
			cin >> fromD;
			cout << "Enter the end date in the format yyyy-mm-tt" << endl;
			cin >> tilD;
			ordersMadePurchases(fromD, tilD);
			break;
		case 15:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Enter the customers's ID" << endl;
			cin >> id;
			cout << "Enter the start date in the format yyyy-mm-tt" << endl;
			cin >> fromD;
			totalDiscountCustomer(id, fromD);
			break;
		case 16:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Enter the year in the format yyyy";
			cin >> fromD;
			sumRevenue(fromD);
			break;
		case 17:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Enter the start date in the format yyyy-mm-tt" << endl;
			cin >> fromD;
			customersAdded(fromD);
			break;
		case 18:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Enter the supplier's ID" << endl;
			cin >> id;
			cout << "Enter the start date in the format yyyy-mm-tt" << endl;
			cin >> fromD;
			cout << "Enter the end date in the format yyyy-mm-tt" << endl;
			cin >> tilD;
			totalPaidSupplier(id, fromD, tilD);
			break;
		case 19:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Enter the seller's ID" << endl;
			cin >> id;
			cout << "Enter the start date in the format yyyy-mm-tt" << endl;
			cin >> fromD;
			cout << "Enter the end date in the format yyyy-mm-tt" << endl;
			cin >> tilD;
			totalSoldSeller(id, fromD, tilD);
			break;
		case 20:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Enter the start date in the format yyyy-mm-tt" << endl;
			cin >> fromD;
			cout << "Enter the end date in the format yyyy-mm-tt" << endl;
			cin >> tilD;
			top10Books(fromD, tilD);
			break;
		case 99:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << endl << "Bye" << endl;
			return 0;
			break;
		default:
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Not a valid input" << endl;
			break;
		}
		waitToContinue();
	}
}
