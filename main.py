import math
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import os
from struct import unpack


def byte_to_int(str1):
    # От байта типа STR до int
    result = 0
    for i in range(len(str1)):
        y = int(str1[len(str1) - 1 - i])
        result += y * 2 ** i
    return result


def breakup_byte(num1, n):
    # byte - это параметр байта типа ввода, n - это количество точек для каждого числа,
    result = []  # Возвращенные числа
    num = num1[2:]
    num_len = len(num)
    str1 = ""
    for i in range(8 - num_len):
        str1 += str(0)
    num = str1 + num
    for i in range(int(8 / n)):
        temp = num[8 - n * (i + 1):8 - n * i]
        result.append(byte_to_int(temp))
    result.reverse()
    return result


def breakup_16byte(str1, str2):
    # 16 бит хранятся в небольшом конце
    num1 = str1[2:]
    num2 = str2[2:]
    str1_ = ""
    str2_ = ""
    num_len1 = len(num1)
    num_len2 = len(num2)
    for i in range(8 - num_len1):
        str1_ += str(0)
    num1 = str1_ + num1
    for i in range(8 - num_len2):
        str2_ += str(0)
    num2 = str2_ + num2
    num = num2 + num1
    # 16 -bit с двумя байтами, чтобы указать, что RGB установлен на 555 Последнее дополнение 0 0
    result = []
    r = byte_to_int(num[1:6])
    g = byte_to_int(num[6:11])
    b = byte_to_int(num[11:16])
    result.append(r * 8)
    result.append(g * 8)
    result.append(b * 8)
    return result


def corr_coef(a, b, m_a, m_b, dev_a, dev_b):
    return (a - m_a) * (b - m_b) / (dev_a * dev_b)


def math_corr(i_a: list, w, h):
    res = 1 / (w * h) * sum(sum(i_a, []))
    return res


def sqrt_dev(i_a: list, w, h):
    res = 0
    for i in range(w):
        for j in range(h):
            res += (i_a[h][w] - math_corr(i_a, w, h)) ** 2
    res = res / (w * h - 1)
    return math.sqrt(res)


def get_r(i_a: list, b_a: list, w, h):
    sqrt_a = sqrt_dev(i_a, w, h)
    sqrt_b = sqrt_dev(b_a, w, h)
    m_a = math_corr(i_a, w, h)
    m_b = math_corr(b_a, w, h)


