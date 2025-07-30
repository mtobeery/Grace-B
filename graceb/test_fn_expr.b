fn add(a, b) {
  return a + b;
}

int result = add(4, 6);
print(result);

int nested = add(add(1, 2), 3);
print(nested);
