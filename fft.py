import scipy as sp
import numpy as np
from random import randint
from time import process_time_ns

x = []
for i in range(65536):
    x.append(randint(0, 10000))

start_time = process_time_ns()
for i in range(1000):
    y = np.fft.fft(x)
end_time = process_time_ns()
print("Numpy fft on standart array:", (end_time - start_time) / 1000)


start_time = process_time_ns()
for i in range(1000):
    y = sp.fft.fft(x)
end_time = process_time_ns()
print("Scipy fft on standart array:", (end_time - start_time) / 1000)


x = np.array(x)

start_time = process_time_ns()
for i in range(1000):
    y = np.fft.fft(x)
end_time = process_time_ns()
print("Numpy fft on numpy array:   ", (end_time - start_time) / 1000)


start_time = process_time_ns()
for i in range(1000):
    y = sp.fft.fft(x)
end_time = process_time_ns()
print("Scipy fft on numpy array:   ", (end_time - start_time) / 1000)

# print(sp.fft.fft([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]))