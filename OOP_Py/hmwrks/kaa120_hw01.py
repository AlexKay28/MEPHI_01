def f(x: float, y: float):
    radius = ((3 - x)**2 + (6 - y)**2)**0.5
    return True if (3 < radius < 5) and (-1 <= x <= 1) and (1 <= y <= 2) else False


def g(a: int, b: int, c: int):
    # (a*x + 3)(c^2*x^2 + 2cbx + b^2-5^2)
    A, B, C = c**2, 2*c*b, b**2 - 25
    if A != 0:
        # Уравнение второй скобки является квадратным
        des = B**2 - 4*A*C
        if des < 0:
            result = 0
        elif des > 0:
            result = 2
        else:
            result = 1
    else:
        # Уравнение второй скобки не является квадратным
        if B != 0:
            # Уравнение второй скобки является линейным
            result = 1
        else:
            if C == 0:
                return "continuum"
            else:
                result = 0
    return str(result) if a == 0 else str(result + 1)