def bmp_img_read_save_hist(dir):
    # List 1,4,8,16,24 позиции
    imgs = os.listdir(dir)
    # Путь генерации изображений хранится в IMGS_PATH
    imgs_path = []
    print(imgs)
    for img_name in imgs:
        img_path = dir + os.sep + img_name
        imgs_path.append(img_path)
    # осуществлять
    for img_path in imgs_path:
        with open(img_path, "rb") as img:
            # Начало информации о файле BMP, прямо прочитайте информацию о размере изображения напрямую
            img.seek(28)
            bit_num = unpack("<i", img.read(4))[0]  # Byte номер
            img.seek(10)
            # От начала до количества байтов, требуемых данными изображения
            to_img_data = unpack("<i", img.read(4))[0]
            img.seek(img.tell() + 4)
            # Unpack to Decimal
            img_width = unpack("<i", img.read(4))[0]
            img_height = unpack("<i", img.read(4))[0]
            img.seek(50)
            color_num = unpack("<i", img.read(4))[0]
            print(f'{bit_num=}')
            # 1 цифра каждый пиксель, 4 -бит на пиксель 0,5 байта, 8 -бит пикселя 1 байт, 16 -бит и один пиксель 2 байта
            # (555+0), 24 -бит пиксель 3 байта (Bgr+Alpha) Читать указатель в общей сложности 54 -бит на цветные диски,
            # из которых 16,24 изображения не нужно исправлять на цветовом диске
            if bit_num <= 8:
                # 2^n
                color_table_num = 2 ** int(bit_num)
                color_table = np.zeros((color_table_num, 3), dtype=np.int32)
                for i in range(color_table_num):
                    b = unpack("B", img.read(1))[0]
                    g = unpack("B", img.read(1))[0]
                    r = unpack("B", img.read(1))[0]
                    alpha = unpack("B", img.read(1))[0]
                    color_table[i][0] = b
                    color_table[i][1] = g
                    color_table[i][2] = r
            # Поставить данные в Numpy
            img.seek(to_img_data)
            img_np = np.zeros((img_height, img_width, 3), dtype=np.int32)
            reds = np.zeros((img_height, img_width, 3), dtype=np.int32)
            greens = np.zeros((img_height, img_width, 3), dtype=np.int32)
            blues = np.zeros((img_height, img_width, 3), dtype=np.int32)
            num = 0  # Catter Общее количество байтов читается в
            # Data -расположение слева направо, снизу наверху
            for y in range(img_height):
                for x in range(img_width):
                    if bit_num <= 8:  # 8 -bit изображение чтение
                        img_byte = unpack("B", img.read(1))[0]
                        img_byte = bin(img_byte)
                        color_index = breakup_byte(img_byte, bit_num)
                        num += 1
                        for index in color_index:
                            if x < img_width:
                                img_np[img_height - y - 1][x] = color_table[index]
                                reds[img_height - y - 1][x] = color_table[index]
                                greens[img_height - y - 1][x] = color_table[index]
                                blues[img_height - y - 1][x] = color_table[index]
                    elif bit_num == 24:  # 24 -bit изображение чтения
                        num += 3
                        g = unpack("B", img.read(1))[0]
                        b = unpack("B", img.read(1))[0]
                        r = unpack("B", img.read(1))[0]
                        img_np[img_height - y - 1][x] = [r, b, g]
                        reds[img_height - y - 1][x] = [r, 0, 0]
                        greens[img_height - y - 1][x] = [0, g, 0]
                        blues[img_height - y - 1][x] = [0, 0, b]
                    elif bit_num == 16:  # 16 -bit изображение чтение
                        str1 = bin(unpack("B", img.read(1))[0])
                        str2 = bin(unpack("B", img.read(1))[0])
                        bgr_color = breakup_16byte(str1, str2)
                        img_np[img_height - y - 1][x] = [bgr_color[0], bgr_color[1], bgr_color[2]]
                        num += 2
                while num % 4 != 0:  # Количество каждой строки должно быть множеством 4
                    num += 1
                    img.read(1)

        plt.figure("img")
        plt.imshow(img_np)
        plt.show()

        f, (ax1, ax2, ax3) = plt.subplots(3, 1)
        f.set_label("RGB")

        ax1.imshow(reds)
        ax2.imshow(greens)
        ax3.imshow(blues)
        plt.show()

        save_color(img_path, 'red')
        save_color(img_path, 'green')
        save_color(img_path, 'blue')

        red_colors = [[x[0] for x in reds[y]] for y in range(img_height)]
        red_colors = np.reshape(red_colors, (img_height, img_width)).flatten()
        green_colors = [[x[1] for x in greens[y]] for y in range(img_height)]
        green_colors = np.reshape(green_colors, (img_height, img_width)).flatten()
        blue_colors = [[x[2] for x in blues[y]] for y in range(img_height)]
        blue_colors = np.reshape(blue_colors, (img_height, img_width)).flatten()

        data = {'red': red_colors, 'green': green_colors, 'blue': blue_colors}
        data = pd.DataFrame(data)
        corr = data.corr()
        print(f'Корреляция между цветами в файле {img_path} RGB равна\n {corr}')

        list_y = [-10, -5, 0, 5, 10]
        list_x = [x for x in range(-130, 131, 10)]
        # TODO: autocorrelation func for red, green, blue channels and plot with 3 subplots (x, y)

        Y_components = np.zeros((img_height, img_width, 1), dtype=np.int32)
        Cb_components = np.zeros((img_height, img_width, 1), dtype=np.int32)
        Cr_componets = np.zeros((img_height, img_width, 1), dtype=np.int32)

        for h in range(img_height):
            for w in range(img_width):
                Y_components[h][w] = 0.299 * reds[h][w][0] + 0.587 * greens[h][w][1] + 0.114 * blues[h][w][2]
                Cb_components[h][w] = 0.5643 * (blues[h][w][2] - Y_components[h][w]) + 128
                Cr_componets[h][w] = 0.7152 * (reds[h][w][0] - Y_components[h][w]) + 128
        data = {'Y': Y_components.ravel(), 'Cb': Cb_components.ravel(), 'Cr': Cr_componets.ravel()}
        data = pd.DataFrame(data)
        corr = data.corr()
        print(f'Корреляция между цветами в файле {img_path} YCbCr равна\n {corr}')
        # TODO: autocorrelation func for Y, Cb, Cr channels and plot with 3 subplots (x, y)

        save_component(img_path, 'Y', Y_components.ravel())
        save_component(img_path, 'Cb', Cb_components.ravel())
        save_component(img_path, 'Cr', Cr_componets.ravel())

        def save_img():
            # Сделайт изображение в формате JPG в папке saved_img
            img_name_save = "saved_img" + os.sep + "saved_" + str(img_path.split(os.sep)[1]).split('.')[0] + ".jpg"
            matplotlib.image.imsave(img_name_save, img_np.astype(np.uint8))
            img_name_save = "saved_img" + os.sep + "saved_red_" + str(img_path.split(os.sep)[1]).split('.')[0] + ".jpg"
            matplotlib.image.imsave(img_name_save, reds.astype(np.uint8))
            img_name_save = "saved_img" + os.sep + "saved_green_" + str(img_path.split(os.sep)[1]).split('.')[
                0] + ".jpg"
            matplotlib.image.imsave(img_name_save, greens.astype(np.uint8))
            img_name_save = "saved_img" + os.sep + "saved_blue_" + str(img_path.split(os.sep)[1]).split('.')[0] + ".jpg"
            matplotlib.image.imsave(img_name_save, blues.astype(np.uint8))

        def draw_hist():
            if bit_num <= 8:
                plt.figure("hist")
                arr = img_np.flatten()
                plt.hist(arr, bins=2 ** bit_num, facecolor='green', alpha=0.75)
                plt.show()
            else:
                plt.figure("hist")
                ar = np.array(img_np[:, :, 0]).flatten()
                plt.hist(ar, bins=256, facecolor='r', edgecolor='r', alpha=0.5)
                ag = np.array(img_np[:, :, 1]).flatten()
                plt.hist(ag, bins=256, facecolor='g', edgecolor='g', alpha=0.5)
                ab = np.array(img_np[:, :, 2]).flatten()
                plt.hist(ab, bins=256, facecolor='b', edgecolor='b', alpha=0.5)
                plt.savefig("hist.png")
                plt.show()

        txt_name = "img_txt" + os.sep + "txt_" + (img_path.split(os.sep)[1]).split('.')[0] + '.txt'
        img_np = np.reshape(img_np, (img_height * 3, img_width))
        np.savetxt(txt_name, img_np, fmt='%d')


