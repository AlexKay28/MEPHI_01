# коэффициенты для определения уравнения прямой

x1, y1 = 2, 0.5   # 1st point - p
x2, y2 = 2, 0  # 2nd point - q

A = y1 - y2
B = x2 - x1
C = x1 * y2 - x2 * y1

# поиск точек пересечения

R1, R2 = 1, 2
x0 = -(A*C)/(A**2 + B**2)
y0 = -(B*C)/(A**2 + B**2)

check = (x0**2 + y0**2)**0.5

# 1st circle
d1 = (R1**2 - C**2/(A**2 + B**2))**0.5
if check > R1:
    print('no points of intersection')
elif check == R1:
    mult = (d1 ** 2 / (A ** 2 + B ** 2)) ** 0.5
    ax_1, ay_1 = x0 + B * mult, y0 - A * mult
    print('only one point')
    print('points on the bigger  cricle: [', ax_1, ay_1, ']')
else:
    mult = (d1 ** 2 / (A ** 2 + B ** 2)) ** 0.5
    ax_1, ay_1 = x0 + B * mult, y0 - A * mult
    bx_1, by_1 = x0 - B * mult, y0 + A * mult
    print('three points')
    print('points on the smaller cirlce: [', ax_1, ay_1, '] and [', bx_1, by_1, '] and distance -', d1 * 2)

# 2nd circle
d2 = (R2**2 - C**2/(A**2 + B**2))**0.5
if check > R2:
    print('no points of intersection')
elif check == R2:
    mult = (d2 ** 2 / (A ** 2 + B ** 2)) ** 0.5
    ax_2, ay_2 = x0 + B * mult, y0 - A * mult
    print('only one point')
    print('points on the bigger  cricle: [', ax_2, ay_2, ']')
else:
    mult = (d2**2/(A**2 + B**2))**0.5
    ax_2, ay_2 = x0 + B*mult, y0 - A*mult
    bx_2, by_2 = x0 - B*mult, y0 + A*mult
    print('three points')
    print('points on the bigger  cricle: [', ax_2, ay_2, '] and [', bx_2, by_2, '] and distance -', d2 * 2)


# выверить реальные точки






