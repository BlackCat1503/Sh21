#include <stdio.h>

int sum(int a, int b);
int dif(int a, int b);
int mult(int a, int b);
int div(int a, int b);

int sum(int a, int b) { return a + b; }

int dif(int a, int b) { return a - b; }

int mult(int a, int b) { return a * b; }

int div(int a, int b) { return a / b; }

int main() {
  double a, b;
  scanf("%lf %lf", &a, &b);
  if ((a == (int)a) && (b == (int)b)) {
    if (b == 0) {
      printf("%d %d %d n/a\n", sum(a, b), dif(a, b), mult(a, b));
      return 0;
    } else {
      printf("%d %d %d %d\n", sum(a, b), dif(a, b), mult(a, b), div(a, b));
      return 0;
    }
  } else {
    printf("n/a");
    return 0;
  }
}
