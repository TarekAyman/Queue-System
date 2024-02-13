#include <iostream>
#include <fstream>
#include "../header_files/Queue.h"

using namespace std;

vector<string> read_vip_clients_file();

void read_queue_storage_file();

vector<string> split(string &line);

void create_client_service(vector<string> &client_data);

void add_to_specific_queue(Client_Service &client_service);

void write_to_queue_storage_file(Client_Service &client_service);

void rewrite_queue_storage_file();

void write_vip_clients();

void write_ordinary_clients();

bool search_for_clients_in_file(string &search_type);

void get_client_by_data(vector<string> client_data, string &search_type, int count, bool &valid);

void rewrite_vip_file(string &mobile_number);

vector<string> read_vip_clients_file() {
    vector<string> mobile_numbers;
    fstream out_file("text_files/VIP_Clients.txt", ios::in);
    while (!out_file.eof()) {
        string line;
        out_file >> line;
        if (!line.empty())
            mobile_numbers.push_back(line);
    }
    return mobile_numbers;
}

void read_queue_storage_file() {
    vector<string> client_data;
    fstream out_file("text_files/queue_storage.txt", ios::in);
    while (!out_file.eof()) {
        string line;
        out_file >> line;
        if (!line.empty()) {
            client_data = split(line);
            create_client_service(client_data);
        }
    }
}

vector<string> split(string &line) {
    vector<string> client_data;
    string data;
    for (char i: line) {
        if (i != ',')
            data += i;
        else {
            client_data.push_back(data);
            data = "";
        }
    }
    client_data.push_back(data);
    return client_data;
}

void create_client_service(vector<string> &client_data) {
    Client_Service client_service;
    client_service.client.mobile_number = client_data[0];
    client_service.name = client_data[1];
    client_service.number = stoi(client_data[3]);
    if (client_data[2] == "1")
        client_service.client.is_vip = true;
    else
        client_service.client.is_vip = false;
    add_to_specific_queue(client_service);
}

void add_to_specific_queue(Client_Service &client_service) {
    if (client_service.client.is_vip) {
        enqueue_vip(client_service);
    } else {
        enqueue_ordinary_client(client_service);
        number_in_queue = size(ordinary_clients);
    }
}

void write_to_queue_storage_file(Client_Service &client_service) {
    fstream out_file("text_files/queue_storage.txt", ios::app);
    out_file << client_service.client.mobile_number
             << ',' << client_service.name << ','
             << client_service.client.is_vip << ','
             << client_service.number << endl;
}

void rewrite_queue_storage_file() {
    write_vip_clients();
    write_ordinary_clients();
}

void write_vip_clients() {
    fstream out_file("text_files/queue_storage.txt", ios::out);
    for (int i = 1; i <= size(vip_clients); ++i) {
        Client_Service client_service = get_vip_client(i);
        out_file << client_service.client.mobile_number
                 << ',' << client_service.name << ','
                 << client_service.client.is_vip << ','
                 << client_service.number << endl;
    }
}

void write_ordinary_clients() {
    fstream out_file("text_files/queue_storage.txt", ios::app);
    for (int i = 1; i <= size(ordinary_clients); ++i) {
        Client_Service client_service = get_ordinary_client(i);
        out_file << client_service.client.mobile_number
                 << ',' << client_service.name << ','
                 << client_service.client.is_vip << ','
                 << client_service.number << endl;
    }
}

bool search_for_clients_in_file(string &search_type) {
    vector<string> client_data;
    int count = 0;
    bool valid = false;
    fstream out_file("text_files/queue_storage.txt", ios::in);
    while (!out_file.eof()) {
        string line;
        out_file >> line;
        if (line!=""){
            client_data = split(line);
            get_client_by_data(client_data, search_type, count, valid);
            count++;
        }
    }
    return valid;
}

void get_client_by_data(vector<string> client_data, string &search_type, int count, bool &valid) {
    if (client_data[0] == search_type || client_data[1] == search_type) {
        cout << '\n' << client_data[0] << setw(25)
             << client_data[1] << setw(25);
        if (count != 0) {
            if (stoi(client_data[3]) != 0) {
                cout << "Number in Queue: " << client_data[3] <<
                     setw(25) << "waiting list: " << count << endl;
            } else {
                cout << "waiting list: " << count << endl;
            }
        } else {
            cout << "Being serviced now\n";
        }
        valid = true;
    }
}

void rewrite_vip_file(string &mobile_number) {
    fstream out_file("text_files/VIP_Clients.txt", ios::app);
    out_file << endl << mobile_number << endl;
}
