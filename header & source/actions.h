#ifndef ACTIONS_H
#define ACTIONS_H

#include "db.h";

//1. Show all available books in inventory
int showBooks();

//2. Show all open orders
int showOrders();

//3. List of all customers who made a purchase
int showCustomers();

//4. Show all suppliers
int showSuppliers();

//5. Show all purchases between given dates: fromDate & tilDate
int showPurchases(date fromDate, date tilDate);

//6. Show all books available for global sale
int showBooksOnSale();

//7. Check if a given book: bookTitle + bookAuthor is available in the inventory
int searchBook(string bookTitle, string bookAuthor);

//8. List of all suppliers of a given book: bookTitle + bookAuthor
int showSuppliersOfBook(string bookTitle, string bookAuthor);

//9. How many books from type: bookTitle + bookAuthor where sold from date fromDate
int booksSold(string bookTitle, string bookAuthor, date fromDate);

//10. How many books (different books too?) were purchased by customer: custID since given date fromDate
int booksPurchased(unsigned int custID, date fromDate);

//11. Show the customer details who bought the most since given date fromDate
int mostPurchasesCustomer(date fromDate);

//12. show the supplier details who sold us the most books since given date fromDate
int mostOrdersSuplier(date fromDate);

//13. amount of orders made (and how many books?) between given dates: fromDate & tilDate
int ordersMade(date fromDate, date tilDate);

//14. amount of orders made (and how many books?) between given dates: fromDate & tilDate that were made by customers sold
int ordersMadePurchases(date fromDate, date tilDate);

//15. show the total discount a customer received since a certain date
int totalDiscountCustomer(unsigned int custID, date fromDate);

//16. sum revenue in Q1, Q2, Q3, Q4 in a given year: y
int sumRevenue(int y);

//17. Show how many customers were added from given date: fromDate
int customersAdded(date fromDate);

//18. total amount paid to a given supplier: suppID, between given dates: fromDate, tilDate
int totalPaidSupplier(unsigned int supID, date fromDate, date tilDate);

//19. total amount a given seller: sellID earned between given dates: fromDate, tilDate
int totalSoldSeller(unsigned int sellID, date fromDate date tilDate);

//20. top 10 most sold books between given dates: fromDate, tilDate
int top10Books(date fromDate, date tilDate);

#endif
