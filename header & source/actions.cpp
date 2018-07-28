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
													 "FROM (	SELECT * "
													 "			FROM (	SELECT * "
													 "					FROM orders "
													 "					LEFT JOIN order_status ON orders.status_id = order_status.status_id "
													 "					UNION "
													 "					SELECT * "
													 "					FROM orders "
													 "					RIGHT JOIN order_status ON orders.status_id = order_status.status_id) AS stat_order "
													 "			LEFT JOIN books ON books.book_id = stat_order.book_id "
													 "			UNION "
													 "			SELECT * "
													 "			FROM stat_order "
													 "			RIGHT JOIN books ON books.book_id = stat_order.book_id) "
													 "WHERE	(cust_id != 0 AND status_id != 5 AND status_id != 6) OR "
													 "		(cust_id = 0 AND status_id != 3 AND status_id != 6);");
	ResultSet *rset = pstmt->executeQuery();
	rset->beforeFirst();
	if (rset->rowsCount() == 0) {
		cout << "There are no open orders" << endl;
	}
	else {
		cout << "Order ID/tOrder Date/tCustomer ID/tSupplier ID/tBook ID/tTitle/tAmount/tTotal Price/tStatus" << endl;
	}
	while (rset->next()) {
		cout << rset->getUInt("order_id") << "/t" << rset->getString("order_date") << "/t" << rset->getUInt("cust_id") << rset->getUInt("supp_id") << rset->getUInt("book_id") << rset->getString("title") << rset->getUInt("amount") << rset->getDouble("tot_price") << rset->getString("stat_type") << endl;
	}
	delete pstmt;
	delete rset;
	delete con;
	return 0;
}

//3. List of all customers who made a purchase
int showCustomers() {
	SELECT *
		FROM
		(SELECT cust_id
			FROM purchases
			WHERE canceled = false
			GROUP BY cust_id) cust_who_bought
		RIGHT JOIN customers ON customers.cust_id = cust_who_bought.cust_id;
}

//4. Show all suppliers
int showSuppliers() {
	SELECT *
		FROM suppliers;
}

//5. Show all purchases between given dates: fromDate & tilDate
int showPurchases(date fromDate, date tilDate) {
	SELECT *
		FROM purchases
		WHERE(purch_date BETWEEN 'fromDate' AND 'tilDate');
}

//6. Show all books available for global sale
int showBooksOnSale() {
	SELECT book_id, title, author_name, price AS original_price, (price*(100 - discount)) AS disc_price
		FROM
		(SELECT *
			FROM store_items
			LEFT JOIN books ON store_items.book_id = books.book_id
			UNION
			SELECT *
			FROM store_items
			RIGHT JOIN books ON store_items.book_id = books.book_id)
		WHERE discount > 0;
}

//7. Check if a given book: bookTitle + bookAuthor is available in the inventory
int searchBook(string bookTitle, string bookAuthor) {
	SELECT book_id, amount
		FROM
		(SELECT book_id
			FROM books
			WHERE title = 'bookTitle' AND author_name = 'bookAuthor') AS search_book
		RIGHT JOIN inventory ON search_book.book_id = inventory.book_id;
}

//8. List of all suppliers of a given book: bookTitle + bookAuthor
int showSuppliersOfBook(string bookTitle, string bookAuthor) {
	SELECT *
		FROM
		(SELECT *
			FROM
			(SELECT book_id
				FROM books
				WHERE title = 'bookTitle' AND author_name = 'bookAuthor') AS search_book
			RIGHT JOIN book_prices ON search_book.book_id = book_prices.book_id) AS supplied_books
		RIGHT JOIN suppliers ON suppliers.supp_id = supplied_books.supp_id;
}

//9. How many books from type: bookTitle + bookAuthor where sold from date fromDate
int booksSold(string bookTitle, string bookAuthor, date fromDate) {
	SELECT COUNT(*) AS books_sold
		FROM
		(SELECT *
			FROM
			(SELECT book_id
				FROM books
				WHERE title = 'bookTitle' AND author_name = 'bookAuthor') AS search_book
			RIGHT JOIN purchases ON purchases.book_id = search_book.book_id
			WHERE purch_date >= 'fromDate' AND canceled = false);
}

//10. How many books (different books too?) were purchased by customer: custID since given date fromDate
int booksPurchased(unsigned int custID, date fromDate) {
	SELECT COUNT(book_id) AS tot_books, COUNT(DISTINCT book_id) AS diff_books
		FROM
		(SELECT *
			FROM purchases
			WHERE cust_id = 'custID' AND canceled = false AND purch_date >= 'fromDate');
}

