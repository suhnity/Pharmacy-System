#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

class Customer {
public:
    string name;
    string address;
    string phoneNumber;
    string cardNumber;
    string pinCode;
    vector<pair<string, int>> orders;

    Customer(const string& name, const string& address) : name(name), address(address) {}
};

class Medicine {
public:
    string name;
    double price;

    Medicine(const string& name, double price) : name(name), price(price) {}
};

class PharmacySystem {
private:
    vector<Customer> customers;
    vector<Medicine> medicines;

public:
    void addCustomer(const Customer& customer) {
        customers.push_back(customer);
    }

    void addMedicine(const Medicine& medicine) {
        medicines.push_back(medicine);
    }

    void displayMedicineList() const {
        cout << "=================================== Medicine List ==================================" << endl;
        for (const auto& medicine : medicines) {
            cout << medicine.name << " - PHP " << fixed << setprecision(2) << medicine.price << endl;
        }
    }

    void displayCustomers() const {
        cout << "=================================== Customer List ==================================" << endl;
        for (const auto& customer : customers) {
            cout << "Name: " << customer.name << ", Address: " << customer.address << endl;
            cout << "Order Summary:" << endl;
            for (const auto& order : customer.orders) {
                cout << "Medicine: " << order.first << ", Quantity: " << order.second << endl;
            }
            cout << "Total Price: PHP " << fixed << setprecision(2) << computeTotalPrice(customer.orders) << endl;
        }
    }

    double computeTotalPrice(const vector<pair<string, int>>& items) const {
        double totalPrice = 0.0;
        for (const auto& item : items) {
            for (const auto& medicine : medicines) {
                if (compareIgnoreCase(medicine.name, item.first)) {
                    totalPrice += medicine.price * item.second;
                    break;
                }
            }
        }
        return totalPrice;
    }

    void takeOrder(Customer& customer) {
        string input;
        do {
            displayMedicineList();
            string medicineName;
            int quantity;

            cout << "====================================================================================" << endl;
            cout << "Enter the name of the medicine you want to order (press enter to finish): ";
            getline(cin, medicineName);

            if (medicineName.empty()) {
                break;
            }

            cout << "Enter the quantity: ";
            cin >> quantity;
            cin.ignore();

            bool validMedicine = any_of(medicines.begin(), medicines.end(), [&](const Medicine& m) {
                return compareIgnoreCase(m.name, medicineName);
            });

            if (!validMedicine) {
                cout << "Invalid medicine name." << endl;
                continue;
            }

            customer.orders.emplace_back(medicineName, quantity);

            cout << "Do you want to add more orders? (y/n) ";
            getline(cin, input);
        } while (input == "y" || input == "Y");
    }

    bool compareIgnoreCase(const string& str1, const string& str2) const {
        return equal(str1.begin(), str1.end(), str2.begin(), str2.end(), [](char c1, char c2) {
            return tolower(c1) == tolower(c2);
        });
    }

    bool validatePhoneNumber(const string& phoneNumber) const {
        return phoneNumber.length() == 11;
    }

    bool validateCardNumber(const string& cardNumber) const {
        return cardNumber.length() == 16;
    }

    bool validatePinCode(const string& pinCode) const {
        return pinCode.length() == 4;
    }

    void processOrder(Customer& customer) {
        double totalPrice = computeTotalPrice(customer.orders);

        cout << "=================================== Order Summary ==================================" << endl;
        cout << "Customer: " << customer.name << ", Address: " << customer.address << endl;
        cout << "Medicine(s):" << endl;
        for (const auto& order : customer.orders) {
            cout << "Name: " << order.first << ", Quantity: " << order.second << endl;
        }
        cout << "Total Price: PHP " << fixed << setprecision(2) << totalPrice << endl;
        cout << "================================== Payment Process =================================" << endl;

        handlePayment(customer, totalPrice);
    }

