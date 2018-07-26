CREATE DATABASE IF NOT EXISTS book_store;
USE book_store;

CREATE TABLE IF NOT EXISTS customers (
	cust_phone VARCHAR(10) PRIMARY KEY,
    first_name VARCHAR(20),
    date_added DATE,
    tot_expense DOUBLE
);

CREATE TABLE IF NOT EXISTS purchases (
	purch_id INT UNSIGNED PRIMARY KEY,
    FOREIGN KEY (book_id) REFERENCES store_items(book_id),
    seller_id INT UNSIGNED,
    FOREIGN KEY (cust_phone) REFERENCES customers(cust_phone),
    purch_date DATE,
    canceled BOOL,
    origin_price DOUBLE,
    final_price DOUBLE
);

CREATE TABLE IF NOT EXISTS store_items (
	FOREIGN KEY (book_id) REFERENCES books(book_id),
	genre ENUM('sci-fi', 'popular science', 'fantasy', 'fiction', 'history','mystery'),/* the system should give options for genres!!!!*/
    price DOUBLE,
	discount INT /* % */
);

CREATE TABLE IF NOT EXISTS books (
	book_id INT UNSIGNED PRIMARY KEY,
    first_name VARCHAR(20),
    author_name VARCHAR(50)
);

CREATE TABLE IF NOT EXISTS orders (
	order_id INT UNSIGNED PRIMARY KEY,
    FOREIGN KEY (book_id) REFERENCES store_items(book_id),
    FOREIGN KEY (supp_id) REFERENCES suppliers(supp_id),
    FOREIGN KEY (cust_phone) REFERENCES customers(cust_phone),
    amount INT,
    status ENUM ('not ordered yet','ordered','received','message sent to customer','sold','canceled'),
    order_date DATE,
    tot_price DOUBLE
);

CREATE TABLE IF NOT EXISTS suppliers (
	supp_id INT UNSIGNED PRIMARY KEY,
    phone VARCHAR(10)
);

CREATE TABLE IF NOT EXISTS book_prices (
	FOREIGN KEY (supp_id) REFERENCES suppliers(supp_id),
    FOREIGN KEY (book_id) REFERENCES store_items(book_id),
    PRIMARY KEY (supp_id, book_id), /* check if this causes problems */
    price DOUBLE
);

CREATE TABLE IF NOT EXISTS inventory (
	FOREIGN KEY (book_id) REFERENCES store_items(book_id),
    amount INT
);

