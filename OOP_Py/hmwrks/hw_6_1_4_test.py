import hw_6_1_4 as hw
import unittest


# тесты на пересечение отрезков
class Intersec_test(unittest.TestCase):
    def test1(self):
        # отезки [0, 5] и [3, 10]
        a = hw.Segment(0, 5)
        b = hw.Segment(3, 10)
        self.assertEqual(hw.Segment.intersection(a, b), hw.Segment(3, 5))

    def test2(self):
        a = hw.Segment(-5, 5)
        b = hw.Segment(-1, 10)
        self.assertEqual(hw.Segment.intersection(a, b), hw.Segment(-1, 5))

    def test3(self):
        a = hw.Segment(0, 0)
        b = hw.Segment(0, 10)
        self.assertEqual(hw.Segment.intersection(a, b), [])

    def test4(self):
        a = hw.Segment(6, 2)
        b = hw.Segment(0, 10)
        self.assertEqual(hw.Segment.intersection(a, b), hw.Segment(2, 6))

    def test5(self):
        a = hw.Segment(10, 0)
        b = hw.Segment(5, -3)
        self.assertEqual(hw.Segment.intersection(a, b), hw.Segment(0, 5))

    def test6(self):
        a = hw.Segment(0, 0)
        b = hw.Segment(0, -0)
        self.assertEqual(hw.Segment.intersection(a, b), [])


    def test7(self):
        a = hw.Segment(1, 0)
        b = hw.Segment(5, 3)
        self.assertEqual(hw.Segment.intersection(a, b), [])

    # тесты на разность
    def test8(self):
        a = hw.Segment(0, 8)
        b = hw.Segment(5, 2)
        assert (a - b) == [hw.Segment(0, 2), hw.Segment(5, 8)]

    def test9(self):
        a = hw.Segment(0, 0)
        b = hw.Segment(-0, -0)
        assert (a - b) == []

    def test10(self):
        a = hw.Segment(-10, 10)
        b = hw.Segment(20, -20)
        assert (a - b) == []

    def test11(self):
        a = hw.Segment(6, 145)
        b = hw.Segment(0, 100)
        assert (a - b) == hw.Segment(100, 145)
