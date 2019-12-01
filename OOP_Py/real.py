def f(x: float, y: float):
    rad = ((3 - x)**2 + (6 - y)**2)**0.5
    return True if (3 < rad < 5) and (-1 <= x <= 1) and (1 <= y <= 2) else False

print('a' == 'A')