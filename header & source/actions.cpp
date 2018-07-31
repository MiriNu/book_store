#include "actions.h"

//1. Show all available books in inventory
int showBooks() {
	Database &db = Database::getInstance();
	Connection *con = db.getConnection();
	PreparedStatement *pstmt = con->prepareStatement("SELECT * "
													 "FROM books "
													 "LEFT JOIN inventory ON books.book_id = inventory.book_id "
													 "WHERE inventory.amount > 0;");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no books in the inventory" << endl;
	}
	else {
		cout << "Book ID\tTitle\tAuthor\tAmount" << endl;
		while (rset->next()) {
			cout << rset->getUInt("book_id") << "\t" << rset->getString("title") << "\t" << rset->getString("author_name") << "\t" << rset->getString("amount") << endl;
		}
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//2. Show all open orders
int showOrders() {
	Database &db = Database::getInstance();
	Connection *con = db.getConnection();
	PreparedStatement *pstmt = con->prepareStatement("SELECT order_id, order_date, cust_id, supp_id, book_id, title, amount, tot_price, stat_type "
													 "FROM ( "
													 "	SELECT order_id, order_date, cust_id, supp_id, books.book_id, title, amount, tot_price, stat_type, stat_order.status_id "
													 "	FROM ( "
													 "		SELECT order_id, order_date, cust_id, supp_id, book_id, amount, tot_price, stat_type, orders.status_id "
													 "		FROM orders "
													 "		LEFT JOIN order_status ON orders.status_id = order_status.status_id "
													 "	) AS stat_order "
													 "	LEFT JOIN books ON books.book_id = stat_order.book_id "
													 ") AS order_info "
													 "WHERE(cust_id AND status_id != 5 AND status_id != 6) OR(cust_id IS NULL AND status_id != 3 AND status_id != 6);");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no open orders" << endl;
	}
	else {
		cout << "Order ID\tOrder Date\tCustomer ID\tSupplier ID\tBook ID\tTitle\tAmount\tTotal Price\tStatus" << endl;
		while (rset->next()) {
			cout << rset->getUInt("order_id") << "\t" << rset->getString("order_date") << "\t" << rset->getUInt("cust_id") << "\t" << rset->getUInt("supp_id") << "\t" << rset->getUInt("book_id") << "\t" << rset->getString("title") << "\t" << rset->getUInt("amount") << "\t" << rset->getDouble("tot_price") << "\t" << rset->getString("stat_type") << endl;
		}
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//3. List of all customers who made a purchase
int showCustomers() {
	Database &db = Database::getInstance();
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
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no customers who made a purchase" << endl;
	}
	else {
		cout << "Customer ID\tFirst name\tLast name\tPhone" << endl;
		while (rset->next()) {
			cout << rset->getUInt("cust_id") << "\t" << rset->getString("first_name") << "\t" << rset->getString("last_name") << "\t" << rset->getString("phone") << endl;
		}
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//4. Show all suppliers
int showSuppliers() {
	Database &db = Database::getInstance();
	Connection *con = db.getConnection();
	PreparedStatement *pstmt = con->prepareStatement("SELECT * "
													 "FROM suppliers;");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no suppliers" << endl;
	}
	else {
		cout << "Supplier ID\tSupplier name\tPhone\tBank account" << endl;
		while (rset->next()) {
			cout << rset->getUInt("supp_id") << "\t" << rset->getString("supp_name") << "\t" << rset->getString("phone") << "\t" << rset->getString("bank_acc") << endl;
		}
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//5. Show all purchases between given dates: fromDate & tilDate
int showPurchases(string fromDate, string tilDate) {
	Database &db = Database::getInstance();
	Connection *con = db.getConnection();
	PreparedStatement *pstmt = con->prepareStatement("SELECT * "
													 "FROM purchases "
													 "WHERE (purch_date BETWEEN '" + fromDate + "' AND '" + tilDate + "');");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no purchases in the given range of dates" << endl;
	}
	else {
		cout << "Purchase ID\tBook ID\tSeller ID\tCustomer ID\tPurchase date\tCanceled\tOriginal price\tCustomer payment" << endl;
		while (rset->next()) {
			cout << rset->getUInt("purch_id") << "\t" << rset->getUInt("book_id") << "\t" << rset->getUInt("seller_id") << "\t" << rset->getUInt("cust_id") << "\t" << rset->getString("purch_date") << "\t";
			if (rset->getBoolean("canceled")) {
				cout << "TRUE\t";
			}
			else {
				cout << "FALSE\t";
			}
			cout << rset->getDouble("origin_price") << "\t" << rset->getDouble("cust_pay") << endl;
		}
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//6. Show all books available for global sale
int showBooksOnSale() {
	Database &db = Database::getInstance();
	Connection *con = db.getConnection();
	PreparedStatement *pstmt = con->prepareStatement("SELECT book_id, title, author_name, original_price, disc_price "
													 "FROM ( "
													 "	SELECT books.book_id, title, author_name, price AS original_price, (price*(100 - discount) / 100) AS disc_price, discount "
													 "	FROM store_items "
													 "	LEFT JOIN books ON store_items.book_id = books.book_id "
													 ") AS books_info "
													 "WHERE discount > 0;");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no books on sale" << endl;
	}
	else {
		cout << "Book ID\tTitle\tAuthor name\tOriginal price\tDiscount price" << endl;
		while (rset->next()) {
			cout << rset->getUInt("book_id") << "\t" << rset->getString("title") << "\t" << rset->getString("author_name") << "\t" << rset->getDouble("original_price") << "\t" << rset->getDouble("disc_price") << endl;
		}
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//7. Check if a given book: bookTitle + bookAuthor is available in the inventory
int searchBook(string bookTitle, string bookAuthor) {
	Database &db = Database::getInstance();
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
		cout << "There is no book with matching title and author in the database, therefore the amount in the inventory is 0" << endl;
	}
	else {
		rset->next();
		cout << rset->getString("title") << " by " << rset->getString("author_name") << " has " << rset->getUInt("amount") << " copies in the inventory" << endl;
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//8. List of all suppliers of a given book: bookTitle + bookAuthor
int showSuppliersOfBook(string bookTitle, string bookAuthor) {
	Database &db = Database::getInstance();
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
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There is no book with matching title and author in the database" << endl;
	}
	else {
		cout << "Supplier ID\tSupplier name\tPhone\tBank account\tBook ID\tPrice" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("supp_id") << "\t" << rset->getString("supp_name") << "\t" << rset->getString("phone") << "\t" << rset->getString("bank_acc") << "\t" << rset->getUInt("book_id") << "\t" << rset->getDouble("price") << endl;
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//9. How many books from type: bookTitle + bookAuthor where sold from date fromDate
int booksSold(string bookTitle, string bookAuthor, string fromDate) {
	Database &db = Database::getInstance();
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
		cout << "No such book has been sold" << endl;
	}
	else {
		cout << bookTitle << " by " << bookAuthor << " sold " << rset->getInt("books_sold") << " copies since " << fromDate << endl;
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//10. How many books (different books too?) were purchased by customer: custID since given date fromDate
int booksPurchased(unsigned int custID, string fromDate) {
	Database &db = Database::getInstance();
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
		cout << "There are no purchases made by the customer" << endl;
	}
	else {
		cout << "The customer made " << rset->getInt("tot_books") << " purchases, and bought " << rset->getInt("diff_books") << " different books" << endl;
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//11. Show the customer details who bought the most since given date fromDate
int mostPurchasesCustomer(string fromDate) {
	Database &db = Database::getInstance();
	Connection *con = db.getConnection();
	PreparedStatement *pstmt = con->prepareStatement("SELECT * "
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
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no customers who made a purchase" << endl;
	}
	else {
		cout << "Customer ID\tFirst name\tLast name\tPhone\tAmount" << endl;
		while (rset->next()) {
			cout << rset->getUInt("cust_id") << "\t" << rset->getString("first_name") << "\t" << rset->getString("last_name") << "\t" << rset->getString("phone") << "\t" << rset->getUInt("book_amount") << endl;
		}
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//12. show the supplier details who sold us the most books since given date fromDate
int mostOrdersSuplier(string fromDate) {
	Database &db = Database::getInstance();
	Connection *con = db.getConnection();
	PreparedStatement *pstmt = con->prepareStatement("SELECT * "
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
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no orders from any supplier" << endl;
	}
	else {
		cout << "Supplier ID\tSupplier name\tPhone\tBank account\tAmount" << endl;
		while (rset->next()) {
			cout << rset->getUInt("supp_id") << "\t" << rset->getString("supp_name") << "\t" << rset->getString("phone") << "\t" << rset->getString("bank_acc") << "\t" << rset->getUInt("amount") << endl;
		}
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//13. amount of orders made (and how many books?) between given dates: fromDate & tilDate
int ordersMade(string fromDate, string tilDate) {
	Database &db = Database::getInstance();
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
		cout << "No orders were made" << endl;
	}
	else {
		cout << rset->getInt("orders_amount") << " orders were made, and " << rset->getInt("books_amount_ordered") << " books were ordered" << endl;
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//14. amount of orders made (and how many books?) between given dates: fromDate & tilDate that were made by customers sold
int ordersMadePurchases(string fromDate, string tilDate) {
	Database &db = Database::getInstance();
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
		cout << "No orders were made by customers" << endl;
	}
	else {
		cout << rset->getInt("orders_amount") << " orders were made, and " << rset->getInt("books_amount_ordered") << " books were ordered" << endl;
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//15. show the total discount a customer received since a certain date
int totalDiscountCustomer(unsigned int custID, string fromDate) {
	Database &db = Database::getInstance();
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
		cout << "The customer didn't made any purchases since the date" << endl;
	}
	else {
		cout << "The total discount the customer received is " << rset->getInt("total_disc") << endl;
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//16. sum revenue in Q1, Q2, Q3, Q4 in a given year: y
int sumRevenue(int y) {
	Database &db = Database::getInstance();
	Connection *con = db.getConnection();
	PreparedStatement *pstmt = con->prepareStatement("SELECT * "
													 "FROM ( "
													 "	SELECT * "
													 "	FROM ( "
													 "		SELECT SUM(cust_pay) AS tot_Q1 "
													 "		FROM purchases "
													 "		WHERE (purch_date BETWEEN '" + to_string(y) + "-01-01' AND '" + to_string(y) + "-03-31') AND canceled = false "
													 "	) AS Q1 "
													 "	LEFT JOIN ( "
													 "		SELECT * "
													 "		FROM ( "
													 "			SELECT SUM(cust_pay) AS tot_Q2 "
													 "			FROM purchases "
													 "			WHERE (purch_date BETWEEN '" + to_string(y) + "-04-01' AND '" + to_string(y) + "-06-30') AND canceled = false "
													 "		) AS Q2 "
													 "	) Q1Q2 ON(tot_Q1 OR tot_Q1 IS NULL) "
													 "	LEFT JOIN ( "
													 "		SELECT * "
													 "		FROM ( "
													 "			SELECT SUM(cust_pay) AS tot_Q3 "
													 "			FROM purchases "
													 "			WHERE (purch_date BETWEEN '" + to_string(y) + "-07-01' AND '" + to_string(y) + "-09-30') AND canceled = false"
													 "		) AS Q3 "
													 "	) Q1Q2Q3 ON(tot_Q1 OR tot_Q1 IS NULL) "
													 "	LEFT JOIN ( "
													 "		SELECT * "
													 "		FROM ( "
													 "			SELECT SUM(cust_pay) AS tot_Q4 "
													 "			FROM purchases "
													 "			WHERE (purch_date BETWEEN '" + to_string(y) + "-10-01' AND '" + to_string(y) + "-12-31') AND canceled = false "
													 "		) AS Q4 "
													 "	) Q1Q2Q3Q4 ON(tot_Q1 OR tot_Q1 IS NULL) "
													 ") AS revenue;");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	rset->next();
	if (rset->isNull("tot_Q1") && rset->isNull("tot_Q2") && rset->isNull("tot_Q3") && rset->isNull("tot_Q4")) {
		cout << "The store didn't operate in that year" << endl;
	}
	else {
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

//17. Show how many customers were added from given date: fromDate
int customersAdded(string fromDate) {
	Database &db = Database::getInstance();
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
		cout << "There are no new customers" << endl;
	}
	else {
		cout << "The number of new customers since " + fromDate + " is " << rset->getInt("new_custs") << endl;
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//18. total amount paid to a given supplier: suppID, between given dates: fromDate, tilDate
int totalPaidSupplier(unsigned int suppID, string fromDate, string tilDate) {
	Database &db = Database::getInstance();
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
		cout << "There were no orders from the supplier" << endl;
	}
	else {
		cout << "The total amount paid to the supplier is " << rset->getInt("tot_shekels") << endl;
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//19. total amount a given seller: sellID earned between given dates: fromDate, tilDate
int totalSoldSeller(unsigned int sellID, string fromDate, string tilDate) {
	Database &db = Database::getInstance();
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
		cout << "There are no purchases with that seller" << endl;
	}
	else {
		cout << "The total amount is " << rset->getInt("tot_shekels") << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("cust_id") << "\t" << rset->getString("first_name") << "\t" << rset->getString("last_name") << rset->getString("phone") << endl;
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//20. top 10 most sold books between given dates: fromDate, tilDate
int top10Books(string fromDate, string tilDate) {
	Database &db = Database::getInstance();
	Connection *con = db.getConnection();
	PreparedStatement *pstmt = con->prepareStatement("SELECT * "
													 "FROM ( "
													 "	SELECT books.book_id, title, author_name, amount "
													 "	FROM ( "
													 "		SELECT book_id, COUNT(*) AS amount "
													 "		FROM purchases "
													 "		WHERE purch_date BETWEEN '" + fromDate + "' AND '" + tilDate + "' "
													 "		GROUP BY book_id "
													 "	) AS count_amount "
													 "	LEFT JOIN books ON books.book_id = count_amount.book_id "
													 ") AS books_order "
													 "ORDER BY amount DESC "
													 "LIMIT 10;");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There were no purchase in that time" << endl;
	}
	else {
		cout << "Book ID\tTitle\tAuthor name\tAmount" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("book_id") << "\t" << rset->getString("title") << "\t" << rset->getString("author_name") << "\t" << rset->getString("amount") << endl;
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}
