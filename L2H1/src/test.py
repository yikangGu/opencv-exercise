# def funcA(A):
#     print("function A")
#     print(A)


def funcB(B):
    print(B(3))
    print("function B")


# @funcA
@funcB
def func(c):
    print("function C")
    return c**2
