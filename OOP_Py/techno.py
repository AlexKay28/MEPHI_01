from functools import partial
def f(a, b):
    if b == 0:
        return 1
    if b % 2 == 0:
        return f(a, b / 2) ** 2
    else:
        print("HUI")


g = partial(f, b=3)
print(g(2))
