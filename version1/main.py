from PIL import Image
import numpy as np
from math import *
import struct
import cv2 as cv
from matplotlib import pyplot as plt
import seaborn as sns

# 2, 3

kodim_ = 'kodim02.bmp'
img = np.array(Image.open(kodim_))

with open('%s' % kodim_, "rb") as f:
    file_data = f.read()

header_data = struct.unpack('<2sIHHIIIIHHIIIIII', file_data[:54])
width = struct.unpack_from('<i', file_data, 18)[0]
height = struct.unpack_from('<i', file_data, 22)[0]


# print(header_data)


def get_col_array(color):
    color_array = np.copy(img)
    for i in range(0, height):
        for j in range(0, width):
            for k in range(0, 3):
                if k == color:
                    continue
                color_array[i][j][k] = 0
    return color_array


# f = open(kodim_, "rb")
# data = bytearray(f.read())
# f.close()
# print(data[:20])

# r = get_col_array(0)
# g = get_col_array(1)
# b = get_col_array(2)
#
# Image.fromarray(r).save('r.bmp')
# Image.fromarray(g).save('g.bmp')
# Image.fromarray(b).save('b.bmp')

# 4a

def mat_waiting(image, comp):
    H = image.shape[0]
    W = image.shape[1]
    result = 0
    for i in range(0, H):
        for j in range(0, W):
            result += image[i][j][comp]
    return result / (width * height)


def dispersion(image, comp):
    H = image.shape[0]
    W = image.shape[1]
    result = 0
    a = mat_waiting(image, comp)
    for i in range(0, H):
        for j in range(0, W):
            result += (image[i][j][comp] - a) ** 2
    result /= (width * height - 1)
    return result ** 0.5


def correlation(image, compA, compB):
    a = mat_waiting(image, compA)
    b = mat_waiting(image, compB)
    res = 0
    for i in range(0, height):
        for j in range(0, width):
            res += (image[i][j][compA] - a) * (image[i][j][compB] - b)
    res /= (width * height - 1)
    return res / (dispersion(image, compA) * dispersion(image, compB))


def correlation_print():
    print("r_RG = ", round(correlation(img, 0, 1), 5))
    print("r_RB = ", round(correlation(img, 0, 2), 5))
    print("r_BG = ", round(correlation(img, 1, 2), 5))


# 4b


def autocorrelation(image, component, y, x):
    H = image.shape[0]
    W = image.shape[1]
    if y >= 0 and x >= 0:
        img1 = image[:H - y, :W - x]
        img2 = image[y:, x:]
    elif y <= 0 and x <= 0:
        y = abs(y)
        x = abs(x)
        img1 = image[y:, x:]
        img2 = image[:H - y, :W - x]
    elif y <= 0 and x >= 0:
        y = abs(y)
        img1 = image[y:, :W - x]
        img2 = image[:H - y, x:]
    elif y >= 0 and x <= 0:
        x = abs(x)
        img1 = image[:H - y, x:]
        img2 = image[y:, :W - x]

    H -= y
    W -= x
    var1 = mat_waiting(img1, component)
    var2 = mat_waiting(img2, component)
    tempArray = np.ndarray((H, W, 1))
    for i in range(0, H):
        for j in range(0, W):
            tempArray[i][j] = (img1[i][j][component] - var1) * (img2[i][j][component] - var2)
    var = mat_waiting(tempArray, 0)
    return var / (dispersion(img1, component) * dispersion(img2, component))


def task4b(im):
    auto_red = []
    auto_green = []
    auto_blue = []
    for y in range(-10, 11, 5):
        auto_red.append([autocorrelation(im, 0, x, y) for x in range(-10, 11, 5)])
        auto_green.append([autocorrelation(im, 1, x, y) for x in range(-10, 11, 5)])
        auto_blue.append([autocorrelation(im, 2, x, y) for x in range(-10, 11, 5)])

    plt.plot([x for x in range(-10, 11, 5)], auto_red)
    plt.legend(['-10', '-5', '0', '5', '10'])
    plt.savefig('task4b_1.png')
    plt.show()
    plt.plot([x for x in range(-10, 11, 5)], auto_green)
    plt.legend(['-10', '-5', '0', '5', '10'])
    plt.savefig('task4b_2.png')
    plt.show()
    plt.plot([x for x in range(-10, 11, 5)], auto_blue)
    plt.legend(['-10', '-5', '0', '5', '10'])
    plt.savefig('task4b_3.png')
    plt.show()


