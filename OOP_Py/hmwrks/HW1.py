#First task
a, b, c = int(input()), int(input()), int(input())
array = [a, b, c]
Nmax = array.count(max(set(array)))
print(Nmax)

#Second task
a, b, c = int(input()), int(input()), int(input())
array = [a, b, c]
seq = set(array)
seq.remove(max(set(array)))
if len(seq) == 0:
    print(0)
else:
    Nmax = array.count(max(seq))
    print(Nmax)

#Third task
Number = int(input())
for i in range(1, Number + 1):
    if Number % i == 0:
        print(i)
