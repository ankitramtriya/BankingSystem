Design and Development of online banking management system</br>
Description: The project aims to develop a banking system that is user-friendly and multi-
functional. The project should have the following functionalities:</br>
a. All account transactional details and customer information are stored in files.</br>
b. Account holders have to pass through a login system to enter their accounts.</br>
c. The application should possess password-protected administrative access; thus
preventing the whole management system from unauthorized access.</br>
d. Three types of login should be created: normal user, joint account user; administrator;</br>
e. Once you login as administrator, you should be able to add, delete, modify, search for a
specific account details.</br>
f. Once the customer connect to the server, you should get login and password prompt.</br>
After successful login, you should get menu for example:</br>
Do you want to:</br>
 Deposit</br>
 Withdraw</br>
 Balance Enquiry</br>
 Password Change</br>
 View details</br>
 Exit</br>
g. If you login as a joint account holder, proper file locking should be implemented. If you
want to view the account details then read lock is to be set else if you want to withdraw
or deposit then write lock should be used to protect the critical data section.</br>
h. Use socket programming – Server maintains the data base and service multiple clients
concurrently. Client program can connect to the server and access their specific account
details.</br>
i. Use system calls instead of Library functions wherever it is possible in the project:
Process Management, File Management, File Locking, Multithreading and Inter Process
Communication Mechanisms.</br>

First run the server.c file and then run front.c file in different terminal</br>
