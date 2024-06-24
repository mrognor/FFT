import matplotlib.pyplot as plt
import numpy as np


def plot(x, y):
    # Создаем новое окно
    plt.figure()

    # Рисуем график
    plt.plot(x, y)

    # Рисуем оси
    ax = plt.gca()
    ax.axhline(y=0, color='k')
    ax.axvline(x=0, color='k')

    # Рисуем сетку
    plt.grid(True)


# Convert sin to polar coordinates
# td = np.arange(0, 10 * np.pi, 0.01)
# yd = np.sin(0.5 * td)

# tp = np.array([])
# yp = np.array([])

# for i in range(0, len(td)):
#     tp = np.append(tp, yd[i] * np.sin(td[i]))
#     yp = np.append(yp, yd[i] * np.cos(td[i]))

# plot(td, yd)
# plot(tp, yp)

# plt.show()


td = np.arange(0, 2 * np.pi, 0.01)
yd = np.sin(2 * td)

plot(td, yd)

for k in range(1, 5):
    tp = np.array([])
    yp = np.array([])
    
    for i in range(0, len(td)):
        tp = np.append(tp, yd[i] * np.sin(k * td[i]))
        yp = np.append(yp, yd[i] * np.cos(k * td[i]))

    plot(tp, yp)



plt.show()