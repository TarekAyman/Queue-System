#include <iostream>
#include "../header_files/Client_Service.h"
#include <iomanip>

using namespace std;

struct Node {
    Client_Service client_service;
    Node *next = nullptr;
};

struct Queue {
    Node *front = nullptr;
    Node *rear = nullptr;
};

Queue vip_clients, ordinary_clients;

void enqueue_vip(Client_Service &client_service);

pair<bool, int> is_exist(string &mobile_number, Queue queue);

void vip_client_already_exist(Client_Service &client_service);

bool empty(Queue queue);

void enqueue_ordinary_client(Client_Service &client_service);

void ordinary_client_already_exist(Client_Service &client_service);

void diplay_current_added_client(Client_Service& client_service);

void dequeue();

void dequeue_vip_clients();

void dequeue_ordinary_clients();

void print_all_clients();

void print_ordinary_clients(Node *current, int count);

void print_vip_clients();

void print_vip_clients(Node *current, int count);

int size(Queue queue);

Client_Service get_vip_client(int position);

Client_Service get_ordinary_client(int position);

int number_in_queue = 0;

bool empty(Queue queue) {
    return (queue.front == nullptr && queue.rear == nullptr);
}

void enqueue_vip(Client_Service &client_service) {
    if (is_exist(client_service.client.mobile_number, vip_clients).first) {
        vip_client_already_exist(client_service);
    } else {
        Node *new_client = new Node;
        new_client->client_service = client_service;
        if (empty(vip_clients))
            vip_clients.front = vip_clients.rear = new_client;
        else {
            vip_clients.rear->next = new_client;
            vip_clients.rear = new_client;
        }
    }
}

pair<bool, int> is_exist(string &mobile_number, Queue queue) {
    pair<bool, int> pair(false, -1);
    Node *current = queue.front;
    int index = 1;
    while (current != nullptr && current->client_service.
            client.mobile_number != mobile_number) {
        current = current->next;
        index++;
    }
    if (current != nullptr) {
        pair.first = true;
        pair.second = index;
        return pair;
    } else {
        return pair;
    }
}

void vip_client_already_exist(Client_Service &client_service) {
    cout << "Client already exist\n";
    int index = is_exist(client_service.client.mobile_number,
                         vip_clients).second;
    client_service = get_vip_client(index);
    cout << client_service.client.mobile_number
         << setw(25) << client_service.name
         << setw(25) << "Waiting list: " << index << endl;
}

void enqueue_ordinary_client(Client_Service &client_service) {
    if (is_exist(client_service.client.mobile_number,
                 ordinary_clients).first) {
        ordinary_client_already_exist(client_service);
    } else {
        Node *new_client = new Node;
        if (client_service.number == 0) {
            client_service.number = ++number_in_queue;
        }
        new_client->client_service = client_service;
        if (empty(ordinary_clients))
            ordinary_clients.front = ordinary_clients.rear = new_client;
        else {
            ordinary_clients.rear->next = new_client;
            ordinary_clients.rear = new_client;
        }
    }
}

void ordinary_client_already_exist(Client_Service &client_service) {
    cout << "Client already exist\n";
    int index = is_exist(client_service.client.mobile_number,
                         ordinary_clients).second;
    client_service = get_ordinary_client(index);
    cout << client_service.client.mobile_number
         << setw(25) << client_service.name
         << setw(25) << "Number in Queue: " << client_service.number
         << setw(25) << "Waiting list: " << (size(vip_clients) + index) - 1
         << endl;
}

void diplay_current_added_client(Client_Service& client_service) {
    if (client_service.client.is_vip) {
        cout << client_service.client.mobile_number
            << setw(25) << client_service.name
            << setw(25) << "Waiting list: " << size(vip_clients) << endl;
    }else {
        cout << client_service.client.mobile_number
            << setw(25) << client_service.name
            << setw(25) << "Number in Queue: " << client_service.number
            << setw(25) << "Waiting list: " << (size(vip_clients) + size(ordinary_clients)) - 1
            << endl;
    }
}

void dequeue() {
    if (empty(vip_clients) && empty(ordinary_clients))
        cout << "Queue is empty";
    else if (!empty(vip_clients))
        dequeue_vip_clients();
    else
        dequeue_ordinary_clients();
}

void dequeue_vip_clients() {
    if (!empty(vip_clients)) {
        Node *current = vip_clients.front;
        vip_clients.front = vip_clients.front->next;
        cout << "Client with mobile number: " << current->client_service.client.
                mobile_number << " has already finished" << endl;
        delete current;
    }
    if (vip_clients.front == nullptr)
        vip_clients.front = vip_clients.rear = nullptr;
}

void dequeue_ordinary_clients() {
    if (!empty(ordinary_clients)) {
        Node *current = ordinary_clients.front;
        ordinary_clients.front = ordinary_clients.front->next;
        cout << "Client with mobile number: " << current->client_service.client.
                mobile_number << " has already finished" << endl;
        delete current;
    }
    if (ordinary_clients.front == nullptr)
        ordinary_clients.front = ordinary_clients.rear = nullptr;
}

void print_all_clients() {
    if (empty(ordinary_clients) && empty(vip_clients))
        cout << "Queue is empty";

    if (!empty(vip_clients)) {
        print_vip_clients(vip_clients.front, 0);
    }

    if (!empty(ordinary_clients)) {
        print_ordinary_clients(ordinary_clients.front,
            size(vip_clients));
    }
}

void print_ordinary_clients(Node *current, int count) {
    if (current != nullptr) {
        cout << '\n' << current->client_service.client.mobile_number
             << setw(25) << current->client_service.name
             << setw(25) << "Number in Queue: " << current->client_service.number;
        if (count != 0)
            cout << setw(25) << "waiting list: " << count << endl;
        else
            cout << setw(25) << "Being serviced now" << endl;
        print_ordinary_clients(current->next, count + 1);
    }
}

void print_vip_clients() {
    if (vip_clients.front != nullptr)
        print_vip_clients(vip_clients.front, 0);
    else
        cout << "Queue is empty";
}

void print_vip_clients(Node *current, int count) {
    if (current != nullptr) {
        cout << '\n' << current->client_service.client.mobile_number
             << setw(25) << current->client_service.name;
        if (count != 0)
            cout << setw(51) << "waiting list: " << count << endl;
        else
            cout << setw(26) << "Being serviced now" << endl;
        print_vip_clients(current->next, count + 1);
    }
}

int size(Queue queue) {
    Node *current = queue.front;
    int count = 0;
    while (current != nullptr) {
        current = current->next;
        count++;
    }
    return count;
}

Client_Service get_vip_client(int position) {
    Node *current = vip_clients.front;
    for (int i = 1; i < position; ++i) {
        current = current->next;
    }
    return current->client_service;
}

Client_Service get_ordinary_client(int position) {
    Node *current = ordinary_clients.front;
    for (int i = 1; i < position; ++i) {
        current = current->next;
    }
    return current->client_service;
}
