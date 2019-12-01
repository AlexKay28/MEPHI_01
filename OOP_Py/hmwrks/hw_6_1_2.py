from math import gcd


class Frac:
    # n - numerator (числитель)
    # d - denominator (знаменатель)
    def __init__(self, n: int, d: int):
        if (d == 0):
            raise ZeroDivisionError
        if d < 0 and n < 0:
            d = d * -1
            n = n * -1
        t = gcd(n, d)
        self.n = n // t
        self.d = d // t
        self.n_float = n
        self.d_float = d

    # разность между «самим» объектом (self) и каким-то иным (other)
    def __sub__(self, other):
        return Frac(self.n * other.d - other.n * self.d, self.d * other.d)

    # сумма
    def __add__(self, other):
        try:
            return Frac(self.n * other.d + other.n * self.d, self.d * other.d)
        except AttributeError:
            return Frac(self.n + self.d * other, self.d)

    # операции сравнения
    def __lt__(self, other):
        try:
            return self.n_float * other.d_float < other.n_float * self.d_float
        except AttributeError:
            return self.n < other * self.d

    def __eq__(self, other):
        try:
            return self.n_float * other.d_float == other.n_float * self.d_float
        except AttributeError:
            return self.n == other * self.d

    def __gt__(self, other):
        try:
            return self.n_float * other.d_float > other.n_float * self.d_float
        except AttributeError:
            return self.n > other * self.d

    # умножение
    def __mul__(self, other):
        try:
            return Frac(self.n * other.n, self.d * other.d)
        except AttributeError:
            return Frac(self.n * other, self.d)

    # деление
    def __truediv__(self, other):
        try:
            return Frac(self.n * other.d, self.d * other.n)
        except AttributeError:
            return Frac(self.n, self.d * other)

    # представление объекта в виде строки
    def __repr__(self):
        if self.d == 1:
            return f"{self.n}"
        return f"{self.n}/{self.d}"

a = Frac(4, 9)
b = Frac(2, 6)
print('a is [', a, '] b is [', b, ']')
try:
    print('this is a + b: ', a, ' + ', b, ' = ', a + b)
except:
    print("Не работает!")
try:
    print('this is a * b: ', a, ' * ', b, ' = ', a * b)
except:
    print("Не работает!")
try:
    print('this is a / b: ', a, ' : ', b, ' = ', a / b)
except:
    print("Не работает!")
try:
    print('this is a + 2: ', a, ' + ', 5, ' = ', a + 5)
except:
    print("Не работает!")
try:
    print('here we\'re comparing that a < 1: where a is ', a, ' so ', a < 1)
except:
    print("Не работает!")
try:
    print('here we\'re comparing big and small numbers: ', Frac(1111111111111111113, 10**20) == Frac(1111111111111111112, 10**20))
except:
    print("Не работает!")

print(Frac(-1, -3))