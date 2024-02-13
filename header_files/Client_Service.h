#include <string>

using namespace std;

struct Client {
    string mobile_number;
    bool is_vip = false;
};

struct Client_Service {
    Client client;
    string name;
    int number = 0;
};
