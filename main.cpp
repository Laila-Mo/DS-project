#include <iostream>
#include "Tree_234.h"
#include<string>
using namespace std;

int main()
{
    dictionary<string>dictionary;
    int choice, NumberOfBooks, control;
    string book;
    bool s;
    
    do
    {
        cout << "1.Create library" << endl;
        cout << "2.Search for a book" << endl;
        cout << "3.Delete a book" << endl;
        cout << "4.Display available books" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "Enter the number of books you want to add" << endl;
            cin >> NumberOfBooks;
            cin.ignore();
            cout << "Enter book to add" << endl;
            for (int i = 0; i < NumberOfBooks; i++)
            {
                
                getline(cin, book);
                
                s=dictionary.Add(book);
                if (s == 0) {
                    cout << "word: " + book + "already exists" << endl;
                }
            }
            break;
        case 2:
            cout << "Enter the book you want to search for" << endl;
            cin.ignore();
            getline(cin, book);

            if (dictionary.Find(book) == NULL) {
                cout << "not found" << endl;
            }
            else {
                cout << book + " is found"<<endl;
            }
              
       
            break;
        case 3:
            cout << "Enter the book you want to delete" << endl;
            cin.ignore();
            getline(cin, book);
            s=dictionary.Remove(book);
            if (s == 0) {
                cout << "word: " + book + "is not found" << endl;
            }
            break;
        case 4:
            dictionary.Display(cout);
            break;
        default:
            cout << "choice doesn't exist" << endl;
            break;
        }
        cout << "Enter 1 to get to main menu";
        cin >> control;
    } while (control == 1);
    system("pause");
    return 0;
}
