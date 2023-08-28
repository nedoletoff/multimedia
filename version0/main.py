import math
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import os
import struct
import cv2

from PIL import Image

blue_color = 0
green_color = 1
red_color = 2


class BMP:
    def __init__(self, img_path) -> None:
        self.rgb = None
        self.cr = None
        self.cb = None
        self.y = None
        self.blue = None
        self.green = None
        self.red = None
        self.img_path = img_path
        self.bmp = open(img_path, 'rb')
        self.type = self.bmp.read(2).decode()
        self.size = struct.unpack('I', self.bmp.read(4))
        self.reserved1 = struct.unpack('H', self.bmp.read(2))
        self.reserved2 = struct.unpack('H', self.bmp.read(2))
        self.offset = struct.unpack('I', self.bmp.read(4))
        self.dib_header_size = struct.unpack('I', self.bmp.read(4))
        self.width = struct.unpack('I', self.bmp.read(4))
        self.height = struct.unpack('I', self.bmp.read(4))
        self.color_planes = struct.unpack('H', self.bmp.read(2))
        self.bits_per_pixel = struct.unpack('H', self.bmp.read(2))
        self.compression_method = struct.unpack('I', self.bmp.read(4))
        self.raw_image_size = struct.unpack('I', self.bmp.read(4))
        self.horizontal_resolution = struct.unpack('I', self.bmp.read(4))
        self.vertical_resolution = struct.unpack('I', self.bmp.read(4))
        self.number_of_colours = struct.unpack('I', self.bmp.read(4))
        self.important_colours = struct.unpack('I', self.bmp.read(4))
        self.img = np.array(Image.open(img_path))
        self.bmp.close()

    def __str__(self):
        return f"Type: {self.type}\nSize: {self.size}\nReserved1: {self.reserved1}\nReserved2: {self.reserved2}\n" + \
            f"Offset: {self.offset}\nDIB Header Size: {self.dib_header_size}\nWidth: {self.width}\n" + \
            f"Height: {self.height}\nColor Planes: {self.color_planes}\nBits Per Pixel: {self.bits_per_pixel}\n" + \
            f"Compression Method: {self.compression_method}\nRaw Image Size: {self.raw_image_size}\n" + \
            f"Horizontal Resolution: {self.horizontal_resolution}\nVertical Resolution: {self.vertical_resolution}\n" + \
            f"Number of Colours: {self.number_of_colours}\nImportant Colours: {self.important_colours}"

    def get_img(self):
        return self.img

    def red_img(self):
        if self.red is None:
            red = self.img.copy()
            for i in red:
                for j in i:
                    j[green_color] = 0
                    j[blue_color] = 0
            self.red = red.copy()
            return red
        return self.red.copy()

    def green_img(self):
        if self.green is None:
            green = self.img.copy()
            for i in green:
                for j in i:
                    j[red_color] = 0
                    j[blue_color] = 0
            self.green = green.copy()
            return green
        return self.green.copy()

    def blue_img(self):
        if self.blue is None:
            blue = self.img.copy()
            for i in blue:
                for j in i:
                    j[red_color] = 0
                    j[green_color] = 0
            self.blue = blue.copy()
            return blue
        return self.blue.copy()

    def y_img(self):
        if self.y is None:
            y = np.zeros((self.height[0], self.width[0], 1), dtype=np.int32)
            for i in range(self.height[0]):
                for j in range(self.width[0]):
                    y[i][j] = (self.img[i][j][red_color] * 0.299 + self.img[i][j][green_color] * 0.587 +
                               self.img[i][j][blue_color] * 0.114)
            self.y = y.copy()
            return y.copy()
        return self.y.copy()

    def cb_img(self):
        if self.cb is None:
            y = self.y_img()
            cb = np.zeros((self.height[0], self.width[0], 1), dtype=np.int32)
            for i in range(self.height[0]):
                for j in range(self.width[0]):
                    cb[i][j] = (self.img[i][j][blue_color] - y[i][j]) * 0.5643 + 128
            self.cb = cb.copy()
            return cb.copy()

    def cr_img(self):
        if self.cr is None:
            y = self.y_img()
            cr = np.zeros((self.height[0], self.width[0], 1), dtype=np.int32)
            for i in range(self.height[0]):
                for j in range(self.width[0]):
                    cr[i][j] = (self.img[i][j][red_color] - y[i][j]) * 0.7132 + 128
            self.cr = cr.copy()
            return cr.copy()

    def ycbcr_to_rgb(self):
        y = self.y_img()
        cb = self.cb_img()
        cr = self.cr_img()
        rgb = np.zeros((self.height[0], self.width[0], 3), dtype=np.int32)
        for i in range(self.height[0]):
            for j in range(self.width[0]):
                rgb[i][j][red_color] = y[i][j] + 1.402 * (cr[i][j] - 128)
                rgb[i][j][green_color] = y[i][j] - 0.714 * (cr[i][j] - 128) - 0.344 * (cb[i][j] - 128)
                rgb[i][j][blue_color] = y[i][j] + 1.772 * (cb[i][j] - 128)
        self.rgb = rgb.copy()
        return rgb


def draw_img(img: np.array):
    plt.imshow(img)
    plt.show()


def draw_img2(img: np.array):
    plt.imshow(img, cmap='gray', vmin=0, vmax=255)
    plt.show()


def save_img(img: np.array, name: str):
    path = "saved_img"
    fe = ".bmp"
    if not os.path.exists(path):
        os.mkdir(path)
    name = path + os.sep + name + fe
    cv2.imwrite(name, img)


def save_img2(img: np.array, name: str):
    path = "saved_img"
    fe = ".bmp"
    if not os.path.exists(path):
        os.mkdir(path)
    name = path + os.sep + name + fe
    img = img.astype(np.uint8)
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    img = Image.fromarray(img)
    img.save(name)


def decimation1(img: np.array, decimation: int):



def main():
    bmp = BMP("kodim02.bmp")
    print(bmp)
    fig, ax = plt.subplots(2)
    ax[0].imshow(bmp.get_img())
    ax[1].imshow(bmp.ycbcr_to_rgb())
    plt.show()


if __name__ == '__main__':
    main()
