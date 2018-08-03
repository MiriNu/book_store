#include "actions.h"

//checks if the format of a string is yyyy-mm-tt, returns true if so and false otherwise
bool validateDateFormat(string date) {
	if (date.size() != 10) {
		return false;
	}
	for (int i = 0; i < 10; i++) {
		if ((i == 4 || i == 7) && date[i] != '-') {
			return false;
		}
		else if (i != 4 && i != 7 && (date[i] < '0' || date[i] > '9')) {
			return false;
		}
	}
	return true;
}

//checks the width of every item in the table and selects the width required for each column
void widths(ResultSet* rs, int* arr, int cul) {
	while (rs->next()) {
		for (int i = 1; i <= cul; i++) {
			if (arr[i - 1] < (rs->getString(i)).length() + 3) {
				arr[i - 1] = (rs->getString(i)).length() + 3;
			}
		}
	}
}

//1. Show all available books in inventory
int showBooks() {
	Database &db = Database::getInstance();
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT * "
														 "FROM books "
														 "LEFT JOIN inventory ON books.book_id = inventory.book_id "
														 "WHERE inventory.amount > 0;");
		ResultSet *rset = pstmt->executeQuery();
		int widthArr[4] = { 10, 8, 9, 9 };
		rset->beforeFirst();
		widths(rset, widthArr, 4);
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << endl << "There are no books in the inventory" << endl;
		}
		else {
			cout << endl;
			cout << left << setw(widthArr[0]) << setfill(' ') << "Book ID";
			cout << left << setw(widthArr[1]) << setfill(' ') << "Title";
			cout << left << setw(widthArr[2]) << setfill(' ') << "Author";
			cout << left << setw(widthArr[3]) << setfill(' ') << "Amount";
			cout << endl;
			while (rset->next()) {
				cout << left << setw(widthArr[0]) << setfill(' ') << rset->getUInt("book_id");
				cout << left << setw(widthArr[1]) << setfill(' ') << rset->getString("title");
				cout << left << setw(widthArr[2]) << setfill(' ') << rset->getString("author_name");
				cout << left << setw(widthArr[3]) << setfill(' ') << rset->getString("amount");
				cout << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//2. Show all open orders
int showOrders() {
	Database &db = Database::getInstance();
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT order_id, order_date, cust_id, first_name, suppliers.supp_id, supp_name, book_id, title, amount, tot_price, stat_type "
														 "FROM ( "
														 "	SELECT order_id, order_date, customers.cust_id, first_name, supp_id, book_id, title, amount, tot_price, stat_type "
														 "	FROM ( "
														 "		SELECT order_id, order_date, cust_id, supp_id, books.book_id, title, amount, tot_price, stat_type, stat_order.status_id "
														 "		FROM ( "
														 "			SELECT order_id, order_date, cust_id, supp_id, book_id, amount, tot_price, stat_type, orders.status_id "
														 "			FROM orders "
														 "			LEFT JOIN order_status ON orders.status_id = order_status.status_id "
														 "			WHERE (cust_id AND orders.status_id != 5 AND orders.status_id != 6) OR (cust_id IS NULL AND orders.status_id != 3 AND orders.status_id != 6) "
														 "		) AS stat_order "
														 "		LEFT JOIN books ON books.book_id = stat_order.book_id "
														 "	) AS book_info "
														 "	LEFT JOIN customers ON book_info.cust_id = customers.cust_id "
														 ") AS cust_info "
														 "LEFT JOIN suppliers ON cust_info.supp_id = suppliers.supp_id;");
		ResultSet *rset = pstmt->executeQuery();
		int widthArr[11] = { 11, 13, 14, 16, 14, 16, 10, 8, 9, 14, 9 };
		rset->beforeFirst();
		widths(rset, widthArr, 11);
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << endl << "There are no open orders" << endl;
		}
		else {
			cout << endl;
			cout << left << setw(widthArr[0]) << setfill(' ') << "Order ID";
			cout << left << setw(widthArr[1]) << setfill(' ') << "Order date";
			cout << left << setw(widthArr[2]) << setfill(' ') << "Customer ID";
			cout << left << setw(widthArr[3]) << setfill(' ') << "Customer name";
			cout << left << setw(widthArr[4]) << setfill(' ') << "Supplier ID";
			cout << left << setw(widthArr[5]) << setfill(' ') << "Supplier name";
			cout << left << setw(widthArr[6]) << setfill(' ') << "Book ID";
			cout << left << setw(widthArr[7]) << setfill(' ') << "Title";
			cout << left << setw(widthArr[8]) << setfill(' ') << "Amount";
			cout << left << setw(widthArr[9]) << setfill(' ') << "Total Price";
			cout << left << setw(widthArr[10]) << setfill(' ') << "Status";
			cout << endl;
			while (rset->next()) {
				cout << left << setw(widthArr[0]) << setfill(' ') << rset->getUInt("order_id");
				cout << left << setw(widthArr[1]) << setfill(' ') << rset->getString("order_date");
				cout << left << setw(widthArr[2]) << setfill(' ') << rset->getUInt("cust_id");
				cout << left << setw(widthArr[3]) << setfill(' ') << rset->getString("first_name");
				cout << left << setw(widthArr[4]) << setfill(' ') << rset->getUInt("supp_id");
				cout << left << setw(widthArr[5]) << setfill(' ') << rset->getString("supp_name");
				cout << left << setw(widthArr[6]) << setfill(' ') << rset->getUInt("book_id");
				cout << left << setw(widthArr[7]) << setfill(' ') << rset->getString("title");
				cout << left << setw(widthArr[8]) << setfill(' ') << rset->getUInt("amount");
				cout << left << setw(widthArr[9]) << setfill(' ') << rset->getDouble("tot_price");
				cout << left << setw(widthArr[10]) << setfill(' ') << rset->getString("stat_type");
				cout << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//3. List of all customers who made a purchase
int showCustomers() {
	Database &db = Database::getInstance();
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT * "
														 "FROM ( "
														 "	SELECT cust_id "
														 "	FROM purchases "
														 "	WHERE canceled = false "
														 "	GROUP BY cust_id "
														 ") AS cust_who_bought "
														 "LEFT JOIN customers ON customers.cust_id = cust_who_bought.cust_id;");
		ResultSet *rset = pstmt->executeQuery();
		int widthArr[4] = { 14, 13, 12, 8 };
		rset->beforeFirst();
		widths(rset, widthArr, 4);
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << endl << "There are no customers who made a purchase" << endl;
		}
		else {
			cout << endl;
			cout << left << setw(widthArr[0]) << setfill(' ') << "Customer ID";
			cout << left << setw(widthArr[1]) << setfill(' ') << "First name";
			cout << left << setw(widthArr[2]) << setfill(' ') << "Last name";
			cout << left << setw(widthArr[3]) << setfill(' ') << "Phone";
			cout << endl;
			while (rset->next()) {
				cout << left << setw(widthArr[0]) << setfill(' ') << rset->getUInt("cust_id");
				cout << left << setw(widthArr[1]) << setfill(' ') << rset->getString("first_name");
				cout << left << setw(widthArr[2]) << setfill(' ') << rset->getString("last_name");
				cout << left << setw(widthArr[3]) << setfill(' ') << rset->getString("phone");
				cout << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//4. Show all suppliers
int showSuppliers() {
	Database &db = Database::getInstance();
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT * "
														 "FROM suppliers;");
		ResultSet *rset = pstmt->executeQuery();
		int widthArr[4] = { 14, 16, 8, 15 };
		rset->beforeFirst();
		widths(rset, widthArr, 4);
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << endl << "There are no suppliers" << endl;
		}
		else {
			cout << endl;
			cout << left << setw(widthArr[0]) << setfill(' ') << "Supplier ID";
			cout << left << setw(widthArr[1]) << setfill(' ') << "Supplier name";
			cout << left << setw(widthArr[2]) << setfill(' ') << "Phone";
			cout << left << setw(widthArr[3]) << setfill(' ') << "Bank account";
			cout << endl;
			while (rset->next()) {
				cout << left << setw(widthArr[0]) << setfill(' ') << rset->getUInt("supp_id");
				cout << left << setw(widthArr[1]) << setfill(' ') << rset->getString("supp_name");
				cout << left << setw(widthArr[2]) << setfill(' ') << rset->getString("phone");
				cout << left << setw(widthArr[3]) << setfill(' ') << rset->getString("bank_acc");
				cout << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//5. Show all purchases between given dates: fromDate & tilDate
int showPurchases(string fromDate, string tilDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate) || !validateDateFormat(tilDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT purch_id, book_id, title, seller_id, seller_name, customers.cust_id, customers.first_name, purch_date, canceled, cust_pay "
														 "	FROM ( "
														 "		SELECT purch_id, book_id, title, sellers.seller_id, sellers.first_name seller_name, cust_id, purch_date, canceled, cust_pay "
														 "		FROM ( "
														 "			SELECT purch_id, purch.book_id, books.title, seller_id, cust_id, purch_date, canceled, cust_pay "
														 "			FROM ( "
														 "				SELECT purch_id, book_id, seller_id, cust_id, purch_date, IF(canceled = 0, 'No', 'Yes') AS canceled, origin_price, cust_pay "
														 "				FROM purchases "
														 "				WHERE (purch_date BETWEEN '" + fromDate + "' AND '" + tilDate + "') "
														 "			) AS purch "
														 "			LEFT JOIN books ON books.book_id = purch.book_id "
														 "		) AS answer "
														 "		LEFT JOIN sellers ON answer.seller_id = sellers.seller_id "
														 "	) AS ans_with_sellername "
														 "	LEFT JOIN customers ON customers.cust_id = ans_with_sellername.cust_id "
														 "	ORDER BY purch_id;");
		ResultSet *rset = pstmt->executeQuery();
		int widthArr[10] = { 14, 10, 8, 12, 14, 14, 16, 16, 11, 10 };
		rset->beforeFirst();
		widths(rset, widthArr, 10);
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << endl << "There are no purchases in the given range of dates" << endl;
		}
		else {
			cout << endl;
			cout << left << setw(widthArr[0]) << setfill(' ') << "Purchase ID";
			cout << left << setw(widthArr[1]) << setfill(' ') << "Book ID";
			cout << left << setw(widthArr[2]) << setfill(' ') << "Title";
			cout << left << setw(widthArr[3]) << setfill(' ') << "Seller ID";
			cout << left << setw(widthArr[4]) << setfill(' ') << "Seller name";
			cout << left << setw(widthArr[5]) << setfill(' ') << "Customer ID";
			cout << left << setw(widthArr[6]) << setfill(' ') << "Customer name";
			cout << left << setw(widthArr[7]) << setfill(' ') << "Purchase date";
			cout << left << setw(widthArr[8]) << setfill(' ') << "Canceled";
			cout << left << setw(widthArr[9]) << setfill(' ') << "Payment";
			cout << endl;
			while (rset->next()) {
				cout << left << setw(widthArr[0]) << setfill(' ') << rset->getUInt("purch_id");
				cout << left << setw(widthArr[1]) << setfill(' ') << rset->getUInt("book_id");
				cout << left << setw(widthArr[2]) << setfill(' ') << rset->getString("title");
				cout << left << setw(widthArr[3]) << setfill(' ') << rset->getUInt("seller_id");
				cout << left << setw(widthArr[4]) << setfill(' ') << rset->getString("seller_name");
				cout << left << setw(widthArr[5]) << setfill(' ') << rset->getUInt("cust_id");
				cout << left << setw(widthArr[6]) << setfill(' ') << rset->getString("first_name");
				cout << left << setw(widthArr[7]) << setfill(' ') << rset->getString("purch_date");
				cout << left << setw(widthArr[8]) << setfill(' ') << rset->getString("canceled");
				cout << left << setw(widthArr[9]) << setfill(' ') << rset->getDouble("cust_pay");
				cout << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//6. Show all books available for global sale
int showBooksOnSale() {
	Database &db = Database::getInstance();
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT book_id, title, author_name, original_price, disc_price "
														 "FROM ( "
														 "	SELECT books.book_id, title, author_name, price AS original_price, (price*(100 - discount) / 100) AS disc_price, discount "
														 "	FROM store_items "
														 "	LEFT JOIN books ON store_items.book_id = books.book_id "
														 ") AS books_info "
														 "WHERE discount > 0;");
		ResultSet *rset = pstmt->executeQuery();
		int widthArr[5] = { 9, 8, 14, 17, 17 };
		rset->beforeFirst();
		widths(rset, widthArr, 5);
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << endl << "There are no books on sale" << endl;
		}
		else {
			cout << endl;
			cout << left << setw(widthArr[0]) << setfill(' ') << "Book ID";
			cout << left << setw(widthArr[1]) << setfill(' ') << "Title";
			cout << left << setw(widthArr[2]) << setfill(' ') << "Author name";
			cout << left << setw(widthArr[3]) << setfill(' ') << "Original price";
			cout << left << setw(widthArr[4]) << setfill(' ') << "Discount price";
			cout << endl;
			while (rset->next()) {
				cout << left << setw(widthArr[0]) << setfill(' ') << rset->getUInt("book_id");
				cout << left << setw(widthArr[1]) << setfill(' ') << rset->getString("title");
				cout << left << setw(widthArr[2]) << setfill(' ') << rset->getString("author_name");
				cout << left << setw(widthArr[3]) << setfill(' ') << rset->getDouble("original_price");
				cout << left << setw(widthArr[4]) << setfill(' ') << rset->getDouble("disc_price");
				cout << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//7. Check if a given book: bookTitle + bookAuthor is available in the inventory
int searchBook(string bookTitle, string bookAuthor) {
	Database &db = Database::getInstance();
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT inventory.book_id, title, author_name, amount "
														 "FROM ( "
														 "	SELECT book_id, title, author_name "
														 "	FROM books "
														 "	WHERE title = '" + bookTitle + "' AND author_name = '" + bookAuthor + "' "
														 ") AS search_book "
														 "LEFT JOIN inventory ON search_book.book_id = inventory.book_id;");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << endl << "There is no book with matching title and author in the database, therefore the amount in the inventory is 0" << endl;
		}
		else {
			rset->next();
			cout << endl;
			cout << rset->getString("title") << " by " << rset->getString("author_name") << " has " << rset->getUInt("amount") << " copies in the inventory" << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//8. List of all suppliers of a given book: bookTitle + bookAuthor
int showSuppliersOfBook(string bookTitle, string bookAuthor) {
	Database &db = Database::getInstance();
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT suppliers.supp_id, supp_name, phone, bank_acc, book_id, price "
														 "FROM ( "
														 "	SELECT book_prices.book_id, supp_id, price "
														 "	FROM ( "
														 "		SELECT book_id "
														 "		FROM books "
														 "		WHERE title = '" + bookTitle + "' AND author_name = '" + bookAuthor + "' "
														 "	) AS search_book "
														 "	LEFT JOIN book_prices ON search_book.book_id = book_prices.book_id "
														 ") AS supplied_books "
														 "LEFT JOIN suppliers ON suppliers.supp_id = supplied_books.supp_id;");
		ResultSet *rset = pstmt->executeQuery();
		int widthArr[6] = { 14, 16, 8, 15, 10, 8 };
		rset->beforeFirst();
		widths(rset, widthArr, 6);
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << endl << "There is no book with matching title and author in the database" << endl;
		}
		else {
			cout << endl;
			cout << left << setw(widthArr[0]) << setfill(' ') << "Supplier ID";
			cout << left << setw(widthArr[1]) << setfill(' ') << "Supplier name";
			cout << left << setw(widthArr[2]) << setfill(' ') << "Phone";
			cout << left << setw(widthArr[3]) << setfill(' ') << "Bank account";
			cout << left << setw(widthArr[4]) << setfill(' ') << "Book ID";
			cout << left << setw(widthArr[5]) << setfill(' ') << "Price";
			cout << endl;
			while (rset->next()) {
				cout << left << setw(widthArr[0]) << setfill(' ') << rset->getUInt("supp_id");
				cout << left << setw(widthArr[1]) << setfill(' ') << rset->getString("supp_name");
				cout << left << setw(widthArr[2]) << setfill(' ') << rset->getString("phone");
				cout << left << setw(widthArr[3]) << setfill(' ') << rset->getString("bank_acc");
				cout << left << setw(widthArr[4]) << setfill(' ') << rset->getUInt("book_id");
				cout << left << setw(widthArr[5]) << setfill(' ') << rset->getDouble("price");
				cout << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//9. How many books from type: bookTitle + bookAuthor where sold from date fromDate
int booksSold(string bookTitle, string bookAuthor, string fromDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT COUNT(book_id) AS books_sold, title, author_name "
														 "FROM ( "
														 "	SELECT purch_id, purchases.book_id, title, author_name, purch_date, canceled "
														 "	FROM ( "
														 "		SELECT book_id, title, author_name "
														 "		FROM books "
														 "		WHERE title = '" + bookTitle + "' AND author_name = '" + bookAuthor + "' "
														 "	) AS search_book "
														 "	LEFT JOIN purchases ON purchases.book_id = search_book.book_id "
														 "	WHERE purch_date >= '" + fromDate + "' AND canceled = false "
														 ") AS books_purch;");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->isNull("title") || rset->isNull("author_name")) {
			cout << endl << "No such book has been sold" << endl;
		}
		else {
			cout << endl;
			cout << rset->getString("title") << " by " << rset->getString("author_name") << " sold " << rset->getInt("books_sold") << " copies since " << fromDate << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//10. How many books (different books too?) were purchased by customer: custID since given date fromDate
int booksPurchased(unsigned int custID, string fromDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT COUNT(book_id) AS tot_books, COUNT(DISTINCT book_id) AS diff_books "
														 "FROM ( "
														 "	SELECT * "
														 "	FROM purchases "
														 "	WHERE cust_id = '" + to_string(custID) + "' AND canceled = false AND purch_date >= '" + fromDate + "' "
														 ") AS cust_purch;");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->getInt("tot_books") == 0) {
			cout << endl << "There are no purchases made by the customer" << endl;
		}
		else {
			cout << endl;
			cout << "The customer made " << rset->getInt("tot_books") << " purchases, and bought " << rset->getInt("diff_books") << " different books" << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//11. Show the customer details who bought the most since given date fromDate
int mostPurchasesCustomer(string fromDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT top_cust.cust_id, book_amount, first_name, last_name, phone "
														 "FROM ( "
														 "	SELECT * "
														 "	FROM ( "
														 "		SELECT cust_id, COUNT(cust_id) book_amount "
														 "		FROM ( "
														 "			SELECT * "
														 "			FROM purchases "
														 "			WHERE canceled = false AND purch_date >= '" + fromDate + "' "
														 "		) AS purch "
														 "		GROUP BY cust_id "
														 "	) AS custs_books "
														 "	ORDER BY book_amount DESC "
														 "	LIMIT 1 "
														 ") AS top_cust "
														 "LEFT JOIN customers ON customers.cust_id = top_cust.cust_id;");
		ResultSet *rset = pstmt->executeQuery();
		int widthArr[5] = { 14, 13, 12, 8, 9 };
		rset->beforeFirst();
		widths(rset, widthArr, 5);
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << endl << "There are no customers who made a purchase" << endl;
		}
		else {
			cout << endl;
			cout << left << setw(widthArr[0]) << setfill(' ') << "Customer ID";
			cout << left << setw(widthArr[1]) << setfill(' ') << "First name";
			cout << left << setw(widthArr[2]) << setfill(' ') << "Last name";
			cout << left << setw(widthArr[3]) << setfill(' ') << "Phone";
			cout << left << setw(widthArr[4]) << setfill(' ') << "Amount";
			cout << endl;
			while (rset->next()) {
				cout << left << setw(widthArr[0]) << setfill(' ') << rset->getUInt("cust_id");
				cout << left << setw(widthArr[1]) << setfill(' ') << rset->getString("first_name");
				cout << left << setw(widthArr[2]) << setfill(' ') << rset->getString("last_name");
				cout << left << setw(widthArr[3]) << setfill(' ') << rset->getString("phone");
				cout << left << setw(widthArr[4]) << setfill(' ') << rset->getUInt("book_amount");
				cout << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//12. show the supplier details who sold us the most books since given date fromDate
int mostOrdersSuplier(string fromDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT amount, max_supp.supp_id, supp_name, phone, bank_acc "
														 "FROM ( "
														 "	SELECT * "
														 "	FROM ( "
														 "		SELECT supp_id, SUM(amount) AS amount "
														 "		FROM orders "
														 "		WHERE order_date >= '" + fromDate + "' AND  status_id != 1 "
														 "		GROUP BY supp_id "
														 "	) AS supps_amounts "
														 "	ORDER BY amount DESC "
														 "	LIMIT 1 "
														 ") AS max_supp "
														 "LEFT JOIN suppliers ON suppliers.supp_id = max_supp.supp_id;");
		ResultSet *rset = pstmt->executeQuery();
		int widthArr[5] = { 14, 16, 8, 15, 9 };
		rset->beforeFirst();
		widths(rset, widthArr, 5);
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << endl << "There are no orders from any supplier" << endl;
		}
		else {
			cout << endl;
			cout << left << setw(widthArr[0]) << setfill(' ') << "Supplier ID";
			cout << left << setw(widthArr[1]) << setfill(' ') << "Supplier name";
			cout << left << setw(widthArr[2]) << setfill(' ') << "Phone";
			cout << left << setw(widthArr[3]) << setfill(' ') << "Bank account";
			cout << left << setw(widthArr[4]) << setfill(' ') << "Amount";
			cout << endl;
			while (rset->next()) {
				cout << left << setw(widthArr[0]) << setfill(' ') << rset->getUInt("supp_id");
				cout << left << setw(widthArr[1]) << setfill(' ') << rset->getString("supp_name");
				cout << left << setw(widthArr[2]) << setfill(' ') << rset->getString("phone");
				cout << left << setw(widthArr[3]) << setfill(' ') << rset->getString("bank_acc");
				cout << left << setw(widthArr[4]) << setfill(' ') << rset->getUInt("amount");
				cout << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//13. amount of orders made (and how many books?) between given dates: fromDate & tilDate
int ordersMade(string fromDate, string tilDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate) || !validateDateFormat(tilDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT COUNT(order_id) orders_amount, SUM(amount) books_amount_ordered "
														 "FROM ( "
														 "	SELECT * "
														 "	FROM orders "
														 "	WHERE (order_date BETWEEN '" + fromDate + "' AND '" + tilDate + "') AND status_id != 1 "
														 ") AS orders_range;");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->getInt("orders_amount") == 0) {
			cout << endl << "No orders were made" << endl;
		}
		else {
			cout << endl;
			cout << rset->getInt("orders_amount") << " orders were made, and " << rset->getInt("books_amount_ordered") << " books were ordered" << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//14. amount of orders made (and how many books?) between given dates: fromDate & tilDate that were made by customers sold
int ordersMadePurchases(string fromDate, string tilDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate) || !validateDateFormat(tilDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT COUNT(order_id) orders_amount, SUM(amount) books_amount_ordered "
														 "FROM ( "
														 "	SELECT * "
														 "	FROM orders "
														 "	WHERE (order_date BETWEEN '" + fromDate + "' AND '" + tilDate + "') AND cust_id AND status_id = 5 "
														 ") AS orders_range;");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->getInt("orders_amount") == 0) {
			cout << endl << "No orders were made by customers" << endl;
		}
		else {
			cout << endl;
			cout << rset->getInt("orders_amount") << " orders were made, and " << rset->getInt("books_amount_ordered") << " books were ordered" << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//15. show the total discount a customer received since a certain date
int totalDiscountCustomer(unsigned int custID, string fromDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT SUM(discount) AS total_disc "
														 "FROM ( "
														 "	SELECT origin_price - cust_pay AS discount "
														 "	FROM ( "
														 "		SELECT * "
														 "		FROM purchases "
														 "		WHERE purch_date >= '" + fromDate + "' AND cust_id = '" + to_string(custID) + "' AND canceled = false "
														 "	) AS purch_disc "
														 ") AS disc_sum;");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->isNull("total_disc")) {
			cout << endl << "The customer didn't made any purchases since the date" << endl;
		}
		else {
			cout << endl;
			cout << "The total discount the customer received is " << rset->getInt("total_disc") << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//16. sum revenue in Q1, Q2, Q3, Q4 in a given year: y
int sumRevenue(string y) {
	Database &db = Database::getInstance();
	if (y.size() != 4) {
		cout << "The year doesn't follow the right format - it should be 4 digits" << endl;
		return 1;
	}
	for (int i = 0; i < 4; i++) {
		if (y[i] < '0' || y[i] > '9') {
			cout << "The year doesn't follow the right format - it should be 4 digits" << endl;
			return 1;
		}
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT * "
														 "FROM ( "
														 "	SELECT * "
														 "	FROM ( "
														 "		SELECT SUM(cust_pay) AS tot_Q1 "
														 "		FROM purchases "
														 "		WHERE (purch_date BETWEEN '" + y + "-01-01' AND '" + y + "-03-31') AND canceled = false "
														 "	) AS Q1 "
														 "	LEFT JOIN ( "
														 "		SELECT * "
														 "		FROM ( "
														 "			SELECT SUM(cust_pay) AS tot_Q2 "
														 "			FROM purchases "
														 "			WHERE (purch_date BETWEEN '" + y + "-04-01' AND '" + y + "-06-30') AND canceled = false "
														 "		) AS Q2 "
														 "	) Q1Q2 ON(tot_Q1 OR tot_Q1 IS NULL) "
														 "	LEFT JOIN ( "
														 "		SELECT * "
														 "		FROM ( "
														 "			SELECT SUM(cust_pay) AS tot_Q3 "
														 "			FROM purchases "
														 "			WHERE (purch_date BETWEEN '" + y + "-07-01' AND '" + y + "-09-30') AND canceled = false"
														 "		) AS Q3 "
														 "	) Q1Q2Q3 ON(tot_Q1 OR tot_Q1 IS NULL) "
														 "	LEFT JOIN ( "
														 "		SELECT * "
														 "		FROM ( "
														 "			SELECT SUM(cust_pay) AS tot_Q4 "
														 "			FROM purchases "
														 "			WHERE (purch_date BETWEEN '" + y + "-10-01' AND '" + y + "-12-31') AND canceled = false "
														 "		) AS Q4 "
														 "	) Q1Q2Q3Q4 ON(tot_Q1 OR tot_Q1 IS NULL) "
														 ") AS revenue;");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->isNull("tot_Q1") && rset->isNull("tot_Q2") && rset->isNull("tot_Q3") && rset->isNull("tot_Q4")) {
			cout << endl << "The store didn't operate in that year" << endl;
		}
		else {
			cout << endl;
			cout << "the total revenue is:" << endl;
			for (unsigned int i = 1; i <= 4; i++) {
				cout << "Q" << i << ": ";
				if (rset->isNull(i)) {
					cout << "0" << endl;
				}
				else {
					cout << rset->getUInt(i) << endl;
				}
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//17. Show how many customers were added from given date: fromDate
int customersAdded(string fromDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT COUNT(cust_id) AS new_custs "
														 "FROM ( "
														 "	SELECT * "
														 "	FROM ( "
														 "		SELECT * "
														 "		FROM ( "
														 "			SELECT cust_id, purch_date "
														 "			FROM purchases "
														 "			WHERE canceled = false "
														 "		) AS cust_purch_date "
														 "		ORDER BY purch_date ASC "
														 "	) AS ordered_date "
														 "	GROUP BY cust_id "
														 ") AS count_cust "
														 "WHERE purch_date >= '" + fromDate + "';");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->getUInt("new_custs") == 0) {
			cout << endl << "There are no new customers" << endl;
		}
		else {
			cout << endl;
			cout << "The number of new customers since " + fromDate + " is " << rset->getInt("new_custs") << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//18. total amount paid to a given supplier: suppID, between given dates: fromDate, tilDate
int totalPaidSupplier(unsigned int suppID, string fromDate, string tilDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate) || !validateDateFormat(tilDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT SUM(tot_price) AS tot_shekels "
														 "FROM ( "
														 "	SELECT * "
														 "	FROM orders "
														 "	WHERE supp_id = '" + to_string(suppID) + "' AND (order_date BETWEEN '" + fromDate + "' AND '" + tilDate + "') AND status_id != 1 AND status_id != 6 "
														 ") AS supp_orders;");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->isNull("tot_shekels")) {
			cout << endl << "There were no orders from the supplier" << endl;
		}
		else {
			cout << endl;
			cout << "The total amount paid to the supplier is " << rset->getInt("tot_shekels") << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//19. total amount a given seller: sellID earned between given dates: fromDate, tilDate
int totalSoldSeller(unsigned int sellID, string fromDate, string tilDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate) || !validateDateFormat(tilDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT SUM(cust_pay) AS tot_shekels "
														 "FROM ( "
														 "	SELECT * "
														 "	FROM purchases "
														 "	WHERE seller_id = '" + to_string(sellID) + "' AND (purch_date BETWEEN '" + fromDate + "' AND '" + tilDate + "') AND canceled = false "
														 ") AS seller_purch;");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->isNull("tot_shekels")) {
			cout << endl << "There are no purchases with that seller" << endl;
		}
		else {
			cout << endl;
			cout << "The total amount is " << rset->getInt("tot_shekels") << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//20. top 10 most sold books between given dates: fromDate, tilDate
int top10Books(string fromDate, string tilDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate) || !validateDateFormat(tilDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT * "
														 "FROM ( "
														 "	SELECT books.book_id, title, author_name, amount "
														 "	FROM ( "
														 "		SELECT book_id, COUNT(*) AS amount "
														 "		FROM purchases "
														 "		WHERE (purch_date BETWEEN '" + fromDate + "' AND '" + tilDate + "') AND canceled = false "
														 "		GROUP BY book_id "
														 "	) AS count_amount "
														 "	LEFT JOIN books ON books.book_id = count_amount.book_id "
														 ") AS books_order "
														 "ORDER BY amount DESC "
														 "LIMIT 10;");
		ResultSet *rset = pstmt->executeQuery();
		int widthArr[4] = { 10, 8, 9, 9 };
		rset->beforeFirst();
		widths(rset, widthArr, 4);
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << endl << "There were no purchase in that time" << endl;
		}
		else {
			cout << endl;
			cout << left << setw(widthArr[0]) << setfill(' ') << "Book ID";
			cout << left << setw(widthArr[1]) << setfill(' ') << "Title";
			cout << left << setw(widthArr[2]) << setfill(' ') << "Author";
			cout << left << setw(widthArr[3]) << setfill(' ') << "Amount";
			cout << endl;
			while (rset->next()) {
				cout << left << setw(widthArr[0]) << setfill(' ') << rset->getUInt("book_id");
				cout << left << setw(widthArr[1]) << setfill(' ') << rset->getString("title");
				cout << left << setw(widthArr[2]) << setfill(' ') << rset->getString("author_name");
				cout << left << setw(widthArr[3]) << setfill(' ') << rset->getString("amount");
				cout << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}
