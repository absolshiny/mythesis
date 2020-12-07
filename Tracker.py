import cv2
from Imagegather import correct_image
import json
from coapserverpi import post, get
import math
import numpy as np

def Transform(bbxes,image_measure,realmeshure,orden):
    coordinates= []
    escala= realmeshure[0]/image_measure[1]
    for ind in orden:
        box=bbxes[ind]
        coordinates_r= [(box[0]+box[2]/2)*escala,(box[1]+box[3]/2)*escala]
        coordinates.append(coordinates_r)
    post(str(coordinates)) 
    return

def med_dist(arr1,arr2):
    dist= math.sqrt(((arr2[0]-arr1[0])**2)+((arr2[1]-arr1[1])**2))
    return dist

def ordenar(Allpositions,bbxes,image_measure,realmeshure):
    coordinates=[]
    escala= realmeshure[0]/image_measure[1]
    orden=[]
    r_list=[float(coord) for coord in Allpositions]
    r_list = [r_list[i:i+2] for i in range(0, len(r_list), 2)]
    for box in bbxes:
        coordinates_r= [(box[0]+box[2]/2)*escala,(box[1]+box[3]/2)*escala]
        coordinates.append(coordinates_r)
    for i in range(len(r_list)):
        Dist=[med_dist(r_list[i],coord) for coord in coordinates]
        min_ind=np.argmin(Dist)
        orden.append(min_ind)
        coordinates[min_ind]=[300,300]
    print (orden)
    return orden

with open('points_bbx.txt') as json_file:
    data = json.load(json_file)

bbx=data["Bboxs"]
ptsrc=data["Puntos"]
terrain_dim=(144,84)
Trdict={'csrt':cv2.TrackerCSRT_create,
        'kcf':cv2.TrackerKCF_create,
        'boosting': cv2.TrackerBoosting_create,
        'mil':cv2.TrackerMIL_create,
        'tld':cv2.TrackerTLD_create,
        'medianflow':cv2.TrackerMedianFlow_create,
        'mosse': cv2.TrackerMOSSE_create}

trackers=cv2.MultiTracker_create()

#v=cv2.VideoCapture(r'/home/david/mythesis/VID_20201201_203106.mp4')
v= cv2.VideoCapture(2)
ret,frame=v.read()
frame=cv2.resize(frame,(720,480),interpolation = cv2.INTER_AREA)

frame_2= correct_image(frame,ptsrc)
orden= ordenar(get(),bbx,frame_2.shape[:2],terrain_dim)
for bbi in bbx:
    #cv2.imshow('frame',frame)
    #bbi=cv2.selectROI('frame',frame)
    #cv2.waitKey(0); cv2.destroyAllWindows()

    tracker_i=Trdict['mosse']()
    trackers.add(tracker_i,frame_2,tuple(bbi))
    c=0
    
while True:
    ret,frame=v.read()
    frame=cv2.resize(frame,(720,480),interpolation= cv2.INTER_AREA)
    frame_2= correct_image(frame,ptsrc)
    if not ret:
        break
    (success,boxes)=trackers.update(frame_2)
    if c==40:
        Transform(boxes,frame_2.shape[:2],terrain_dim,orden)
        c=0
    for box in boxes:
        (x,y,w,h)=[int(a)for a in box]
        cv2.rectangle(frame_2,(x,y),(x+w,y+h),(255,0,0),2)
        
    cv2.imshow("Frame",frame_2)
    c+=1
    if cv2.waitKey(2) & 0xFF == ord('q'):
        break
v.release()
cv2.destroyAllWindows()

