int main() {
    int sum = 0;
    int i;
    for (;1;) {
        i = i + 1;
        if (i / 2 == (i + 1) / 2)
            continue;
        sum = sum + i;
    }
    return sum;
}