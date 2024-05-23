import datetime
import cv2 as cv
import numpy as np
FORMAT = "%Y%m%d"


def save_image(image, tag, directory=None):
    """
    Saves an image to /images/ folder.
    With the format
    :param image: OpenCV image object
    :param tag: A string to identify the image per topic
    :param directory: Directory to save the image to
        """
    date = datetime.datetime.now().strftime(FORMAT)
    if directory is None:
        filename = f"images/{tag}{date}.png"
    else:
        filename = f"images/{directory}/{tag}{date}.png"
    cv.imwrite(filename, image)
    print(f"Saved as {filename}")
    return 0


def format_point_array(vertices):
    """
    Converts a list of vertices into a formatted numpy array of shape (-1,1,2).
    :param vertices:
    :return:
    """
    vertices = np.array(vertices)
    vertices.reshape((-1, 1, 2))
    return vertices
