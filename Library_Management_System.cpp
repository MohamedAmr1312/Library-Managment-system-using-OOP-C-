#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// abstract class user
class USER{
    protected:
    string NAME;
    int ID;
    
    public:
    USER(){NAME="undefined"; ID=0; }
    
    USER(string nm, int id){
        NAME = nm;
        ID = id;
    }

    void set_name(string nm){NAME = nm;}
    void set_id(int id){ID = id;}

    string get_name(){return NAME;}
    int get_ID(){return ID;}

    virtual void displayMenu() const = 0;

    virtual ~USER() {
        cout << "User destructor called for: " << NAME << endl; }
    

}; 
//book class
class Book{
    private:
    string title;
    string author;
    string ISBN;
    bool isavailable;

    public:
    Book(string title, string author, string ISBN) : title(title),author(author),ISBN(ISBN),isavailable(true) {}
    void set_title(string title){this->title = title;}
    void set_author(string author){this->author = author;}
    void set_ISBN(string ISBN){this->ISBN = ISBN;}
    void set_isavailable(bool isavailable){this->isavailable = isavailable;}
    string get_title(){return title;}
    string get_author(){return author;}
    string get_ISBN(){return ISBN;}
    bool get_isavailable(){return isavailable;}
    void displayBookInfo() const {
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "ISBN: " << ISBN << endl;
        cout << "Available: " << (isavailable ? "Yes" : "No") << endl;
    }

};


//borrow record class
class BorrowRecord{
    private:
    shared_ptr<Book> book;
    int memberID;
    string dueDate;

    public:
    BorrowRecord(shared_ptr<Book> book, int memberID, string dueDate) : book(book), memberID(memberID), dueDate(dueDate) {}
    
    shared_ptr<Book> getBook(){return book;}
    int getMemberID(){return memberID;}
    string getDueDate(){return dueDate;}

    void showRecordDetails() const {
        cout << "Book Title: " << book->get_title() << endl;
        cout << "Member ID: " << memberID << endl;
        cout << "Due Date: " << dueDate << endl;
    }
};
// Global book and record storage
vector<shared_ptr<Book>> books;
vector<shared_ptr<BorrowRecord>> allRecords;
class Librarian : public USER {
    public:
        Librarian(string nm, int id) : USER(nm, id) {}

        void addbook(string title, string author, string ISBN) {
            shared_ptr<Book> newBook = make_shared<Book>(title, author, ISBN);
            books.push_back(newBook);
            cout << "Book added: " << title << endl;
        }
    
        void removebook(string ISBN) {
            for (auto it = books.begin(); it != books.end(); ++it) {
                if ((*it)->get_ISBN() == ISBN) {
                    cout << "Removing book: " << (*it)->get_title() << endl;
                    books.erase(it);
                    return;
                }
            }
            cout << "Book with ISBN " << ISBN << " not found." << endl;
        }
    
        void viewBooks() const {
            cout << "\nAll books in the library:" << endl;
            for (const auto& b : books) {
                b->displayBookInfo();
                cout << "---------" << endl;
            }
        }
    
        void borrowedbook() {
            cout << "\nAll Borrowing Records:" << endl;
            for (const auto& record : allRecords) {
                record->showRecordDetails();
                cout << "---------" << endl;
            }
        }
    
        void displayMenu() const override {
            cout << "\n=== Librarian Menu ===" << endl;
            cout << "1. Add Book" << endl;
            cout << "2. Remove Book" << endl;
            cout << "3. View All Books" << endl;
            cout << "4. View Borrowed Books" << endl;
        }
    
        ~Librarian() {
            cout << "Librarian destructor called for " << NAME << endl;
        }
    };
    

class Member : public USER {
    private:
    vector<shared_ptr<BorrowRecord>> borrowedBooks;

public:
    Member(string nm, int id) : USER(nm, id) {}

    void borrowbook(shared_ptr<Book> book, string dueDate) {
        if (book->get_isavailable()) {
            book->set_isavailable(false);
            auto record = make_shared<BorrowRecord>(book, ID, dueDate);
            borrowedBooks.push_back(record);
            allRecords.push_back(record);  // global record
            cout << NAME << " borrowed: " << book->get_title() << endl;
        } else {
            cout << "Book is not available." << endl;
        }
    }

