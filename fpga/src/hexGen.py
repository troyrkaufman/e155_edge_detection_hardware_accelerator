# read image.png, crop it to 640x480, and save it as imageCropped.png

import cv2

# read image.png
img = cv2.imread('image.png')

# get dimensions of the image
height, width, _ = img.shape

# calculate the center of the image
center_y, center_x = height // 2, width // 2

# calculate the cropping box
start_x = max(center_x - 320, 0)
start_y = max(center_y - 240, 0)
end_x = start_x + 640
end_y = start_y + 480

# crop the image
imgCropped = img[start_y:end_y, start_x:end_x]

# if the cropped image is smaller than 640x480, pad it with black
if imgCropped.shape[0] < 480 or imgCropped.shape[1] < 640:
  imgCropped = cv2.copyMakeBorder(imgCropped, 
                  top=0, 
                  bottom=max(480 - imgCropped.shape[0], 0), 
                  left=0, 
                  right=max(640 - imgCropped.shape[1], 0), 
                  borderType=cv2.BORDER_CONSTANT, 
                  value=[0, 0, 0])

# save it as imageCropped.png
cv2.imwrite('imageCropped.png', imgCropped)

def formatHex(hexValue):
  return hexValue[2:].zfill(2)
# read imageCropped.png and stored hex values for r, g, b in hex.txt
imgCropped = cv2.imread('imageCropped.png')
f = open('hex.txt', 'w')
with open('hex.txt', 'w') as f:
  for i in range(480):
      for j in range(640):
          r, g, b = imgCropped[i, j]
          f.write(formatHex(hex(r)) + formatHex(hex(g)) + formatHex(hex(b)) + '\n')
