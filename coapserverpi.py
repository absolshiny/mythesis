#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Dec  5 23:07:33 2020

@author: david
"""
from coapthon.client.helperclient import HelperClient
def post(payload):
    host = "10.42.0.1"
    port = 5683
    path ="Allpositions_vision"
    #payload="[[1,2],[3,4],[5,6],[2,5],[1,4],[5,2]]"
    client = HelperClient(server=(host, port))
    response=client.post(path,payload)
    client.stop()
    return

def get():
    host = "10.42.0.1"
    port = 5683
    path ="Allpositions"
    #payload="[[1,2],[3,4],[5,6],[2,5],[1,4],[5,2]]"
    client = HelperClient(server=(host, port))
    response=client.get(path)
    client.stop()
    return list(response.payload[1:-1].split(","))

if __name__=='__main__':
    #post("[[30,17],[30,40],[30,60],[6,17],[6,40],[6,60]]")
    print(get())