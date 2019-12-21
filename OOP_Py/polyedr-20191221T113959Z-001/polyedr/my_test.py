#!/usr/bin/env -S python3 -B

import unittest
from shadow.polyedr import Polyedr


class Tests(unittest.TestCase):

    def test_case1(self):
        # куб полностью вне сферы со сторонами 1 1 1
        test1 = Polyedr(f"data/test_case1.geom")
        assert test1.sum_of_edges == 7110.716

    def test_case2(self):
        # тетраедр полностью вне фигуры
        test2 = Polyedr(f"data/test_case2.geom")
        print(5)
        assert test2.sum_of_edges == 10183.055

    def test_case3(self):
        # куб полностью внутри сферы, хороших точек нет
        test3 = Polyedr(f"data/test_case3.geom")
        assert test3.sum_of_edges == 0

    def test_case4(self):
        # тетраедр полностью внутри сферы, хороших точек нет
        test4 = Polyedr(f"data/test_case4.geom")
        assert test4.sum_of_edges == 0

    def test_case5(self):
        # тетраедр частично в сфере, только одна хорошая точка
        test5 = Polyedr(f"data/test_case5.geom")
        assert test5.sum_of_edges == 618.089

    def test_case6(self):
        # тетраедр частично в сфере, только одна "нехорошая" точка
        test6 = Polyedr(f"data/test_case6.geom")
        assert test6.sum_of_edges == 7372.072

    def test_case7(self):
        # параллелепипед частично в сфере, 4 точки хороших, выступают по оси Z
        test7 = Polyedr(f"data/test_case7.geom")
        assert test7.sum_of_edges == 2867.525

    def test_case8(self):
        # параллелепипед частично в сфере, 4 точки хороших, выступают по отрицательному направлению оси Х
        test8 = Polyedr(f"data/test_case8.geom")
        assert test8.sum_of_edges == 5140.199
