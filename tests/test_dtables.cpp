#include <iostream>
#include <yaml-cpp/yaml.h>

using namespace std;

int main() {
    YAML::Node test = YAML::LoadFile("test_data/test.yaml");
    if (test["key_01"]) {
      cout << "HELLO";
    }else{
      cout<< "test";
    }

    return 0;
}