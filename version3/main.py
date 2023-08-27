from PIL import Image
import numpy as np
from math import *
import struct
import cv2 as cv
from matplotlib import pyplot as plt

kodim_ = 'kodim02.bmp'
img = np.array(Image.open(kodim_))


with open('%s' % kodim_, "rb") as f:
    file_data = f.read()

header_data = struct.unpack('<2sIHHIIIIHHIIIIII', file_data[:54])
width = struct.unpack_from('<i', file_data, 18)[0]
height = struct.unpack_from('<i', file_data, 22)[0]


