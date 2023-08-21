int sum(int k);
int putint(int k);
int main() {
  return sum(10);
}

int sum(int k) {
  if (k > 0) {
    return k + sum(k - 1);
  } else {
    return 0;
  }
}