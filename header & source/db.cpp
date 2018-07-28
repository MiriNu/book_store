#include "db.h"

Database *Database::instance = 0;

Database::Database() : driver(get_driver_instance()) {
	connection_properties["hostName"] = DB_HOST;
	connection_properties["port"] = DB_PORT;
	connection_properties["userName"] = DB_USER;
	connection_properties["password"] = DB_PASS;
	connection_properties["OPT_RECONNECT"] = true;

	// use database
	try {
		Connection *con = driver->connect(connection_properties);
		try {
			con->setSchema("book_store");
		}
		catch (SQLException &e) {
			Statement *stmt = con->createStatement();
			stmt->execute("CREATE DATABASE IF NOT EXISTS book_store;"
						  "USE book_store;");
			con->setSchema("book_store"); // switch database
			stmt->execute("CREATE TABLE IF NOT EXISTS customers("
						  "cust_id INT UNSIGNED NOT NULL PRIMARY KEY,"
						  "first_name VARCHAR(100),"
						  "last_name VARCHAR(100),"
						  "phone VARCHAR(10)"
						  ");");
			stmt->execute("CREATE TABLE IF NOT EXISTS sellers("
						 "seller_id INT UNSIGNED NOT NULL PRIMARY KEY,"
						 "first_name VARCHAR(100),"
						 "last_name VARCHAR(100)"
						 ");");
			stmt->execute("CREATE TABLE IF NOT EXISTS books("
						  "book_id INT UNSIGNED NOT NULL PRIMARY KEY,"
						  "title VARCHAR(100),"
						  "author_name VARCHAR(100)"
						  ");");
			stmt->execute("CREATE TABLE IF NOT EXISTS store_items("
						  "book_id INT UNSIGNED NOT NULL,"
						  "genre_id INT UNSIGNED NOT NULL,"
						  "FOREIGN KEY(book_id) REFERENCES books(book_id),"
						  "FOREIGN KEY(genre_id) REFERENCES genres(genre_id),"
						  "price DOUBLE,"
						  "discount INT UNSIGNED /* % */"
						  ");");
			stmt->execute("CREATE TABLE IF NOT EXISTS purchases("
						  "purch_id INT UNSIGNED NOT NULL PRIMARY KEY,"
						  "book_id INT UNSIGNED,"
						  "seller_id INT UNSIGNED,"
						  "cust_id INT UNSIGNED,"
						  "FOREIGN KEY(book_id) REFERENCES store_items(book_id),"
						  "FOREIGN KEY(seller_id) REFERENCES sellers(seller_id),"
						  "FOREIGN KEY(cust_id) REFERENCES customers(cust_id),"
						  "purch_date DATE,"
						  "canceled BOOL,"
						  "origin_price DOUBLE,"
						  "cust_pay DOUBLE"
						  ");");
			stmt->execute("CREATE TABLE IF NOT EXISTS genres("
						  "genre_id INT UNSIGNED NOT NULL PRIMARY KEY,"
						  "g_name VARCHAR(100)"
						  ");");
			stmt->execute("CREATE TABLE IF NOT EXISTS order_status("
						  "status_id INT UNSIGNED NOT NULL PRIMARY KEY,"
						  "stat_type VARCHAR(100)"
						  ");");
			stmt->execute("CREATE TABLE IF NOT EXISTS orders("
						  "order_id INT UNSIGNED NOT NULL PRIMARY KEY,"
						  "book_id INT UNSIGNED,"
						  "supp_id INT UNSIGNED,"
						  "cust_id INT UNSIGNED,"
						  "status_id INT UNSIGNED,"
						  "FOREIGN KEY(book_id) REFERENCES store_items(book_id),"
						  "FOREIGN KEY(supp_id) REFERENCES suppliers(supp_id),"
						  "FOREIGN KEY(cust_id) REFERENCES customers(cust_id),"
						  "amount INT UNSIGNED,"
						  "/*status ENUM ('not ordered yet','ordered','received','message sent to customer','sold','canceled'),*/"
						  "FOREIGN KEY(status_id) REFERENCES order_status(status_id),"
						  "order_date DATE,"
						  "tot_price DOUBLE"
						  ");");
			stmt->execute("CREATE TABLE IF NOT EXISTS suppliers("
						  "supp_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,"
						  "supp_name VARCHAR(100),"
						  "phone VARCHAR(10),"
						  "bank_acc VARCHAR(10)"
						  ");");
			stmt->execute("CREATE TABLE IF NOT EXISTS book_prices("
						  "book_id INT UNSIGNED,"
						  "supp_id INT UNSIGNED,"
						  "FOREIGN KEY(supp_id) REFERENCES suppliers(supp_id),"
						  "FOREIGN KEY(book_id) REFERENCES store_items(book_id),"
						  "PRIMARY KEY(supp_id, book_id), /* check if this causes problems */"
						  "price DOUBLE"
						  ");");
			stmt->execute("CREATE TABLE IF NOT EXISTS inventory("
						  "book_id INT UNSIGNED,"
						  "FOREIGN KEY(book_id) REFERENCES store_items(book_id),"
						  "amount INT UNSIGNED"
						  ");");
			delete stmt;
		}
		delete con;
	}
	catch (SQLException &e) {
		cout << e.getErrorCode() << " " << e.what() << " " << e.getSQLState();
	}
}

Database & Database::getInstance() {
	if (Database::instance == 0) {
		instance = new Database();
	}
	return *instance;
}

Connection * Database::getConnection() {
	try {
		Connection *con = driver->connect(connection_properties);
		con->setSchema(DB_NAME);
		return con;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
	return 0;
}
