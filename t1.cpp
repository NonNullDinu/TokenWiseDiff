#include <iostream>
using namespace std;

int main(){
    int a = 1, b = 2, c = a + b;
    for(int i = 1; i <= c; i++){
        a += i;
    }
    for(int i = 1; i <= c; i++){
        a += i;
    }
    for(int i = 1; i <= c; i++){
        a += i;
    }
    for(int i = 1; i <= c; i++){
        a += i;
    }
    cout << a;
    return 0;
}