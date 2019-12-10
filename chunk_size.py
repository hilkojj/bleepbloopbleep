"""

This script is used to find a proper chunk-size without wasting texture-memory

"""


import math

for chunk_size in range(4, 40, 1):
    for column_height in range(chunk_size * 4, 500, chunk_size):

        if column_height < 200:
            continue

        blocks_per_pixel = 16

        pixels = column_height * chunk_size * chunk_size / blocks_per_pixel

        texture_width = math.sqrt(pixels)

        if not texture_width.is_integer():
            continue

        print("texture_width:", texture_width, "chunk_size:", chunk_size, column_height, column_height / chunk_size)
        if math.log(texture_width, 2).is_integer():
            print("!!!!!!!!!!!!")
