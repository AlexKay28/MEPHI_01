class Segment:
    def __init__(self, a, b):
        self.a = min(a, b)
        self.b = max(a, b)

    # разность двух отрезков *разность длин
    def __sub__(self, other):
        a1, b1 = self.a, self.b
        a2, b2 = other.a, other.b
        if a1 == 0 and b1 == 0:
            return []
        elif a2 == b2:
            return [a1, b1]
        else:
            if a1 < a2 and b2 < b1:
                return [Segment(a1, a2), Segment(b2, b1)]
            if a1 < a2 and b2 >= b1:
                return Segment(a1, a2)
            if a1 >= a2 and b2 < b1:
                return Segment(b2, b1)
            if a1 >= a2 and b2 >= b1:
                return []

    def intersection(self, other):
        int_a = max(self.a, other.a)
        int_b = min(self.b, other.b)
        intersec = int_b - int_a
        if intersec <= 0:
            return []
        return Segment(int_a, int_b)

    def __eq__(self, other):
        a1, b1 = self.a, self.b
        a2, b2 = other.a, other.b
        if (a1 == a2 and b1 == b2):
            return True
        else:
            return False

    def __repr__(self):
        return f"[{self.a}, {self.b}]"