INSERT INTO customers (cust_id,first_name,last_name,phone)
VALUES
(1,,,),
(2,,,),
(3,,,),
(4,,,),
(5,,,),
(6,,,),
(7,,,),
(8,,,),
(9,,,),
(10,,,);

INSERT INTO sellers (seller_id,first_name,last_name)
VALUES
(1,,),
(2,,),
(3,,),
(4,,),
(5,,),
(6,,),
(7,,),
(8,,),
(9,,),
(10,,);

INSERT INTO books (book_id,title,author_name)
VALUES
(1,,),
(2,,),
(3,,),
(4,,),
(5,,),
(6,,),
(7,,),
(8,,),
(9,,),
(10,,);

INSERT INTO store_items (book_id,genre_id,price,discount)
VALUES
(1,,,),
(2,,,),
(3,,,),
(4,,,),
(5,,,),
(6,,,),
(7,,,),
(8,,,),
(9,,,),
(10,,,);

INSERT INTO purchases (purch_id,book_id,seller_id,cust_id,purch_date,canceled,origin_price,cust_pay)
VALUES
(1,,,,,,,),
(2,,,,,,,),
(3,,,,,,,),
(4,,,,,,,),
(5,,,,,,,),
(6,,,,,,,),
(7,,,,,,,),
(8,,,,,,,),
(9,,,,,,,),
(10,,,,,,,);

INSERT INTO genres (genre_id,g_name)
VALUES
(1,),
(2,),
(3,),
(4,),
(5,),
(6,),
(7,),
(8,),
(9,),
(10,);