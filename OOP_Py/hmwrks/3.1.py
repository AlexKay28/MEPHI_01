
def cardinality_cube(a, b, c):
    if a == 0:
        if b == 0 and c == 0: return "continuum"
        elif b == 0 or c == 0: return 1
        else: return 2
    else:
        if b == 0 and c == 0: return 1
        elif c == 0: return 2
        else:
            d = b * b - 4 * a * c
            if d < 0: return 1
            elif d > 0: return 3
            else: return 2

