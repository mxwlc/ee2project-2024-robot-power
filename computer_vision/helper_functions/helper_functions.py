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


def gbr2hsv_bounds(gbr):
    """
    Converts an RGB tuple to a lower and upper bounds for image recognition
    :param gbr:
    :return: lower_bound, upper_bound : the HSV lower and upper bounds of the rgb tuple
    """
    lower = np.array([0, 40, 100])
    upper = np.array([0, 255, 255])
    hsv = cv.cvtColor(gbr, cv.COLOR_BGR2HSV)
    lower[0] = hsv[:, :, 0] - 10
    upper[0] = hsv[:, :, 0] + 10

    return lower, upper


def colour_2_hsv(colour):
    color_dict_HSV = {'black': [[180, 255, 30], [0, 0, 0]],
                      'white': [[180, 18, 255], [0, 0, 231]],
                      'red1': [[180, 255, 255], [159, 50, 70]],
                      'red2': [[9, 255, 255], [0, 50, 70]],
                      'green': [[89, 255, 255], [36, 50, 70]],
                      'blue': [[128, 255, 255], [90, 50, 70]],
                      'yellow': [[35, 255, 255], [25, 50, 70]],
                      'purple': [[158, 255, 255], [129, 50, 70]],
                      'orange': [[24, 255, 255], [10, 50, 70]],
                      'gray': [[180, 18, 230], [0, 0, 40]]}
    if colour not in color_dict_HSV:
        bounds = color_dict_HSV['black']
        lower = np.array(bounds[1])
        upper = np.array(bounds[0])
        return lower, upper
    bounds = color_dict_HSV[colour]
    lower = np.array(bounds[1])
    upper = np.array(bounds[0])
    return lower, upper
