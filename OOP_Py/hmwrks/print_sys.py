import sys

console = sys.stdout
file = open('qq.out', 'w', encoding='utf-8')
sys.stdout = file
print("Здравствуй, мир!")
sys.stdout = console
file.close()

# with open('qq2.out', 'w', encoding='utf-8') as file2:
#     sys.stdout = file2
#     print("Здравствуй, мир!")
#     sys.stdout = console



