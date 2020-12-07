#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Dec  4 09:40:34 2020

@author: david
"""

import cv2
import numpy as np
#import matplotlib.pyplot as plt
#import matplotlib.image as mpimg


def correct_image(image,ptsrc):
        #im_src= cv2.imread("/home/david/mythesis/fotos/IMG_20201204_102739.jpg")
        #imgplot_v1 = plt.imshow(im_src)
        #IMG=cv2.resize(im_src,(1600,1800),interpolation = cv2.INTER_AREA)
        #imgplot = plt.imshow(imgplot_v1)
        #plt.show()
        #ptsrc_IMG= np.array([[39,37.5],[374,56],[37,610],[393,584]])
        ptsrc_np=np.array(ptsrc)
        #ptdst=np.array([[0,0],[132*4,0],[0,336],[132*4,336]])#vis6ta normal
        ptdst=np.array([[0,0],[144*4,0],[0,336],[144*4,336]])
        #ptdst=np.array(ptdst_lst)
        h, status = cv2.findHomography(ptsrc_np, ptdst)
        im_out = cv2.warpPerspective(image, h, (144*4,336))
        #cv2.imwrite("File.jpg", im_out)
        #imgplot = plt.imshow(im_out)
        #plt.show()
        return im_out
    
