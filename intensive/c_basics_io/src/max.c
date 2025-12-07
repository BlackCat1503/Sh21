#include <stdio.h>

int max(int a, int b);

int max(int a, int b) {
  if (a > b) {
    return a;
  }
  if (a < b) {
    return b;
  } else {
    return a;
  }
}

int main() {
  double a, b;
  scanf("%lf %lf", &a, &b);
  if ((a == (int)a) && (b == (int)b)) {
    printf("%d\n", max(a, b));
    return 0;
  } else {
    printf("n/a\n");
    return 0;
  }
}