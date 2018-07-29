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
		cout << "Book ID/tTitle/tAuthor" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("book_id") << "/t" << rset->getString("title") << "/t" << rset->getString("book_author") << endl;
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
													 "	SELECT * "
													 "	FROM ( "
													 "		SELECT * "
													 "		FROM orders "
													 "		LEFT JOIN order_status ON orders.status_id = order_status.status_id "
													 "		UNION "
													 "		SELECT * "
													 "		FROM orders "
													 "		RIGHT JOIN order_status ON orders.status_id = order_status.status_id "
													 "	) AS stat_order "
													 "	LEFT JOIN books ON books.book_id = stat_order.book_id "
													 "	UNION "
													 "	SELECT * "
													 "	FROM stat_order "
													 "	RIGHT JOIN books ON books.book_id = stat_order.book_id "
													 ") "
													 "WHERE	(cust_id != 0 AND status_id != 5 AND status_id != 6) OR (cust_id = 0 AND status_id != 3 AND status_id != 6);");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no open orders" << endl;
	}
	else {
		cout << "Order ID/tOrder Date/tCustomer ID/tSupplier ID/tBook ID/tTitle/tAmount/tTotal Price/tStatus" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("order_id") << "/t" << rset->getString("order_date") << "/t" << rset->getUInt("cust_id") << "/t" << rset->getUInt("supp_id") << "/t" << rset->getUInt("book_id") << "/t" << rset->getString("title") << "/t" << rset->getUInt("amount") << "/t" << rset->getDouble("tot_price") << "/t" << rset->getString("stat_type") << endl;
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
													 "RIGHT JOIN customers ON customers.cust_id = cust_who_bought.cust_id;");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no customers who made a purchase" << endl;
	}
	else {
		cout << "Customer ID/tFirst name/tLast name/tPhone" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("cust_id") << "/t" << rset->getString("first_name") << "/t" << rset->getString("last_name") << "/t" << rset->getString("phone") << endl;
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
		cout << "Supplier ID/tSupplier name/tPhone/tBank account" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("supp_id") << "/t" << rset->getString("supp_name") << "/t" << rset->getString("phone") << "/t" << rset->getString("bank_acc") << endl;
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
		cout << "Purchase ID/tBook ID/tSeller ID/tPurchase date/tCanceled/tOriginal price/tCustomer payment" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("purch_id") << "/t" << rset->getUInt("book_id") << "/t" << rset->getUInt("seller_id") << rset->getUInt("cust_id") << rset->getString("purch_date") << "/t";
		if (rset->getBoolean("canceled")) {
			cout << "TRUE/t";
		}
		else {
			cout << "FALSE/t";
		}
		cout << rset->getUInt("origin_price") << "/t" << rset->getUInt("cust_pay") << endl;
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
	PreparedStatement *pstmt = con->prepareStatement("SELECT book_id, title, author_name, price AS original_price, (price * (100 - discount) / 100) AS disc_price "
													 "FROM ( "
													 "	SELECT * "
													 "	FROM store_items "
													 "	LEFT JOIN books ON store_items.book_id = books.book_id "
													 "	UNION "
													 "	SELECT * "
													 "	FROM store_items "
													 "	RIGHT JOIN books ON store_items.book_id = books.book_id "
													 ") "
													 "WHERE discount > 0;");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no books on sale" << endl;
	}
	else {
		cout << "Book ID/tTitle/tAuthor name/tOriginal price/tDiscount price" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("book_id") << "/t" << rset->getString("title") << "/t" << rset->getString("author_name") << "/t" << rset->getString("original_price") << "/t" << rset->getString("disc_price") << endl;
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
	PreparedStatement *pstmt = con->prepareStatement("SELECT book_id, amount "
													 "FROM ( "
													 "	SELECT book_id "
													 "	FROM books "
													 "	WHERE title = '" + bookTitle + "' AND author_name = '" + bookAuthor + "' "
													 ") AS search_book "
													 "RIGHT JOIN inventory ON search_book.book_id = inventory.book_id;");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There is no book with matching title and author" << endl;
	}
	else {
		cout << "Book ID/tAmount" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("book_id") << "/t" << rset->getUInt("amount") << endl;
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
	PreparedStatement *pstmt = con->prepareStatement("SELECT * "
													 "FROM ( "
													 "	SELECT * "
													 "	FROM ( "
													 "		SELECT book_id "
													 "		FROM books "
													 "		WHERE title = '" + bookTitle + "' AND author_name = '" + bookAuthor + "' "
													 "	) AS search_book "
													 "	RIGHT JOIN book_prices ON search_book.book_id = book_prices.book_id "
													 ") AS supplied_books "
													 "RIGHT JOIN suppliers ON suppliers.supp_id = supplied_books.supp_id;");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no customers who made a purchase" << endl;
	}
	else {
		cout << "Supplier ID/tSupplier name/tPhone/tBank account/tBook ID/tPrice" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("supp_id") << "/t" << rset->getString("supp_name") << "/t" << rset->getString("phone") << "/t" << rset->getString("bank_acc") << rset->getUInt("book_id") << "/t" << rset->getDouble("price") << endl;
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
	PreparedStatement *pstmt = con->prepareStatement("SELECT COUNT(*) AS books_sold "
													 "FROM ( "
													 "	SELECT * "
													 "	FROM ( "
													 "		SELECT book_id "
													 "		FROM books "
													 "		WHERE title = '" + bookTitle + "' AND author_name = '" + bookAuthor + "' "
													 "	) AS search_book "
													 "	RIGHT JOIN purchases ON purchases.book_id = search_book.book_id "
													 "	WHERE purch_date >= '" + fromDate + "' AND canceled = false "
													 ");");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "No such book as been sold" << endl;
	}
	else {
		rset->next();
		cout << rset->getInt("books_sold") << " copies were sold" << endl;
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
													 "	WHERE cust_id = '" + custID + "' AND canceled = false AND purch_date >= '" + fromDate + "' "
													 ");");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no customers who made a purchase" << endl;
	}
	else {
		cout << "Customer ID/tFirst name/tLast name/tPhone" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("cust_id") << "/t" << rset->getString("first_name") << "/t" << rset->getString("last_name") << rset->getString("phone") << endl;
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
													 "	SELECT cust_id, MAX(book_amount) max_book_amount "
													 "	FROM ( "
													 "		SELECT cust_id, COUNT(cust_id) book_amount "
													 "		FROM ( "
													 "			SELECT * "
													 "			FROM purchases "
													 "			WHERE canceled = false AND purch_date >= '" + fromDate + "' "
													 "		) "
													 "		GROUP BY cust_id "
													 "	) AS custs_books "
													 ") AS top_cust "
													 "RIGHT JOIN customers ON customers.cust_id = top_cust.cust_id;");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no customers who made a purchase" << endl;
	}
	else {
		cout << "Customer ID/tFirst name/tLast name/tPhone" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("cust_id") << "/t" << rset->getString("first_name") << "/t" << rset->getString("last_name") << rset->getString("phone") << endl;
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
													 "	SELECT supp_id, max(amount) "
													 "	FROM ( "
													 "		SELECT supp_id, SUM(amount) AS amount "
													 "		FROM orders "
													 "		WHERE order_date >= '" + fromDate + "' AND  status_id != 1 "
													 "		GROUP BY supp_id "
													 "	) AS supps_amounts "
													 ") AS max_supp "
													 "RIGHT JOIN suppliers ON suppliers.supp_id = max_supp.supp_id;");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no customers who made a purchase" << endl;
	}
	else {
		cout << "Customer ID/tFirst name/tLast name/tPhone" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("cust_id") << "/t" << rset->getString("first_name") << "/t" << rset->getString("last_name") << rset->getString("phone") << endl;
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
													 ");");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no customers who made a purchase" << endl;
	}
	else {
		cout << "Customer ID/tFirst name/tLast name/tPhone" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("cust_id") << "/t" << rset->getString("first_name") << "/t" << rset->getString("last_name") << rset->getString("phone") << endl;
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
													 "	WHERE (order_date BETWEEN '" + fromDate + "' AND '" + tilDate + "') AND cust_id != 0 AND status_id = 5 "
													 ");");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no customers who made a purchase" << endl;
	}
	else {
		cout << "Customer ID/tFirst name/tLast name/tPhone" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("cust_id") << "/t" << rset->getString("first_name") << "/t" << rset->getString("last_name") << rset->getString("phone") << endl;
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
	PreparedStatement *pstmt = con->prepareStatement("SELECT SUM(discount) AS total_disc"
													 "FROM ( "
													 "	SELECT origin_price - cust_pay AS discount "
													 "	FROM ( "
													 "		SELECT * "
													 "		FROM purchases "
													 "		WHERE purch_date >= '" + fromDate + "' AND cust_id = '" + custID + "' AND canceled = false "
													 "	) "
													 ");");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no customers who made a purchase" << endl;
	}
	else {
		cout << "Customer ID/tFirst name/tLast name/tPhone" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("cust_id") << "/t" << rset->getString("first_name") << "/t" << rset->getString("last_name") << rset->getString("phone") << endl;
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
	PreparedStatement *pstmt = con->prepareStatement("SELECT cust_pay AS total_rev "
													 "FROM ( "
													 "	SELECT SUM(cust_pay) "
													 "	FROM purchases "
													 "	WHERE(purch_date BETWEEN '" + y + "-01-01' AND '" + y + "-03-31') AND canceled = false "
													 "	UNION "
													 "	SELECT SUM(cust_pay) "
													 "	FROM purchases "
													 "	WHERE(purch_date BETWEEN '" + y + "-04-01' AND '" + y + "-06-30') AND canceled = false "
													 "	UNION "
													 "	SELECT SUM(cust_pay) "
													 "	FROM purchases "
													 "	WHERE(purch_date BETWEEN '" + y + "-07-01' AND '" + y + "-09-30') AND canceled = false "
													 "	UNION "
													 "	SELECT SUM(cust_pay) "
													 "	FROM purchases "
													 "	WHERE(purch_date BETWEEN '" + y + "-10-01' AND '" + y + "-12-31') AND canceled = false "
													 ");");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no customers who made a purchase" << endl;
	}
	else {
		cout << "Customer ID/tFirst name/tLast name/tPhone" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("cust_id") << "/t" << rset->getString("first_name") << "/t" << rset->getString("last_name") << rset->getString("phone") << endl;
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
	PreparedStatement *pstmt = con->prepareStatement("SELECT COUNT(cust_id) "
													 "FROM ( "
													 "	SELECT cust_id "
													 "	FROM ( "
													 "		SELECT * "
													 "		FROM ( "
													 "			SELECT cust_id, MIN(purch_date) first_purch "
													 "			FROM purchases "
													 "			WHERE canceled = false "
													 "		) "
													 "		GROUP BY cust_id "
													 "	) "
													 "	WHERE first_purch >= '" + fromDate + "' "
													 ") AS custs "
													 "RIGHT JOIN customers ON customers.cust_id = custs.cust_id;");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no customers who made a purchase" << endl;
	}
	else {
		cout << "Customer ID/tFirst name/tLast name/tPhone" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("cust_id") << "/t" << rset->getString("first_name") << "/t" << rset->getString("last_name") << rset->getString("phone") << endl;
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
													 "	WHERE supp_id = '" + suppID + "' AND (order_date BETWEEN '" + fromDate + "' AND '" + tilDate + "') "
													 ");");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no customers who made a purchase" << endl;
	}
	else {
		cout << "Customer ID/tFirst name/tLast name/tPhone" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("cust_id") << "/t" << rset->getString("first_name") << "/t" << rset->getString("last_name") << rset->getString("phone") << endl;
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
													 "	WHERE seller_id = '" + sellID + "' AND (purch_date BETWEEN '" + fromDate + "' AND '" + tilDate + "') AND canceled = false "
													 ");");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no customers who made a purchase" << endl;
	}
	else {
		cout << "Customer ID/tFirst name/tLast name/tPhone" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("cust_id") << "/t" << rset->getString("first_name") << "/t" << rset->getString("last_name") << rset->getString("phone") << endl;
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
													 "	SELECT * "
													 "	FROM ( "
													 "		SELECT book_id, COUNT(*) AS amount "
													 "		FROM purchases "
													 "		WHERE purch_date BETWEEN '" + fromDate + "' AND '" + tilDate + "' "
													 "		GROUP BY book_id "
													 "	) "
													 "	ORDER BY amount DESC "
													 ") AS books_order "
													 "LEFT JOIN books ON books.book_id = books_order.book_id "
													 "LIMIT 10;");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no customers who made a purchase" << endl;
	}
	else {
		cout << "Customer ID/tFirst name/tLast name/tPhone" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("cust_id") << "/t" << rset->getString("first_name") << "/t" << rset->getString("last_name") << rset->getString("phone") << endl;
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}
