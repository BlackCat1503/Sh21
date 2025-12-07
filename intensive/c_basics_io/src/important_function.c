#include <math.h>
#include <stdio.h>

double func(double x) {
  double y;
  y = 7e-3 * pow(x, 4) +
      ((22.8 * pow(x, (1 / 3)) - 1e3) * x + 3) / (x * x / 2) -
      x * pow((10 + x), (2 / x)) - 1.01;
  return y;
}

int main() {
  double x;
  scanf("%lf", &x);
  if ((x == (int)x)) {
    printf("%.1lf\n", func(x));
    return 0;
  } else {
    printf("n/a\n");
    return 0;
  }
}