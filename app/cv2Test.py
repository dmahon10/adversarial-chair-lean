import cv2
import sys

# Load the cascade
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

# Read the input image
arg = sys.argv[1]
img = cv2.imread(arg, 0)

if img.size == 0:
	print("Cannot load image")
	exit()

# Detect faces
coordinates = face_cascade.detectMultiScale(img, 1.1, 4)

print(coordinates[0])
print(coordinates[0][2])
print(coordinates[1])
# Draw rectangle around the faces
for (x, y, w, h) in coordinates:
    cv2.rectangle(img, (x, y), (x+w, y+h), (255, 0, 0), 10)
# Display the output
cv2.imshow('output', img)
cv2.waitKey(10000)
exit()