    void returnbook(string ISBN) {
        for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); ++it) {
            if ((*it)->getBook()->get_ISBN() == ISBN) {
                (*it)->getBook()->set_isavailable(true);
                cout << NAME << " returned: " << (*it)->getBook()->get_title() << endl;
                borrowedBooks.erase(it);
                return;
            }
        }
        cout << "You have not borrowed a book with ISBN: " << ISBN << endl;
    }

    void viewBorrowedBooks() const {
        cout << "\nBooks borrowed by " << NAME << ":" << endl;
        for (const auto& record : borrowedBooks) {
            record->showRecordDetails();
            cout << "---------" << endl;
        }
    }

    void availablebook() {
        cout << "\nAvailable Books:" << endl;
        for (auto& b : books) {
            if (b->get_isavailable()) {
                b->displayBookInfo();
                cout << "---------" << endl;
            }
        }
    }

    void displayMenu() const override {
        cout << "\n=== Member Menu ===" << endl;
        cout << "1. Borrow Book" << endl;
        cout << "2. Return Book" << endl;
        cout << "3. View Borrowed Books" << endl;
        cout << "4. View Available Books" << endl;
    }

    ~Member() {
        cout << "Member destructor called for " << NAME << endl;
    }
};


// Main menu system
int main() {
    // Create sample users
    shared_ptr<Librarian> lib = make_shared<Librarian>("عم جمعه", 1);
    shared_ptr<Member> mem = make_shared<Member>("mohamed", 2001);

    int choice;
    while (true) {
        cout << "\n=== Library Management System ===" << endl;
        cout << "1. Login as Librarian" << endl;
        cout << "2. Login as Member" << endl;
        cout << "3. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;
        //librarian
        if (choice == 1) {
            lib->displayMenu();
            int opt;
            cin >> opt;
            string title, author, isbn;
            switch (opt) {
                case 1:
                    cout << "Enter Title: ";
                    cin.ignore();
                    getline(cin, title);
                    cout << "Enter Author: ";
                    getline(cin, author);
                    cout << "Enter ISBN: ";
                    getline(cin, isbn);
                    lib->addbook(title, author, isbn);
                    break;
                case 2:
                    cout << "Enter ISBN to remove: ";
                    cin >> isbn;
                    lib->removebook(isbn);
                    break;
                case 3:
                    lib->viewBooks();
                    break;
                case 4:
                    lib->borrowedbook();
                    break;
            }
        }
        //member 
        else if (choice == 2) {
            mem->displayMenu();
            int opt;
            cin >> opt;
            string isbn, due;
            switch (opt) {
                case 1:
                    cout << "Enter ISBN to borrow: ";
                    cin >> isbn;
                    cout << "Enter Due Date (YYYY-MM-DD): ";
                    cin >> due;
                    {
                        Book* rawPtr = nullptr; // raw pointer to find the book
                        for (auto& b : books) {
                            if (b->get_ISBN() == isbn && b->get_isavailable()) {
                                rawPtr = b.get();
                                break;
                            }
                        }
                        if (rawPtr != nullptr) {
                            shared_ptr<Book> bookShared = nullptr;
                            for (auto& b : books) {
                                if (b->get_ISBN() == rawPtr->get_ISBN()) {
                                    bookShared = b;
                                    break;
                                }
                            }
                            mem->borrowbook(bookShared, due);
                        } else {
                            cout << "Book not available or not found." << endl;
                        }
                    }
                    break;
                case 2:
                    cout << "Enter ISBN to return: ";
                    cin >> isbn;
                    mem->returnbook(isbn);
                    break;
                case 3:
                    mem->viewBorrowedBooks();
                    break;
                case 4:
                    mem->availablebook();
                    break;
            }
        } 
        //exit
        else if (choice == 3) {
            cout << "Exiting... Goodbye!" << endl;
            break;
        }
    }

    return 0;
}