def negative(input_file: str):
    with open(input_file, mode='rb') as i_file:
        header = i_file.read(138)
        res = [255 - val for val in i_file.read()]
        with open(input_file.split('.')[0] + "_neg.bmp", mode="wb") as o_file:
            o_file.write(header)
            o_file.write(bytes(res))


def save_color(input_file: str, color_name: str):
    with open(input_file, mode='rb') as i_file:
        header = i_file.read(138)
        res = [val for val in i_file.read()]
        with open("saved_img" + os.sep + str(input_file.split(os.sep)[1]).split('.')[0] + "_" + color_name + ".bmp",
                  mode="wb") as o_file:
            o_file.write(header)
            for i, val in enumerate(res):
                if i % 3 == 0 and color_name == 'blue':
                    o_file.write(bytes([val]))
                    o_file.write(bytes([0]))
                    o_file.write(bytes([0]))
                elif i % 3 == 1 and color_name == 'green':
                    o_file.write(bytes([0]))
                    o_file.write(bytes([val]))
                    o_file.write(bytes([0]))
                elif i % 3 == 2 and color_name == 'red':
                    o_file.write(bytes([0]))
                    o_file.write(bytes([0]))
                    o_file.write(bytes([val]))


def save_component(inputfile: str, component_name: str, component: np.array):
    with open(inputfile, mode='rb') as i_file:
        header = i_file.read(138)
        with open("saved_img" + os.sep + str(inputfile.split(os.sep)[1]).split('.')[0] + "_" + component_name + ".bmp",
                  mode="wb") as o_file:
            o_file.write(header)
            for i, val in enumerate(component):
                val = bytes([val])
                o_file.write(val)
                o_file.write(val)
                o_file.write(val)


if __name__ == "__main__":
    bmp_img_read_save_hist('imgs')
