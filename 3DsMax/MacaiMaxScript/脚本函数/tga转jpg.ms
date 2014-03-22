sourceImage = openBitMap  "E:\\test.jpg"
outputImage = bitmap sourceImage.width  sourceImage.height
copy  sourceImage outputImage 
outputImage.fileName = "E:\\test1.jpg"
JPEG.setQuality 99
save outputImage