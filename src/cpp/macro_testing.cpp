
void FOO0(){}
void FOO1(int a){}
void FOO2(int a, int b){}
void FOO3(int a, int b, int c){}

// #define GET_MACRO(_1,_2,_3,NAME,...) NAME
// #define FOO(name, ...) GET_MACRO(__VA_ARGS__, FOO3, FOO2)(__VA_ARGS__)
template<class...T>
class Final{};

#define FOO(name, ...) Final<__VA_ARGS__> name;
#define FOO_E(name) Final<> name;

int main() {
    int a,b,c;
    FOO(newv, float, double)
    FOO_E(newb)
}