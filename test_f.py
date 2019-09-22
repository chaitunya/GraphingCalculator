import math

def multiple_of(x):
    l = [
            [1, math.log10(x)],
            [2, math.log10(x/2)],
            [5, math.log10(x/5)]
    ]

    # print(l)
    e = min(l, key=lambda x: abs(x[1] - round(x[1])))
    return e[0], round(e[1])

print(multiple_of(0.01))
print(multiple_of(0.02))
print(multiple_of(0.05))
