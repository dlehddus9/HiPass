import cv2
import boto3
import MySQLdb
import numpy as np
import time
import matplotlib.pyplot as plt
from datetime import datetime

#현재시간과 역이 찍힘.
now = datetime.today().strftime("%Y-%m-%d-%H-%M-%S")

var1="hansung-"
var2=".jpg"
var3=var1+now+var2
print(var3)

st=var3.split('-')[0]
ye=var3.split('-')[1]
mo=var3.split('-')[2]
da=var3.split('-')[3]

print(st)
print(ye)
print(mo)
print(da)

cap = cv2.VideoCapture(0)
ret, frame = cap.read()
cv2.imwrite(var3, frame)
cap.release()
print('Target Img Captured')

#현재 찍힌 사진이 s3 버킷에 올라감
s3 = boto3.resource('s3')
filename=var3
s3.Bucket('dongyeon1').put_object(Key=filename, Body=filename)
img1 = "https://s3-ap-northeast-1.amazonaws.com/dongyeon1/"
img2 = var3
img3 = img1+img2
print(img3)

#db연결
db = MySQLdb.connect(host='localhost', user='root', passwd='rkd123', db='pythonprogramming')
cur = db.cursor()



sql = "insert into image(station,year,month,day,img_url) values(%s,%s,%s,%s,%s)"

cur.execute(sql, (st,ye,mo,da,img3))

db.commit()
db.close()