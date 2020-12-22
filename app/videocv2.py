import cv2
import os
import RPi.GPIO as GPIO
import logging
logging.basicConfig(level=logging.DEBUG)

# Whether or not to open GUI
ENVIRONMENT = os.getenv('ENVIRONMENT', 'dev')

# If running on RPi
if ENVIRONMENT != 'dev':

	# GPIO config
	GPIO.setwarnings(False)
	GPIO.setmode(GPIO.BCM)
	GPIO.setup(23, GPIO.OUT)
	GPIO.setup(24, GPIO.OUT)

# Load the cascade
face_cascade = cv2.CascadeClassifier('./haarcascade_frontalface_default.xml')

# Read in from default device
video = cv2.VideoCapture(0)

# Get frame height and width then calculate midline
frame_width = video.get(cv2.CAP_PROP_FRAME_WIDTH)
frame_height = video.get(cv2.CAP_PROP_FRAME_HEIGHT)
midline = int(round(frame_width / 2))

# Check video capture is open
if not video.isOpened():
	logging.debug('Cannot open camera')
	exit()

# Loop through each frame
while (True):

	# Read the next frame
	success, frame = video.read(0)

	# Draw vertical centre line
	cv2.line(frame, (midline, 0), (midline, int(frame_height)), (0,255,0), 5)

	# Find faces
	coordinates = face_cascade.detectMultiScale(frame, 1.1, 4)
	logging.debug('{} faces detected'.format(len(coordinates)))

	# If we have any faces, get the nearest one (by height)
	if len(coordinates) > 0:
		[x, y, w, h] = max(coordinates, key=lambda x: x[2])

		# Draw rectangle around nearest face
		cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 10)

		# If face NOT in center, fill in rectangle
		#if int(round(x + w/2)) > midline + 20 or int(round(x + w/2)) < midline - 20:
			#cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), -1)

		# While face to camera's left
		if int(round(x + w/2)) > midline + 30:
			
			# Fill in
			cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), -1)

			if ENVIRONMENT != 'dev':
				# Turn right turn pin off and left turn pin on
				GPIO.output(24, False)
				GPIO.output(23, True)
				logging.debug('left')

		# While face to camera's right
		elif int(round(x + w/2)) < midline - 30:
			# Fill in
			cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), -1)

			if ENVIRONMENT != 'dev':
				# Turn left turn pin off and right turn pin on
				GPIO.output(23, False)
				GPIO.output(24, True)
				logging.debug('right')	

		else:
			logging.debug('center')
			# Turn off both pins
			if ENVIRONMENT != 'dev':
				GPIO.output(23, False)
				GPIO.output(24, False)

	else:
		logging.debug('no face')
		# Turn off both pins
		if ENVIRONMENT != 'dev':
			GPIO.output(23, False)
			GPIO.output(24, False)


	# Show output only if running locally (will throw error in docker)
	if ENVIRONMENT == 'dev':
		cv2.imshow('output', frame)

	# Show frame for 30 ms, break if 'Ctrl + D' pressed
	pressed = cv2.waitKey(29) & 255
	if pressed == 4:
		break

# Close video capture, clear pins, exit
video.release()
if ENVIRONMENT != 'dev':
	GPIO.cleanup()
exit()