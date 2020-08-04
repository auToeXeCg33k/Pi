from mpmath import *
import time

mp.dps = 10000


def abel(k):
    return mpf(4) / (mpf(8) * mpf(k) + mpf(1))


def bela(k):
    return mpf(2) / (mpf(8) * mpf(k) + mpf(4))


def cecilia(k):
    return mpf(1) / (mpf(8) * mpf(k) + mpf(5))


def domotor(k):
    return mpf(1) / (mpf(8) * mpf(k) + mpf(6))


def iteration(k):
    return mpf((mpf(abel(k)) - mpf(bela(k)) - mpf(cecilia(k)) - mpf(domotor(k))) / mpf(16 ** k))


pi = mpf(0)

print("Starting computation...")

begin = int(round(time.time() * 1000))

for k in range(0, 10000):
    pi += iteration(k)

end = int(round(time.time() * 1000))

print("Completion time:", end - begin, "ms.")

file = open("result.txt", "w")

file.write(str(pi))

file.close()

print("Result in \"result.txt\".")
