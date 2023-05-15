#include <iostream>
#include "git.hpp"
using namespace std;

void menu()
{
    // print the user interface
    cout << "-------------------------------------" << endl;
    cout << "Please choose an option (number):" << endl;
    cout << "1. Init" << endl;
    cout << "2. Add file to current commit" << endl;
    cout << "3. Remove file from the current commit" << endl;
    cout << "4. Commit" << endl;
    cout << "5. Checkout different version" << endl;
    cout << "6. Status" << endl;
    cout << "7. Quit" << endl;
}

int main()
{
    // create a new Git object
    Git git;

    // user menu

    bool done = false;
    while (!done)
    {
        menu();
        string input;
        cin >> input;

        switch (stoi(input))
        {
        case 1:
            git.initialize();
            break;

        case 2:
            git.addFile();
            break;

        case 3:
            git.removeFile();
            break;

        case 4:
            git.commit();
            break;

        case 5:
            git.checkout();
            break;

        case 6:
            git.status();
            break;
        case 7:
            done = true;
            break;
        default:
            break;
        }
    }
}