    void handlePayment(Customer& customer, double totalPrice) {
        string paymentMethod;
        do {
            cout << "Choose a payment method (e-cash, bank, cash on delivery): ";
            getline(cin, paymentMethod);
        } while (paymentMethod != "e-cash" && paymentMethod != "bank" && paymentMethod != "cash on delivery");

        if (paymentMethod == "e-cash") {
            handleECashPayment(customer, totalPrice);
        } else if (paymentMethod == "bank") {
            handleBankPayment(customer, totalPrice);
        } else if (paymentMethod == "cash on delivery") {
            cout << "Confirm order? (y/n): ";
            string confirm;
            getline(cin, confirm);
            if (confirm != "y" && confirm != "Y") {
                cout << "Order canceled." << endl;
                return;
            }
            cout << "Order successful. Payment will be done on delivery." << endl;
        }
    }

    void handleECashPayment(Customer& customer, double totalPrice) {
        string phoneNumber;

        do {
            cout << "Enter your phone number: ";
            getline(cin, phoneNumber);
        } while (!validatePhoneNumber(phoneNumber));

        customer.phoneNumber = phoneNumber;

        double payment;
        do {
            cout << "------------------------------------------------------------------------------------" << endl;
            cout << "Enter the amount: PHP ";
            cin >> payment;
            cin.ignore();

            if (payment < totalPrice) {
                cout << "Insufficient payment amount." << endl;
            } else {
                cout << "Payment successful." << endl;
                if (payment > totalPrice) {
                    double change = payment - totalPrice;
                    cout << "Change: PHP " << fixed << setprecision(2) << change << endl;
                }
            }
        } while (payment < totalPrice);
    }

    void handleBankPayment(Customer& customer, double totalPrice) {
        string cardNumber, pinCode;

        do {
            cout << "Enter your card number (16 digits): ";
            getline(cin, cardNumber);
        } while (!validateCardNumber(cardNumber));

        do {
            cout << "Enter your 4-digit pin code: ";
            getline(cin, pinCode);
        } while (!validatePinCode(pinCode));

        customer.cardNumber = cardNumber;
        customer.pinCode = pinCode;

        double payment;
        do {
            cout << "------------------------------------------------------------------------------------" << endl;
            cout << "Enter the amount: PHP ";
            cin >> payment;
            cin.ignore();

            if (payment < totalPrice) {
                cout << "Insufficient payment amount." << endl;
            } else {
                cout << "Payment successful." << endl;
                if (payment > totalPrice) {
                    double change = payment - totalPrice;
                    cout << "Change: PHP " << fixed << setprecision(2) << change << endl;
                }
            }
        } while (payment < totalPrice);
    }
};

int main() {
    PharmacySystem pharmacySystem;

    cout << "=============================== Customer Information ===============================" << endl;
    string name, address;
    cout << "Enter your name: ";
    getline(cin, name);

    cout << "Enter your address: ";
    getline(cin, address);

    Customer customer(name, address);
    pharmacySystem.addCustomer(customer);

    Medicine medicine1("Bioflu", 9.00);
    Medicine medicine2("Biogesic", 4.75);
    Medicine medicine3("Neozep", 6.70);
    Medicine medicine4("Alaxan", 8.95);
    Medicine medicine5("Medicol", 6.75);
    Medicine medicine6("Liveraide", 21.17);

    pharmacySystem.addMedicine(medicine1);
    pharmacySystem.addMedicine(medicine2);
    pharmacySystem.addMedicine(medicine3);
    pharmacySystem.addMedicine(medicine4);
    pharmacySystem.addMedicine(medicine5);
    pharmacySystem.addMedicine(medicine6);

    char orderOption;
    do {
        pharmacySystem.takeOrder(customer);
        cout << "Do you want to proceed to checkout? (y/n) ";
        cin >> orderOption;
        cin.ignore();
    } while (orderOption == 'n' || orderOption == 'N');

    pharmacySystem.processOrder(customer);

    pharmacySystem.displayCustomers();

    return 0;
}
