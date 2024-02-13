#include <iostream>
#include <vector>
#include "Errors.h"
#include "../text_files/working_with_files.h"

using namespace std;

enum Options {
    ENQUEUE = 0, DEQUEUE, PRINT, PRINT_ALL_CLIENTS,
    PRINT_VIP_CLIENTS, SEARCH, SEARCH_BY_SERVICE_NAME,
    SEARCH_BY_MOBILE_NUMBER, UPDATE, INVALID
};

void execute_option(string &option);

void execute_enqueue();

string get_mobile_number();

pair<bool, string> is_valid_number(string &mobile_number);

void mobile_number_errors(string &mobile_number);

bool is_vip(string &mobile_number);

string get_service_name(string &mobile_number);

void display_service_menu();

bool is_valid_service(string &service_name, string &mobile_number);

string service_mapper(string &service_name);

void execute_dequeue();

void execute_print();

void execute_print_all_clients();

void execute_print_vip_clients();

void execute_search();

void search_by_service_name();

void search_by_mobile_number();

void execute_update();

void execute_invalid_option();

Options mapper(string &option);

pair<string, Options> menu_options[] = {
        {"E",   ENQUEUE},
        {"D",   DEQUEUE},
        {"P",   PRINT},
        {"PA",  PRINT_ALL_CLIENTS},
        {"PV",  PRINT_VIP_CLIENTS},
        {"S",   SEARCH},
        {"SBS", SEARCH_BY_SERVICE_NAME},
        {"SBM", SEARCH_BY_MOBILE_NUMBER},
        {"U",   UPDATE},
};

pair<Options, void (*)()> option_executors[] = {
        {ENQUEUE,                 execute_enqueue},
        {DEQUEUE,                 execute_dequeue},
        {PRINT,                   execute_print},
        {PRINT_ALL_CLIENTS,       execute_print_all_clients},
        {PRINT_ALL_CLIENTS,       execute_print_vip_clients},
        {SEARCH,                  execute_search},
        {SEARCH_BY_SERVICE_NAME,  search_by_service_name},
        {SEARCH_BY_MOBILE_NUMBER, search_by_mobile_number},
        {UPDATE,                  execute_update},
        {INVALID,                 execute_invalid_option},
};

Options mapper(string &option) {
    for (auto &pair: menu_options) {
        if (pair.first == option)
            return pair.second;
    }
    return Options::INVALID;
}

void execute_option(string &option) {
    option_executors[mapper(option)].second();
}

void execute_enqueue() {
    Client_Service client_service;
    client_service.client.mobile_number = get_mobile_number();
    client_service.name = get_service_name(client_service.client.mobile_number);
    if (is_vip(client_service.client.mobile_number)) {
        client_service.client.is_vip = true;
        enqueue_vip(client_service);
    } else
        enqueue_ordinary_client(client_service);
    diplay_current_added_client(client_service);
    write_to_queue_storage_file(client_service);
    rewrite_queue_storage_file();
}

string get_mobile_number() {
    cout << "Enter your mobile number\n";
    cout << "Your input: ";
    string mobile_number;
    cin >> mobile_number;
    while (!is_valid_number(mobile_number).first) {
        mobile_number_errors(mobile_number);
        cout << "Your input: ";
        cin >> mobile_number;
    }
    return mobile_number;
}

pair<bool, string> is_valid_number(string &mobile_number) {
    pair<bool, string> pair(true, "");
    try {
        stoll(mobile_number);
    } catch (...) {
        pair.first = false;
        pair.second = CONTAINING_CHARACTER_ERROR;
        return pair;
    }
    for (int i = 0; i <mobile_number.length(); i++){
        if (mobile_number[i] - '0' < 0 || mobile_number[i] - '0' > 9) {
            pair.first = false;
            pair.second = CONTAINING_CHARACTER_ERROR;
            return pair;
        }
    }

    if (mobile_number.length() != 11) {
        pair.first = false;
        pair.second = LESS_THAN_ELEVEN_DIGIT;
        return pair;
    } else if (mobile_number[0] != '0') {
        pair.first = false;
        pair.second = NOT_BEGIN_WITH_ZERO;
        return pair;
    } else {
        return pair;
    }
}

