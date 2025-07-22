#include <bits/stdc++.h>
using namespace std;

enum class services {
    payment = 1,
    checkBalance = 2,
    deposit = 3,
    withdraw = 4
};

// Cannot be string, 
//In C++, enum or enum class values must be integral types 
//(like int, char, unsigned int, etc.). 
//You cannot use std::string or string literals as enum values directly.

// enum class services2 : string{
//     payment = "Payment Service",
//     checkBalance = "Check Balance Service",
//     deposit = "Deposit Service",
//     withdraw = "Withdraw Service"
// }


enum class Service2 {
    Payment,
    CheckBalance,
    Deposit,
    Withdraw
};

std::string to_string(Service2 s) {
    switch (s) {
        case Service::Payment:      return "Payment Service";
        case Service::CheckBalance: return "Check Balance Service";
        case Service::Deposit:      return "Deposit Service";
        case Service::Withdraw:     return "Withdraw Service";
        default:                    return "Unknown Service";
    }
}


int main(){
    services service = payment;
    cout<<service<<endl;

    service = checkBalance;
    cout<<service<<endl;

    service = deposit;
    cout<<service<<endl;

    service = services::withdraw;
    cout<<service<<endl;

    Service s = Service::Deposit;
    std::cout << to_string(s) << std::endl;  // Output: Deposit Service

    return 0;
}