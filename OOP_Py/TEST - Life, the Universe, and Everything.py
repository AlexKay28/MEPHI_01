num = int(input())
while num != 42 and len(str(abs(num))) < 3:
    print(num)
    num = int(input())
print('the code are stopped')