#!/usr/bin/env python3


class RCompf:
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
        while True:
            print(self.str[self.index], end='')
            self.index += 1
            if self.index == len(self.str) or self.str[self.index] in ['+', '-', '*', '/', ')', ']', '}']:
                print('', end=' ')
                break


if __name__ == '__main__':
    c = RCompf()
    while True:
        str = input("Арифметическая  формула: ")
        print("Результат её компиляции:", end=" ")
        c.compile(str)
        print("\n")