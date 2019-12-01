#!/usr/bin/env python3


class RCompf:
    """
    Рекурсивный компилятор формул преобразует правильные
    арифметические формулы (цепочки языка, задаваемого
    грамматикой Gf) в программы для стекового калькулятора
    (цепочки языка, определяемого грамматикой Gs):

    Gf:
         F  ->  T  |  T+F  |  T-F
         T  ->  M  |  M*T  |  M/T
         M  -> (F) |   V
         V  ->  a  |   b   |   c   |  ...  |    z

    Gs:
         e  ->  e e + | e e - | e e * | e e / |
                | a | b | ... | z

    """

    def compile(self, str):
        self.str, self.index = str, 0
        self.compile_f()

    # Компилировать формулу, используя правила
    # F -> T | T+F | T-F
    def compile_f(self):
        self.compile_t()
        if self.index >= len(self.str):
            return
        cur = self.str[self.index]
        if cur == '+' or cur == '-':
            self.index += 1
            self.compile_f()
            print(cur, end=" ")

    # Компилировать терм, используя правила
    # T -> M | M*T | M/T
    def compile_t(self):
        self.compile_m()
        if self.index >= len(self.str):
            return
        cur = self.str[self.index]
        if cur == '*' or cur == '/':
            self.index += 1
            self.compile_t()
            print(cur, end=" ")

    # Компилировать множитель, используя правила
    # M -> (F) | V
    def compile_m(self):
        if self.str[self.index] == "(" \
                or self.str[self.index] == "[" \
                or self.str[self.index] == "{":
            self.index += 1
            self.compile_f()
            self.index += 1
        else:
            self.compile_v()

    # Компилировать имя переменной, используя правила
    # V -> a | b | ... | z
    def compile_v(self):
        print(self.str[self.index], end=" ")
        self.index += 1


if __name__ == '__main__':
    c = RCompf()
    while True:
        str = input("Арифметическая  формула: ")
        print("Результат её компиляции:", end=" ")
        c.compile(str)
        print("\n")