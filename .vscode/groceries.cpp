#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// Define structures for Customer and Item
struct Customer {
    int id;
    string name;
    string street;
    string city;
    string state;
    string zip;
    string phone;
    string email;
};

struct Item {
    int id;
    string description;
    double price;
};

// Global vectors to store customers and items
vector<Customer> customers;
vector<Item> items;

// Function prototypes
void read_customers(const string& filename);
void read_items(const string& filename);
void one_customer_order();

// Function to split a string into tokens
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to read customer data from file
void read_customers(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            vector<string> data = split(line, ',');
            Customer customer;
            customer.id = stoi(data[0]);
            customer.name = data[1];
            customer.street = data[2];
            customer.city = data[3];
            customer.state = data[4];
            customer.zip = data[5];
            customer.phone = data[6];
            customer.email = data[7];
            customers.push_back(customer);
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << filename << endl;
        exit(1);
    }
}

// Function to read item data from file
void read_items(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            vector<string> data = split(line, ',');
            Item item;
            item.id = stoi(data[0]);
            item.description = data[1];
            item.price = stod(data[2]);
            items.push_back(item);
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << filename << endl;
        exit(1);
    }
}

// Function to process one customer's order
void one_customer_order() {
    // Display number of customers and items
    cout << "Number of customers: " << customers.size() << endl;
    cout << "Number of items: " << items.size() << endl;

    // Prompt for customer number
    int customer_id;
    cout << "Enter customer number: ";
    cin >> customer_id;

    // Find customer record
    bool found_customer = false;
    for (const auto& customer : customers) {
        if (customer.id == customer_id) {
            found_customer = true;
            break;
        }
    }

    if (!found_customer) {
        cerr << "Customer not found!" << endl;
        exit(1);
    }

    // Process items for purchase
    int total_items = 0;
    double total_cost = 0.0;
    int item_number;
    while (true) {
        cout << "Enter item number to purchase (0 to finish): ";
        cin >> item_number;
        if (item_number == 0) break;

        bool found_item = false;
        for (const auto& item : items) {
            if (item.id == item_number) {
                found_item = true;
                cout << "Item description: " << item.description << ", Price: $" << item.price << endl;
                total_items++;
                total_cost += item.price;
                break;
            }
        }
        if (!found_item) {
            cerr << "Item not found!" << endl;
        }
    }

    // Display order summary
    cout << "Number of items purchased: " << total_items << endl;
    cout << "Total cost: $" << fixed << setprecision(2) << total_cost << endl;
}

int main() {
    read_customers("customers.txt");
    read_items("items.txt");
    one_customer_order();
    return 0;
}
