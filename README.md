# FacialRecogAdapted
Some variants of a facial recognition algorithm. See README for more information

To use any of the codes below, you'll need: 
  cmake;
  dlib;
  opencv-python;
  face_recognition;

Preferably use python3!

"Built using dlib's state-of-the-art face recognition built with deep learning. The model has an accuracy of 99.38% on the Labeled Faces in the Wild benchmark." https://github.com/ageitgey/face_recognition

knot_FacialRecognition.py is a face recognition algo that sends information to KNoT(the open source meta platform for IoT). See more information about KNoT clicking here
: https://www.knot.cesar.org.br/

To use test.py, you need server.py running. test.py acts like a client that sends an info to server.js throught a TCPsocket. It's also a Face Recognition algorithm.