void mobile_number_errors(string &mobile_number) {
    if (is_valid_number(mobile_number).second == CONTAINING_CHARACTER_ERROR)
        cout << "Mobile number must contain numbers only\n";
    else if (is_valid_number(mobile_number).second == LESS_THAN_ELEVEN_DIGIT)
        cout << "Mobile number must contain 11 digits\n";
    else
        cout << "Mobile number must begin with 0\n";
}

bool is_vip(string &mobile_number) {
    vector<string> mobile_numbers = read_vip_clients_file();
    for (string &i: mobile_numbers) {
        if (i == mobile_number)
            return true;
    }
    return false;
}

string get_service_name(string &mobile_number) {
    display_service_menu();
    cout << "Enter service you want\n";
    cout << "Your input: ";
    string service_name;
    cin >> service_name;
    while (!is_valid_service(service_name, mobile_number)) {
        if (service_name == "BB")
            cout << "Your aren't VIP client\n";
        else
            cout << "Preferably choose Business Banking service as you are VIP client\n";
        cout << "Enter valid service: ";
        cin >> service_name;
    }
    service_name = service_mapper(service_name);
    return service_name;
}

void display_service_menu() {
    cout.width(35);
    cout << "IB: Individual Banking\n";
    cout.width(33);
    cout << "BB: Business Banking\n";
    cout.width(32);
    cout << "DB: Digital Banking\n";
    cout.width(21);
    cout << "L: Loans\n";
}

bool is_valid_service(string &service_name, string &mobile_number) {
    if (service_name == "IB" || service_name == "DB" || service_name == "L") {
        if (!is_vip(mobile_number))
            return true;
        else
            return false;
    } else if (service_name == "BB") {
        if (!is_vip(mobile_number)) {
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

string service_mapper(string &service_name) {
    if (service_name == "IB")
        return "Individual_Banking";
    else if (service_name == "BB")
        return "Business_Banking";
    else if (service_name == "DB")
        return "Digital_Banking";
    else if (service_name == "L")
        return "Loans";
    else
        return "Invalid service name";
}

void execute_dequeue() {
    dequeue();
    rewrite_queue_storage_file();
}

void execute_print() {
    cout.width(37);
    cout << "PA: Print all clients\n";
    cout.width(37);
    cout << "PV: Print VIP clients\n";
    cout << "Your Input: ";
    string option;
    cin >> option;
    execute_option(option);
}

void execute_print_all_clients() {
    print_all_clients();
}

void execute_print_vip_clients() {
    print_vip_clients();
}

void execute_search() {
    cout.width(43);
    cout << "SBS: Search by service name\n";
    cout.width(44);
    cout << "SBM: Search by mobile number\n";
    cout << "Your Input: ";
    string option;
    cin >> option;
    execute_option(option);
}

void search_by_service_name() {
    cout << "Enter service name: ";
    string service_name;
    cin >> service_name;
    service_name = service_mapper(service_name);
    bool is_found = search_for_clients_in_file(service_name);
    if (!is_found) {
        cout << "Not found\n";
    }
}

void search_by_mobile_number() {
    string mobile_number = get_mobile_number();
    bool is_found = search_for_clients_in_file(mobile_number);
    if (!is_found) {
        cout << "Not found\n";
    }
}

void execute_update() {
    string mobile_number = get_mobile_number();
    Client_Service client_service = ordinary_clients.front->client_service;
    bool is_found = search_for_clients_in_file(mobile_number);
    if (is_vip(mobile_number)&& is_found){
        cout << "\nYou are already VIP client\n";
        return;
    }
    if (client_service.client.mobile_number == mobile_number && empty(vip_clients)) {
        cout << "Client with mobile number: " << mobile_number
             << " become VIP client\n";
        rewrite_vip_file(mobile_number);
    } else if (!is_found) {
        cout << "Not found\n";
    } else {
        cout << "\nClient must be serviced first\n";
    }
}

void execute_invalid_option() {
    cout << "Invalid input\n";
}
