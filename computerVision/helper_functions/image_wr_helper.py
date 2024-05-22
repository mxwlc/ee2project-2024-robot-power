import datetime
import cv2 as cv

FORMAT = "%Y%m%d"


def save_image(image, tag, directory=None):
    """
    Saves an image to /images/ folder.
    With the format
    :param image: OpenCV image object
    :param tag: A string to identify the image per topic
        """
    date = datetime.datetime.now().strftime(FORMAT)
    if directory is None:
        filename = f"images/{tag}{date}.png"
    else:
        filename = f"images/{directory}/{tag}{date}.png"
    cv.imwrite(filename, image)
    print(f"Saved as {filename}")
    return 0

