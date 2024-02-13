#include "../header_files/Menu_Options.h"

void display_main_view();

void display_menu_options();

string get_menu_option();

void clear_console();

void display_main_view() {
    read_queue_storage_file();
    while (true) {
        cout.width(80);
        cout << "*** Welcome to Bank Queue System ***" << endl;
        display_menu_options();
        string option = get_menu_option();
        if (option == "Q") {
            cout << "Thanks for using my bank system\n";
            break;
        }
        execute_option(option);
        clear_console();
    }
}

void display_menu_options() {
    cout.width(25);
    cout << "E: Enqueue" << endl;
    cout.width(25);
    cout << "D: Dequeue" << endl;
    cout.width(23);
    cout << "P: Print" << endl;
    cout.width(24);
    cout << "S: Search" << endl;
    cout.width(24);
    cout << "U: Update" << endl;
    cout.width(22);
    cout << "Q: Quit" << endl;
}

string get_menu_option() {
    cout << "Your Input: ";
    string option;
    cin >> option;
    return option;
}

void clear_console() {
    string line;
    getline(cin, line);
    cin.ignore();
    system("CLS");
}