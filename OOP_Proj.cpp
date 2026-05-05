#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

// Base class (Inheritance)
class Product {
protected:
    int id;
    string name;
    double price;

public:
    Product(int id, string name, double price) {
        this->id = id;
        this->name = name;
        this->price = price;
    }

    virtual void display() = 0; // Polymorphism

    int getId() { return id; }
    string getName() { return name; }
    double getPrice() { return price; }

    virtual ~Product() {}
};

// Derived class 1
class Electronics : public Product {
    int warranty;

public:
    Electronics(int id, string name, double price, int warranty)
        : Product(id, name, price) {
        this->warranty = warranty;
    }

    void display() override {
        cout << id << " | " << name << " | $" << price
            << " | Warranty: " << warranty << " years" << endl;
    }
};

// Derived class 2
class Clothing : public Product {
    string size;

public:
    Clothing(int id, string name, double price, string size)
        : Product(id, name, price) {
        this->size = size;
    }

    void display() override {
        cout << id << " | " << name << " | $" << price
            << " | Size: " << size << endl;
    }
};

// Cart class (Association)
class Cart {
    vector<Product*> items;

public:
    void addProduct(Product* p) {
        items.push_back(p);
        cout << "Product added to cart." << endl;
    }

    void viewCart() {
        if (items.empty()) {
            cout << "Cart is empty." << endl;
            return;
        }
        for (auto p : items) {
            p->display();
        }
    }

    double calculateTotal() {
        double total = 0;
        for (auto p : items) {
            total += p->getPrice();
        }
        return total;
    }

    void clearCart() {
        items.clear();
    }
};

// User class
class User {
    string username;
    Cart cart;

public:
    User(string username) {
        this->username = username;
    }

    Cart& getCart() { return cart; }
    string getUsername() { return username; }
};

// File handling class
class FileManager {
public:
    static void saveOrder(string username, double total) {
        ofstream file("orders.txt", ios::app);
        if (!file) {
            cout << "Error saving order." << endl;
            return;
        }
        file << "User: " << username << " | Total: $" << total << endl;
        file.close();
    }
};

// Main system
int main() {
    vector<Product*> products;

    // Sample products
    products.push_back(new Electronics(1, "Laptop", 800, 2));
    products.push_back(new Electronics(2, "Phone", 500, 1));
    products.push_back(new Clothing(3, "T-Shirt", 20, "M"));
    products.push_back(new Clothing(4, "Jeans", 40, "L"));

    string name;
    cout << "Enter username: ";
    cin >> name;

    User user(name);

    int choice;
    do {
        cout << "\n1. View Products\n";
        cout << "2. Search Product\n";
        cout << "3. Add to Cart\n";
        cout << "4. View Cart\n";
        cout << "5. Checkout\n";
        cout << "0. Exit\n";
        cout << "Choice: ";

        cin >> choice;

        if (cin.fail()) {
            cout << "Invalid input!" << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
        case 1:
            for (auto p : products)
                p->display();
            break;

        case 2: {
            string search;
            cout << "Enter name: ";
            cin >> search;
            bool found = false;

            for (auto p : products) {
                if (p->getName().find(search) != string::npos) {
                    p->display();
                    found = true;
                }
            }

            if (!found)
                cout << "Product not found." << endl;
            break;
        }

        case 3: {
            int id;
            cout << "Enter product ID: ";
            cin >> id;

            Product* selected = nullptr;
            for (auto p : products) {
                if (p->getId() == id)
                    selected = p;
            }

            if (selected)
                user.getCart().addProduct(selected);
            else
                cout << "Invalid product ID." << endl;
            break;
        }

        case 4:
            user.getCart().viewCart();
            break;

        case 5: {
            double total = user.getCart().calculateTotal();
            cout << "Total: $" << total << endl;
            FileManager::saveOrder(user.getUsername(), total);
            user.getCart().clearCart();
            cout << "Order placed successfully!" << endl;
            break;
        }

        case 0:
            cout << "Goodbye!" << endl;
            break;

        default:
            cout << "Invalid choice!" << endl;
        }

    } while (choice != 0);

    // Cleanup memory
    for (auto p : products)
        delete p;

    return 0;
}
