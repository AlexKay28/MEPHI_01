#!/usr/bin/env -S python3 -B
from tk_drawer import TkDrawer
from r2point import R2Point
from convex import Void, Point, Segment, Polygon

tk = TkDrawer()
f = Void()
tk.clean()

try:
    while True:
        f = f.add(R2Point())
        tk.clean()
        if isinstance(f, Point):
            tk.draw_point(f.p)
        elif isinstance(f, Segment):
            tk.draw_line(f.p, f.q)
        elif isinstance(f, Polygon):
            for n in range(f.points.size()):
                tk.draw_line(f.points.last(), f.points.first())
                f.points.push_last(f.points.pop_first())
        print(f"S = {f.area()}, P = {f.perimeter()}")
        print()
except(EOFError, KeyboardInterrupt):
    print("\nStop")
    tk.close()
