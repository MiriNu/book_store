CREATE DATABASE IF NOT EXISTS book_store;
USE book_store;

CREATE TABLE IF NOT EXISTS customers (
	cust_id INT UNSIGNED PRIMARY KEY,
    first_name VARCHAR(20),
    phone VARCHAR(10),
    date_added DATE,
    tot_expense DOUBLE
);

CREATE TABLE IF NOT EXISTS purchases (
	purch_id INT UNSIGNED PRIMARY KEY,
    FOREIGN KEY (book_id) REFERENCES books(book_id),
    seller_id INT UNSIGNED,
    FOREIGN KEY (cust_id) REFERENCES customers(cust_id),
    purch_date DATE,
    canceled BOOL
);

CREATE TABLE IF NOT EXISTS books (
	book_id INT UNSIGNED PRIMARY KEY,
    first_name VARCHAR(20),
    author_name VARCHAR(50),
	genre ENUM('sci-fi', 'popular science', 'fantasy', 'fiction', 'history','mystery'),/* the system should give options for genres!!!!*/
    price DOUBLE,
    FOREIGN KEY (sale_id) REFERENCES sales(sale_id) /* sale stuff are questionable*/
);

CREATE TABLE IF NOT EXISTS orders (
	order_id INT UNSIGNED PRIMARY KEY,
    FOREIGN KEY (book_id) REFERENCES books(book_id),
    FOREIGN KEY (supp_id) REFERENCES suppliers(supp_id),
    FOREIGN KEY (cust_id) REFERENCES customers(cust_id),
    amount INT,
    received BOOL,
    msg_sent BOOL,
    sold BOOL,
    order_date DATE
    
);

CREATE TABLE IF NOT EXISTS suppliers (
	supp_id INT UNSIGNED PRIMARY KEY,
    FOREIGN KEY (book_id) REFERENCES books(book_id),
    phone VARCHAR(10),
    price DOUBLE
);

CREATE TABLE IF NOT EXISTS inventory (
	FOREIGN KEY (book_id) REFERENCES books(book_id),
    amount INT
);

CREATE TABLE IF NOT EXISTS sale (    /* questionable, need to revisit*/
	sale_id INT UNSIGNED PRIMARY KEY,
    type_sale ENUM ('-int','-%int'),
    int_sale INT,
	global BOOL,
    date_start DATE,
    date_finish DATE
);