# task4b(im)


# 5
def YCC():
    color_array_RGB = np.copy(img)
    color_array_YCC = np.copy(img)
    for i in range(0, height):
        for j in range(0, width):
            color_array_YCC[i][j][0] = 0.299 * color_array_RGB[i][j][0] + 0.587 * color_array_RGB[i][j][1] + 0.114 * \
                                       color_array_RGB[i][j][2]
            color_array_YCC[i][j][1] = 0.5643 * (int(color_array_RGB[i][j][2]) - int(color_array_YCC[i][j][0])) + 128
            color_array_YCC[i][j][2] = 0.7132 * (int(color_array_RGB[i][j][0]) - int(color_array_YCC[i][j][0])) + 128

    return color_array_YCC


img_YCC = Image.fromarray(YCC())
img_YCC.save("YCC.bmp")
img_YCC = np.array(Image.open('YCC.bmp'))

print("r_YCb = ", round(correlation(img_YCC, 0, 1), 5))
print("r_YCr = ", round(correlation(img_YCC, 0, 2), 5))
print("r_CbCr = ", round(correlation(img_YCC, 1, 2), 5))


# 6

def set_col_YCC(color):
    color_array = np.copy(img_YCC)
    for i in range(0, height):
        for j in range(0, width):
            for k in range(0, 3):
                color_array[i][j][k] = color_array[i][j][color]
    return color_array


y = set_col_YCC(0)
cb = set_col_YCC(1)
cr = set_col_YCC(2)

Image.fromarray(y).save('y.bmp')
Image.fromarray(cb).save('cb.bmp')
Image.fromarray(cr).save('cr.bmp')


# 7

def RGB():
    color_array_YCC = np.copy(img_YCC)
    color_array_RGB = np.zeros((height, width, 3), dtype=np.uint8)
    for i in range(0, height):
        for j in range(0, width):
            c0 = color_array_YCC[i][j][0] + 1.402 * (int(color_array_YCC[i][j][2]) - 128)
            c1 = color_array_YCC[i][j][0] - 0.714 * (int(color_array_YCC[i][j][2]) - 128) - 0.334 * (
                    int(color_array_YCC[i][j][1]) - 128)
            c2 = color_array_YCC[i][j][0] + 1.772 * (int(color_array_YCC[i][j][1]) - 128)
            if c0 < 0:
                c0 = 0
            elif c0 > 255:
                c0 = 255
            elif c1 < 0:
                c1 = 0
            elif c1 > 255:
                c1 = 255
            elif c2 < 0:
                c2 = 0
            elif c2 > 255:
                c2 = 255
            color_array_RGB[i][j][0] = c0
            color_array_RGB[i][j][1] = c1
            color_array_RGB[i][j][2] = c2

    return color_array_RGB


img_RGB = Image.fromarray(RGB())
img_RGB.save("RGB.bmp")
img_RGB = np.array(Image.open('RGB.bmp'))


def PSNR(comp, image1, image2):
    color_array = np.copy(image2)
    color_array_RGB = np.copy(image1)
    tmp = 0
    for i in range(0, height):
        for j in range(0, width):
            tmp += (int(color_array[i][j][comp]) - int(color_array_RGB[i][j][comp])) ** 2
    res = 10 * log10((width * height * (2 ** 8 - 1) ** 2) / tmp)
    return res


print("PSNR R = ", PSNR(0, img_RGB, img))
print("PSNR G = ", PSNR(1, img_RGB, img))
print("PSNR B = ", PSNR(2, img_RGB, img))


# 8a

