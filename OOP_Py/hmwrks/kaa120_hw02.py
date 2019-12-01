def h1(lst):
    counter = 0
    elements_exist = False
    for element in lst:
        if element % 2 == 0:
            min = abs(element)
            elements_exist = True
            break
    if elements_exist:
        for element in lst:
            if element % 2 == 0:
                if abs(element) < min and element % 2 == 0:
                    min = abs(element)
                    counter = 1
                elif abs(element) == min and element % 2 == 0:
                    counter += 1
    return counter


def h2(lst):
    summ, counter = 0, 0
    for element in lst:
        if element % 2 == 0:
            summ += element
            counter += 1
    if counter == 0:
        return None
    return summ // counter
