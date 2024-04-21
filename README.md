# End-to-End Encrypted Chat Application & Login And Registration System for using the Application & Secure Password Generation and Storage |  C++

Command-line application written in C++ with the following features:
## Features
* Registration: Users can provide usernames and choosing either their own password or generating a strong password.
* Strong Password Generation : this program provides strong pass generation of a password that contains 26 letters containing uppercase lowercase, special characters, and digits.
* Login: Registered users can log in by providing their username and password.
* Password Strength: The system enforces strong password requirements, including a minimum length of 6 characters, containing at least one uppercase letter, one lowercase letter, one digit, and one special character through validation functions.
* Secure Password storage and Chat application encryption(Encryption): User passwords are encrypted using Caesar cipher before being stored in a file.
* Main Menu: Users are presented with a main menu upon running the program, offering options to log in, register, or exit.
* End-to-End encrypted Chat Application: After logging in, users can connect to an active server for using the chat application and text messages sent are encrypted, and are to be decrypted by the recepient only

## File Structure
* cw2.cpp: The main C++ source code file containing the implementation of the Registration, login, and encrypted chat application, along with the implementation of generating strong passwords and validating passwrods.
* records.txt: A text file where user registration information (usernames and encrypted passwords) is stored.
* Server/main.cpp: The c++ source code file containing the implementation of the server socket which clients connect to.
* Client/main.cpp: The c++ source code file containing the implementation of the client socket clients connect to the server.
* README.md: This README file providing an overview of the Chat Appliaction System system.

## Usage
<ol>
<li>Compilation: Compile the login_system.cpp file using a C++ compiler. For example, using g++: g++ cw2.cpp -o cw2.</li>
<hr>
<li>Execution: Run the compiled executable file. For example: ./cw2.</li>
<hr>
<li>Main Menu: Upon running the program, you will be presented with a main menu where you can choose to log in, register, or exit.</li>
<div align="center"><img width="312" alt="Main Menu" src="https://github.com/Yahya210/cw1-Programming/assets/114566232/1c8c9a4a-6659-40b8-86b6-1a6a305b52f2"></div>
<hr>
<li>Registration: If you choose to register, you will be prompted to enter your username and choose a password. You can either enter your own password(Ensuring you meet the strong password criteria) or generate a strong password.</li>
<div align="center"><img width="408" alt="registration-secure-yahya-duplicates" src="https://github.com/Yahya210/cw1-Programming/assets/114566232/ff13c0ae-2a94-45c1-bde7-52f9fda7da55"></div>
<hr>
<li>Login: If you choose to log in, you will be prompted to enter your username and password. If the provided credentials match those in the records, you will be successfully logged in.</li>
<div align="center"><img width="262" alt="Login-Generated" src="https://github.com/Yahya210/cw1-Programming/assets/114566232/2048339d-24e6-4117-b5b2-962323c903ad"></div>
<hr>
<li>Using the Application System: After successful Login, Users can now use the application system, Here are the steps:</li> 
</ol>
    <ol>
      <li><b>Hosting The Server</b>:</li>
         <div>For users to be able to use the Application System, The first user has to host the server, This can simply be done by choosing option 1</div>
        <div align="center"><img width="408" alt="Screenshot 2024-04-19 at 2 34 25 PM" src="https://github.com/Yahya210/cw2-Programming/assets/114566232/29f21391-fd1d-46af-a593-e869311531b3"></div>
<hr>
      <li><b>Connecting To The Server</b></li>
        <div>Now, Users have to connet to the Server for them to be able to communicate with each other, this is can be done by choosing option 2</div>
        <hr>
        <div>In the screenshot below, it shows the first user 'lls' hosts the server, while 'user1' and 'user2' connects to the server, and 'lls' can't see the messages as they're encrypted and they are to be decrypted by the recepient only</div>
        <div align="center"><img width="1167" alt="Screenshot 2024-04-19 at 2 41 54 PM" src="https://github.com/Yahya210/cw2-Programming/assets/114566232/d2d5bfe6-2b59-41ac-8618-7124da16d68c"></div>
    </ol>


## Encryption Verification
To verify that Messages sent to the server have to be encrypted and are to be decrypted by the recipient, i used wire shark which shows that the conversation is encrypted through network traffic as well
<div align="center"><img width="636" alt="image" src="https://github.com/Yahya210/cw2-Programming/assets/114566232/0950fc70-f9ff-40b2-9dbd-407bd1199e46"></div>

    
## List of Dependencies

- **cstring**: Standard C library for string manipulation.
- **iostream**: Standard C++ library for input and output operations.
- **netinet/in.h**: Header file for Internet Protocol (IP) networking.
- **sys/socket.h**: Header file for socket programming.
- **unistd.h**: Standard Unix header file for system calls.
- **pthread.h**: Header file for POSIX threads.
- **string**: Standard C++ library for string manipulation.
* **It also utilizes the <unistd.h> library for the sleep() function, which may not work on all operating systems (Works only on Linux based systems).**

## Note
* Operating System Compatibility: Certain features of this program, such as the use of sleep(), may not work on all operating systems. Adjustments may be needed for cross-platform compatibility.

## Author
This Chat Application system was created by **Yahya Mohamed** as a demonstration of user authentication, secure password generation and storage in C++, End-to-End Encryption for the Chat Application, Server Sockets, Client Sockets, Multithreading.


