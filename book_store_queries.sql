/*1. Show all available books in inventory*/

SELECT * 
FROM books 
LEFT JOIN inventory ON books.book_id = inventory.book_id
WHERE inventory.amount > 0;

/*2. Show all open orders */
SELECT order_id, order_date, cust_id, supp_id, book_id, title, amount, tot_price, stat_type
FROM
	(
    SELECT *
	FROM (SELECT *
		  FROM orders
		  LEFT JOIN order_status ON orders.status_id = order_status.status_id
		  UNION
		  SELECT *
		  FROM orders
		  RIGHT JOIN order_status ON orders.status_id = order_status.status_id) AS stat_order
	LEFT JOIN books ON books.book_id = stat_order.book_id
	UNION
	SELECT *
	FROM stat_order
	RIGHT JOIN books ON books.book_id = stat_order.book_id)
WHERE (cust_id IS NOT NULL AND status_id != 5 AND status_id != 6) OR 
	  (cust_id IS NULL AND status_id != 3 AND status_id != 6);    
      
 /*3. List of all customers*/
 
 SELECT *
 FROM customers
 WHERE tot_expense > 0;
 
 /*4. Show all suppliers*/
 
 SELECT *
 FROM suppliers;
 
 /* 5. Show all purchases between given dates: fromDate & tilDate */
SELECT *
FROM purchases
WHERE (purch_date BETWEEN 'fromDate' AND 'tilDate');

/*6. Show all books available for global sale*/
SELECT book_id, title, author_name, price AS original_price, (price*(100-discount)) AS disc_price
FROM
	(SELECT *
	FROM store_items
	LEFT JOIN books ON store_items.book_id=books.book_id
	UNION
	SELECT *
	FROM store_items
	RIGHT JOIN books ON store_items.book_id=books.book_id)
WHERE discount > 0;

/*7. Check if a given book: bookTitle + bookAuthor is available in the inventory*/
 
SELECT book_id, amount
FROM 
	 (SELECT book_id
	 FROM books
	 WHERE title = 'bookTitle' AND author_name = 'bookAuthor') AS search_book
RIGHT JOIN inventory ON search_book.book_id = inventory.book_id;    


 /* 8. List of all suppliers of a given book: bookTitle + bookAuthor */
 
SELECT *
FROM 
	 (SELECT book_id
	 FROM books
	 WHERE title = 'bookTitle' AND author_name = 'bookAuthor') AS search_book