from deq import Deq
from r2point import R2Point
from math import sqrt


def lenght(p, q):
    # коэффициенты для определения уравнения прямой для точек p, q
    coef_a = p.y - q.y
    coef_b = q.x - p.x
    coef_c = p.x * q.y - q.x * p.y

    print('points are: ', p.x, p.y, ' and ', q.x, q.y)
    print(coef_a, coef_b, coef_c)
    # поиск точек пересечений
    R1, R2 = 1, 2
    x0 = -(coef_a * coef_c) / (coef_a ** 2 + coef_b ** 2)
    y0 = -(coef_b * coef_c) / (coef_a ** 2 + coef_b ** 2)

    # расстояние точки нормали от центра оси
    check = (x0 ** 2 + y0 ** 2) ** 0.5

    no_1st_intersections = False
    no_2nd_intersections = False

    list_of_pret = []

    # 1st circle
    d1 = (R1 ** 2 - coef_c ** 2 / (coef_a ** 2 + coef_b ** 2)) ** 0.5
    if check > R1:
        no_1st_intersections = True
        # print('no points of intersection')
    elif check == R1:
        mul = (d1 ** 2 / (coef_a ** 2 + coef_b ** 2)) ** 0.5
        ax_1, ay_1 = x0 + coef_b * mul, y0 - coef_a * mul
        tm1 = R2Point(ax_1, ay_1)
        list_of_pret.append(tm1)
    else:
        mul = (d1 ** 2 / (coef_a ** 2 + coef_b ** 2)) ** 0.5
        ax_1, ay_1 = x0 + coef_b * mul, y0 - coef_a * mul
        bx_1, by_1 = x0 - coef_b * mul, y0 + coef_a * mul
        tm1 = R2Point(ax_1, ay_1)
        list_of_pret.append(tm1)
        tm2 = R2Point(bx_1, by_1)
        list_of_pret.append(tm2)

    # 2nd circle
    d2 = (R2 ** 2 - coef_c ** 2 / (coef_a ** 2 + coef_b ** 2)) ** 0.5
    if check > R2:
        no_2nd_intersections = True
        # print('no points of intersection')
    elif check == R2:
        mul = (d2 ** 2 / (coef_a ** 2 + coef_b ** 2)) ** 0.5
        ax_2, ay_2 = x0 + coef_b * mul, y0 - coef_a * mul
        tb1 = R2Point(ax_2, ay_2)
        list_of_pret.append(tb1)
    else:
        mul = (d2 ** 2 / (coef_a ** 2 + coef_b ** 2)) ** 0.5
        ax_2, ay_2 = x0 + coef_b * mul, y0 - coef_a * mul
        bx_2, by_2 = x0 - coef_b * mul, y0 + coef_a * mul
        tb1 = R2Point(ax_2, ay_2)
        list_of_pret.append(tb1)
        tb2 = R2Point(bx_2, by_2)
        list_of_pret.append(tb2)

    # в случае отсутствия пересечений и точки не
    # лежат снаружи кольца/лежат внутри кольца
    if no_1st_intersections and no_2nd_intersections:
        return 0

    # составить список точек попавших в отрезок
    list_of_points = [p, q]
    for point in list_of_pret:
        if point.is_inside(p, q):
            list_of_points.insert(1, point)

    # свапами отсортировать список по размерному порядку от точки p До q
    for i in range(1, len(list_of_points) - 1):
        for j in range(i, len(list_of_points) - 1):
            a = list_of_points[0].dist(list_of_points[i])
            b = list_of_points[0].dist(list_of_points[j])
            if a > b:
                list_of_points[j], list_of_points[i] \
                    = list_of_points[i], list_of_points[j]

    # list_of_points выглядит примерно так [p, 1, 2, 3, 4 q]
    # если средняя точка отезка попадает в область кольца,
    # то длина данного отрезка прибавляется
    _g = 0
    for i in range(len(list_of_points) - 1):
        avg_x = (list_of_points[i].x + list_of_points[i + 1].x) / 2
        avg_y = (list_of_points[i].y + list_of_points[i + 1].y) / 2
        if 1 <= (avg_x ** 2 + avg_y ** 2) ** 0.5 < 2:
            _g += list_of_points[i].dist(list_of_points[i + 1])

    return _g
