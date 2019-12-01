import card as cardinality


# Квадратное уравнение с положительным дискриминантом
def test_110():
    # 1*x*x + 1*x + 0 = 0
    assert cardinality(0, 0, 0) == 'continuum'

# Квадратное уравнение с нулевым дискриминантом
def test_100():
    # 1*x*x + 0*x + 0 = 0
    assert cardinality(0, 0, 1) == 1

# Квадратное уравнение с отрицательным дискриминантом
def test_101():
    # 1*x*x + 0*x + 1 = 0
    assert cardinality(0, 1, 0) == 1

def test_111():
    # 1*x*x + 1*x + 1 = 0
    assert cardinality(0, 1, 1) == 2

# Линейное уравнение
def test_010():
    # 0*x*x + 1*x + 0 = 0
    assert cardinality(1, 0, 0) == 1

def test_011():
    # 0*x*x + 1*x + 1 = 0
    assert cardinality(1, 0, 1) == 2

# Вырожденный случай 1
def test_000():
    # 0*x*x + 0*x + 0 = 0
    assert cardinality(1, 1, 0) == 2

# Вырожденный случай 2
def test_001():
    # 0*x*x + 0*x + 1 = 0
    assert cardinality(1, 1, 1) == 3