def decimation(color):
    color_array = np.zeros((height // 2, width // 2, 3), dtype=np.uint8)
    color_array_YCC = np.copy(img_YCC)
    for i in range(0, height // 2, 1):
        for j in range(0, width // 2, 1):
            for k in range(0, 3):
                color_array[i][j][k] = color_array_YCC[i * 2 + 1][j * 2 + 1][color]
    return color_array


def recovery(color):
    color_array = decimation_2(color)
    color_array_recovery = np.zeros((height, width, 3), dtype=np.uint8)
    for i in range(0, height):
        for j in range(0, width):
            for k in range(0, 3):
                color_array_recovery[i][j][k] = color_array[i // 2][j // 2][color]
    return color_array_recovery


def union():
    color_array = np.zeros((height, width, 3), dtype=np.uint8)
    for i in range(0, height):
        for j in range(0, width):
            color_array[i][j][0] = y[i][j][0]
            color_array[i][j][1] = cb_rec[i][j][1]
            color_array[i][j][2] = cr_rec[i][j][2]
    return color_array


dec_cb = decimation(1)
dec_cr = decimation(2)
Image.fromarray(dec_cb).save('dec_cb1.bmp')
Image.fromarray(dec_cr).save('dec_cr1.bmp')

img_YCC_recovery = Image.fromarray(union())
img_YCC_recovery.save("YCC_recovery1.bmp")
img_YCC_recovery = np.array(Image.open('YCC_recovery1.bmp'))


# 8б

def decimation_2(color):
    color_array = np.zeros((height // 2, width // 2, 3), dtype=np.uint8)
    color_array_YCC = np.copy(img_YCC)
    a = 0
    b = 0
    for i in range(0, height, 2):
        for j in range(0, width, 2):
            c = (int(color_array_YCC[i][j][color]) + int(color_array_YCC[i + 1][j][color]) +
                 int(color_array_YCC[i][j + 1][color]) + int(color_array_YCC[i + 1][j + 1][color])) // 4
            for k in range(0, 3):
                color_array[a][b][k] = c
            b += 1
        b = 0
        a += 1
    return color_array


dec_cb_2 = decimation_2(1)
dec_cr_2 = decimation_2(2)
Image.fromarray(dec_cb_2).save('dec_cb_2.bmp')
Image.fromarray(dec_cr_2).save('dec_cr_2.bmp')

# 9


y = set_col_YCC(0)
cb_rec = recovery(1)
cr_rec = recovery(2)

img_YCC_recovery = Image.fromarray(union())
img_YCC_recovery.save("YCC_recovery.bmp")
img_YCC_recovery = np.array(Image.open('YCC_recovery.bmp'))


def RGB_rec():
    color_array_YCC = union()
    color_array_RGB_rec = np.zeros((height, width, 3), dtype=np.uint8)
    for i in range(0, height):
        for j in range(0, width):
            c0 = color_array_YCC[i][j][0] + 1.402 * (int(color_array_YCC[i][j][2]) - 128)
            c1 = color_array_YCC[i][j][0] - 0.714 * (int(color_array_YCC[i][j][2]) - 128) - 0.334 * (
                    int(color_array_YCC[i][j][1]) - 128)
            c2 = color_array_YCC[i][j][0] + 1.772 * (int(color_array_YCC[i][j][1]) - 128)
            if c0 < 0:
                c0 = 0
            elif c0 > 255:
                c0 = 255
            elif c1 < 0:
                c1 = 0
            elif c1 > 255:
                c1 = 255
            elif c2 < 0:
                c2 = 0
            elif c2 > 255:
                c2 = 255
            color_array_RGB_rec[i][j][0] = c0
            color_array_RGB_rec[i][j][1] = c1
            color_array_RGB_rec[i][j][2] = c2
    return color_array_RGB_rec


img_RGB_recovery = Image.fromarray(RGB_rec())
img_RGB_recovery.save("RGB_recovery1.bmp")
img_RGB_recovery = np.array(Image.open('RGB_recovery1.bmp'))

# 10
print('10')

print("PSNR R = ", PSNR(0, img_RGB_recovery, img))
print("PSNR G = ", PSNR(1, img_RGB_recovery, img))
print("PSNR B = ", PSNR(2, img_RGB_recovery, img))
print("PSNR Cb = ", PSNR(1, img_YCC_recovery, img_YCC))
print("PSNR Cr = ", PSNR(2, img_YCC_recovery, img_YCC))

# 11
print('11')


def decimation_4a(color):
    color_array = np.zeros((height // 4, width // 4, 3), dtype=np.uint8)
    color_array_YCC = np.copy(img_YCC)
    for i in range(0, height // 4, 1):
        for j in range(0, width // 4, 1):
            for k in range(0, 3):
                color_array[i][j][k] = color_array_YCC[i * 4 + 1][j * 4 + 1][color]
    return color_array


dec_cb_4 = decimation_4a(1)
dec_cr_4 = decimation_4a(2)
Image.fromarray(dec_cb_4).save('dec_cb_4.bmp')
Image.fromarray(dec_cr_4).save('dec_cr_4.bmp')


def decimation_4b(color):
    color_array = np.zeros((height // 4, width // 4, 3), dtype=np.uint8)
    color_array_YCC = np.copy(img_YCC)
    a = 0
    b = 0
    for i in range(0, height, 4):
        for j in range(0, width, 4):
            c = (int(color_array_YCC[i][j][color]) + int(color_array_YCC[i + 1][j][color]) +
                 int(color_array_YCC[i][j + 1][color]) + int(color_array_YCC[i + 1][j + 1][color]) +
                 int(color_array_YCC[i][j + 2][color]) + int(color_array_YCC[i + 2][j][color]) +
                 int(color_array_YCC[i + 2][j + 2][color]) + int(color_array_YCC[i][j + 3][color]) +
                 int(color_array_YCC[i + 3][j][color]) + int(color_array_YCC[i + 3][j + 3][color]) +
                 int(color_array_YCC[i + 1][j + 2][color]) + int(color_array_YCC[i + 1][j + 3][color]) +
                 int(color_array_YCC[i + 2][j + 3][color]) + int(color_array_YCC[i + 2][j + 1][color]) +
                 int(color_array_YCC[i + 3][j + 1][color]) + int(color_array_YCC[i + 3][j + 2][color])) // 16

            for k in range(0, 3):
                color_array[a][b][k] = c
            b += 1
        b = 0
        a += 1
    return color_array


dec_cb_4b = decimation_4b(1)
dec_cr_4b = decimation_4b(2)
Image.fromarray(dec_cb_4b).save('dec_cb_4b.bmp')
Image.fromarray(dec_cr_4b).save('dec_cr_4b.bmp')


def recovery(color):
    color_array = decimation_4b(color)
    color_array_recovery = np.zeros((height, width, 3), dtype=np.uint8)
    for i in range(0, height):
        for j in range(0, width):
            for k in range(0, 3):
                color_array_recovery[i][j][k] = color_array[i // 4][j // 4][color]
    return color_array_recovery


y_4 = set_col_YCC(0)
cb_rec_4 = recovery(1)
cr_rec_4 = recovery(2)


def union():
    color_array = np.zeros((height, width, 3), dtype=np.uint8)
    for i in range(0, height):
        for j in range(0, width):
            color_array[i][j][0] = y_4[i][j][0]
            color_array[i][j][1] = cb_rec_4[i][j][1]
            color_array[i][j][2] = cr_rec_4[i][j][2]
    return color_array


img_YCC_recovery_4 = Image.fromarray(union())
img_YCC_recovery_4.save("YCC_recovery_4.bmp")
img_YCC_recovery_4 = np.array(Image.open('YCC_recovery_4.bmp'))

img_RGB_recovery_4 = Image.fromarray(RGB_rec())
img_RGB_recovery_4.save("RGB_recovery_4.bmp")
img_RGB_recovery_4 = np.array(Image.open('RGB_recovery_4.bmp'))

print("PSNR R = ", PSNR(0, img_RGB_recovery_4, img))
print("PSNR G = ", PSNR(1, img_RGB_recovery_4, img))
print("PSNR B = ", PSNR(2, img_RGB_recovery_4, img))
print("PSNR Cb = ", PSNR(1, img_YCC_recovery_4, img_YCC))
print("PSNR Cr = ", PSNR(2, img_YCC_recovery_4, img_YCC))

# 12
print('12')


def saturation(color, im):
    tmp_color = [0] * 256
    color_array_RGB = np.copy(im)
    for i in range(0, height):
        for j in range(0, width):
            tmp_color[color_array_RGB[i][j][color]] += 1
    return tmp_color


with open("tmp.txt", "w") as file:
    print(saturation(2, img_YCC), file=file, sep=' ')

img_cv = cv.imread(kodim_)
img_cv_YCC = cv.imread('YCC.bmp')

plt.hist(saturation(0, img), bins=int(180 / 1))
hist = cv.calcHist([img_cv], [0], None, [256], [0, 256])
hist1 = cv.calcHist([img_cv], [1], None, [256], [0, 256])
hist2 = cv.calcHist([img_cv], [2], None, [256], [0, 256])
hist3 = cv.calcHist([img_cv_YCC], [0], None, [256], [0, 256])
hist4 = cv.calcHist([img_cv_YCC], [1], None, [256], [0, 256])
hist5 = cv.calcHist([img_cv_YCC], [2], None, [256], [0, 256])

plt.plot(hist, color='r')
plt.title('Histogram R')
plt.xlim([0, 256])
plt.savefig('hist_R.png')
plt.close()

plt.plot(hist1, color='g')
plt.title('Histogram G')
plt.xlim([0, 256])
plt.savefig('hist_G.png')
plt.close()

plt.plot(hist2, color='b')
plt.title('Histogram B')
plt.xlim([0, 256])
plt.savefig('hist_B.png')
plt.close()

plt.plot(hist4, color='b')
plt.title('Histogram Y')
plt.xlim([0, 256])
plt.savefig('hist_Y.png')
plt.close()

plt.plot(hist4, color='b')
plt.title('Histogram Cb')
plt.xlim([0, 256])
plt.savefig('hist_Cb.png')
plt.close()

plt.plot(hist5, color='b')
plt.title('Histogram Cr')
plt.xlim([0, 256])
plt.savefig('hist_Cr.png')
plt.close()

# 13
print('13')


def entropy(color, im):
    sat = saturation(color, im)
    entr = 0
    for i in range(0, 256):
        if sat[i] != 0:
            entr += sat[i] / (768 * 512) * log2(sat[i] / (768 * 512))
    return -entr


print("Энтропия R = ", entropy(0, img))
print("Энтропия G = ", entropy(1, img))
print("Энтропия B = ", entropy(2, img))
print("Энтропия Y = ", entropy(0, img_YCC))
print("Энтропия Cb = ", entropy(1, img_YCC))
print("Энтропия Cr = ", entropy(2, img_YCC))

# 14
print('14')


def func_rule(num, im, color):
    rule_array = np.zeros((height - 1, width - 1, 3), dtype=np.uint8)
    color_array = np.copy(im)
    if num == 1:
        for i in range(1, height - 1):
            for j in range(1, width - 1):
                rule_array[i - 1][j - 1][color] = (color_array[i - 1][j - 1][color]) - int(
                    color_array[i - 1][j - 2][color])
        return rule_array

    elif num == 2:
        for i in range(1, height - 1):
            for j in range(1, width - 1):
                rule_array[i - 1][j - 1][color] = (color_array[i - 1][j - 1][color]) - int(
                    color_array[i - 2][j - 1][color])
        return rule_array

    elif num == 3:
        for i in range(1, height - 1):
            for j in range(1, width - 1):
                rule_array[i - 1][j - 1][color] = (color_array[i - 1][j - 1][color]) - int(
                    color_array[i - 2][j - 2][color])
        return rule_array

    elif num == 4:
        for i in range(1, height - 1):
            for j in range(1, width - 1):
                rule_array[i - 1][j - 1][color] = (color_array[i - 1][j - 1][color]) - (
                        int(color_array[i - 1][j - 2][color]) + int(color_array[i - 2][j - 2][color]) + int(
                    color_array[i - 2][j - 1][color])) // 3
        return rule_array


def saturation_DPCM(color, im):
    tmp_color = [0] * 512
    d = func_rule(4, im, color)
    for i in range(0, height - 1):
        for j in range(0, width - 1):
            c = int(d[i][j][color])
            if int(d[i][j][color]) < 0:
                c = int(d[i][j][color]) + 256
            tmp_color[c] += 1
    return tmp_color


def entropy_DPCM(color, im):
    sat = saturation_DPCM(color, im)
    entr = 0
    for i in range(0, len(sat)):
        if sat[i] != 0:
            entr += sat[i] / (768 * 512) * log2(sat[i] / (768 * 512))
    return -entr


for r in range(0, 4):
    for c in range(3):
        temp_ar = func_rule(r, img, c)
        plt.plot(temp_ar, list(range(-200, 200)))
        plt.title('Histogram ' + str(r) + ' ' + str(c))
        plt.xlim([-200, 200])
        plt.savefig('hist_' + str(r) + '_' + str(c) + '.png')
        plt.close()


# 17

def binar(im, thr=127):
    img_ = cv.cvtColor(im, cv.COLOR_BGR2GRAY)
    return cv.threshold(img_, thr, 255, cv.THRESH_BINARY)[1]


plt.imshow(binar(img), cmap='gray')
plt.axis('off')
plt.savefig('binar.png')
