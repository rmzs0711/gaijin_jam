#include <set>
struct B;
struct A {
    B& b;
};
struct B {
    A a;
};
int main() {

    return 0;
}