//11. Show the customer details who bought the most since given date fromDate
int mostPurchasesCustomer(date fromDate) {
	SELECT *
		FROM
		(SELECT cust_id, MAX(book_amount) max_book_amount
			FROM
			(SELECT cust_id, COUNT(cust_id) book_amount
				FROM
				(SELECT *
					FROM purchases
					WHERE canceled = false AND purch_date >= 'fromDate')
				GROUP BY cust_id) AS custs_books) AS top_cust
		RIGHT JOIN customers ON customers.cust_id = top_cust.cust_id;
}

//12. show the supplier details who sold us the most books since given date fromDate
int mostOrdersSuplier(date fromDate) {
	SELECT *
		FROM
		(SELECT supp_id, max(amount)
			FROM
			(SELECT supp_id, SUM(amount) AS amount
				FROM orders
				WHERE order_date >= 'fromDate' AND  status_id != 1
				GROUP BY supp_id) AS supps_amounts) AS max_supp
		RIGHT JOIN suppliers ON suppliers.supp_id = max_supp.supp_id;
}

//13. amount of orders made (and how many books?) between given dates: fromDate & tilDate
int ordersMade(date fromDate, date tilDate) {
	SELECT COUNT(order_id) orders_amount, SUM(amount) books_amount_ordered
		FROM
		(SELECT *
			FROM orders
			WHERE(order_date BETWEEN 'fromDate' AND 'tilDate') AND status_id != 1);
}

//14. amount of orders made (and how many books?) between given dates: fromDate & tilDate that were made by customers sold
int ordersMadePurchases(date fromDate, date tilDate) {
	SELECT COUNT(order_id) orders_amount, SUM(amount) books_amount_ordered
		FROM
		(SELECT *
			FROM orders
			WHERE(order_date BETWEEN 'fromDate' AND 'tilDate') AND cust_id != 0 AND status_id = 5);
}

//15. show the total discount a customer received since a certain date
int totalDiscountCustomer(unsigned int custID, date fromDate) {
	SELECT SUM(discount) AS total_disc
		FROM
		(SELECT origin_price - cust_pay AS discount
			FROM
			(SELECT *
				FROM purchases
				WHERE purch_date >= 'fromDate' AND cust_id = 'custID' AND cancled = false));
}

//16. sum revenue in Q1, Q2, Q3, Q4 in a given year: y
int sumRevenue(int y) {
	SELECT cust_pay AS total_rev
		FROM(SELECT SUM(cust_pay)
			FROM purchases
			WHERE(purch_date BETWEEN 'y' + '-01-01' AND 'y' + '-03-31') AND cancled = false
			UNION
			SELECT SUM(cust_pay)
			FROM purchases
			WHERE(purch_date BETWEEN 'y' + '-04-01' AND 'y' + '-06-30') AND cancled = false
			UNION
			SELECT SUM(cust_pay)
			FROM purchases
			WHERE(purch_date BETWEEN 'y' + '-07-01' AND 'y' + '-09-30') AND cancled = false
			UNION
			SELECT SUM(cust_pay)
			FROM purchases
			WHERE(purch_date BETWEEN 'y' + '-10-01' AND 'y' + '-12-31') AND cancled = false);
}

//17. Show how many customers were added from given date: fromDate
int customersAdded(date fromDate) {
	SELECT COUNT(cust_id)
		FROM
		(SELECT cust_id
			FROM
			(SELECT *
				FROM
				(SELECT cust_id, MIN(purch_date) first_purch
					FROM purchases
					WHERE canceled = false)
				GROUP BY cust_id)
			WHERE first_purch >= fromDate) custs
		RIGHT JOIN customers ON customers.cust_id = custs.cust_id;
}

//18. total amount paid to a given supplier: suppID, between given dates: fromDate, tilDate
int totalPaidSupplier(unsigned int supID, date fromDate, date tilDate) {
	SELECT SUM(tot_price) AS tot_shekels
		FROM(SELECT *
			FROM orders
			WHERE supp_id = 'suppID' AND(order_date BETWEEN 'fromDate' AND 'tilDate'));
}

//19. total amount a given seller: sellID earned between given dates: fromDate, tilDate
int totalSoldSeller(unsigned int sellID, date fromDate date tilDate) {
	SELECT SUM(cust_pay) AS tot_shekels
		FROM(SELECT *
			FROM purchases
			WHERE seller_id = 'sellID' AND(purch_date BETWEEN 'fromDate' AND 'tilDate') AND canceled = false);
}

//20. top 10 most sold books between given dates: fromDate, tilDate
int top10Books(date fromDate, date tilDate) {
	SELECT *
		FROM
		(SELECT *
			FROM
			(SELECT book_id, COUNT(*) AS amount
				FROM purchases
				WHERE purch_date BETWEEN 'fromDate' AND 'tilDate'
				GROUP BY book_id)
			ORDER BY amount DESC) AS books_order
		LEFT JOIN books ON books.book_id = books_order.book_id
		LIMIT 10;
}
