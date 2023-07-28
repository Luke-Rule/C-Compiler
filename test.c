int main() {
    int a = 2;
    int b = 3;
    {
        int a = 1;
        return b + a;
    }
    return b;
}