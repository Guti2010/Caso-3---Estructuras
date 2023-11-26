#include <iostream>
#include "lib/DallEapi.cpp"
#include <string>
using namespace std;

int main(){
    DallE imageCreator;
    string url = imageCreator.getImage("a cat with a hat on");
    cout << url << endl;
}