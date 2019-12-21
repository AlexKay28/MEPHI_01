#!/usr/bin/env -S python3 -B

from time import time
from common.tk_drawer import TkDrawer
from shadow.polyedr import Polyedr


tk = TkDrawer()
try:
    list_of_cases = ["test_case1", "test_case2", "test_case3", "test_case4",
                    "test_case5", "test_case6", "test_case7", "test_case8"]
    # for name in ["ccc", "cube", "box", "king", "cow"]:
    for name in list_of_cases:
        print("=============================================================")
        print(f"Начало работы с полиэдром '{name}'")
        start_time = time()
        # Polyedr(f"data/{name}.geom").draw(tk)
        Polyedr(f"data/{name}.geom")
        delta_time = time() - start_time
        print(f"Изображение полиэдра '{name}' заняло {delta_time} сек.")
        input("Hit 'Return' to continue -> ")
except(EOFError, KeyboardInterrupt):
    print("\nStop")
    tk.close()
