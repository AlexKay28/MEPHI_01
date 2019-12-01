from math import gcd


class Frac:
    # n - numerator (числитель)
    # d - denominator (знаменатель)
    def __init__(self, n, d):
        if (d == 0):
            raise ZeroDivisionError
        if d < 0 and n < 0:
            d = d * -1
            n = n * -1
        t = gcd(n, d)
        self.n = n // t
        self.d = d // t

    # разность между «самим» объектом (self) и каким-то иным (other)
    def __sub__(self, other):
        return Frac(self.n * other.d - other.n * self.d, self.d * other.d)

    # сумма
    def __add__(self, other):
        try:
            return Frac(self.n * other.d + other.n * self.d, self.d * other.d)
        except AttributeError:
            return Frac(self.n + self.d, self.d)

    def __lt__(self, other):
        try:
            return self.n/self.d < other.n/other.d
        except AttributeError:
            return self.n/self.d < other

    def __mul__(self, other):
        return Frac(self.n * other.n, self.d * other.d)

    def __truediv__(self, other):
        return Frac(self.n * other.d, self.d * other.n)

    def __eq__(self, other):
        return self.n/self.d == other.n/other.d

    # представление объекта в виде строки
    def __repr__(self):
        return f"{self.n}/{self.d}"

a = Frac(1, 2)
b = Frac(2, 6)
try:
    print(a + b)
except:
    print("Не работает!")
try:
    print(a * b)
except:
    print("Не работает!")
try:
    print(a / b)
except:
    print("Не работает!")
try:
    print(a + 1)
except:
    print("Не работает!")
try:
    print(a < 1)
except:
    print("Не работает!")
try:
    print(Frac(0, 1) == Frac(0, 1))
except:
    print("Не работает!")

print(Frac(-1, -3))