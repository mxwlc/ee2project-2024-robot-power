import numpy as np
import cv2 as cv
from helper_functions import helper_functions as hf

"""
TODO finish drawing opencvlogo
"""


def pol_to_car(polar_coordinates):
    x = int(polar_coordinates[0] * np.cos(polar_coordinates[1]))
    y = int(polar_coordinates[0] * np.sin(polar_coordinates[1]))
    return np.array((x, y))


def car_to_pol(cart_coordinates):
    magnitude = np.sqrt(cart_coordinates[0] ** 2 + cart_coordinates[1] ** 2)
    angle = np.arctan2(cart_coordinates[1], cart_coordinates[0])
    return np.array((magnitude, angle))


def vertices_polygon(r, n):
    angles = np.linspace(0, (2 * np.pi), n, endpoint=False)
    vertices = np.zeros((n, 2), np.int32)
    for index, theta in np.ndenumerate(angles):
        vertices[index] = pol_to_car((r, theta))
    vertices = hf.format_point_array(vertices)
    return vertices


def rotate(vertices, angle):
    for index in range(0, len(vertices)):
        xp = int(vertices[index][0] * np.cos(angle) - vertices[index][1] * np.sin(angle))
        yp = int(vertices[index][0] * np.sin(angle) + vertices[index][1] * np.cos(angle))
        vertices[index] = np.array((xp, yp))
    return vertices


def construct_openCV_logo(img, vertices, origin):
    if len(vertices) != 3:
        raise ValueError("The input vertices should have three values")
    # circle 1 - top
    center_1 = vertices[0]
    center_1 = (center_1[0] + origin[0], center_1[1] + origin[1])
    cv.ellipse(img, np.array(center_1), (50, 50), 0, (90 - 20), (90 + 20),  255, -1)


def main():
    vertices = vertices_polygon(150, 3)
    print(vertices)
    img = np.zeros((500, 500, 1), np.uint8)
    origin = (img.shape[1] // 2, img.shape[0] // 2)
    vertices = rotate(vertices, -90 * np.pi / 180)

    for point in vertices:
        point = (point[0] + origin[0], point[1] + origin[1])
        cv.circle(img, (int(point[0]), int(point[1])), 2, 255, -1)

    construct_openCV_logo(img, vertices, origin)
    cv.imshow('img', img)
    k = cv.waitKey(0)

    if k == ord("q"):
        exit()


if __name__ == "__main__":
    main()
