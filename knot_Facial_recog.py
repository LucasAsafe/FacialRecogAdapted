import face_recognition
import cv2
import socket
from knotpy import *
# This is a super simple (but slow) example of running face recognition on live video from your webcam.
# There's a second example that's a little more complicated but runs faster.

# PLEASE NOTE: This example requires OpenCV (the `cv2` library) to be installed only to read from your webcam.
# OpenCV is *not* required to use the face_recognition library. It's only required if you want to run this
# specific demo. If you have trouble installing it, try any of the other demos that don't require it instead.

credentials = {
	'uuid': '5e2d5dfd-e2e1-4b40-8a06-6f0a0b020000',
	'token': 'a608e9732ad03b92d3cbf1845e3dab4542a7b873',
	'servername': 'localhost',
	'port': 3000
}
conn = KnotConnection('socketio', credentials)

myThings = conn.myDevices()

conn = KnotConnection('socketio', credentials)
# Get a reference to webcam #0 (the default one)
video_capture = cv2.VideoCapture(0)

# Load a sample picture and learn how to recognize it.
lucasface = face_recognition.load_image_file("lucas.jpg")
lucasface_encoding = face_recognition.face_encodings(lucasface)[0]

# Load a second sample picture and learn how to recognize it.
lucasinsta = face_recognition.load_image_file("lucas1.jpg")
lucasinsta_encoding = face_recognition.face_encodings(lucasinsta)[0]

paulo = face_recognition.load_image_file("paulo.png")
paulo_encoding = face_recognition.face_encodings(paulo)[0]

suzy = face_recognition.load_image_file("suzy.jpeg")
suzy_encoding = face_recognition.face_encodings(suzy)[0]

barbara = face_recognition.load_image_file("barbara.jpeg")
barbara_encoding = face_recognition.face_encodings(barbara)[0]

antonio = face_recognition.load_image_file("antonio.jpeg")
antonio_encoding = face_recognition.face_encodings(antonio)[0]

florencia = face_recognition.load_image_file("florencia.jpeg")
florencia_encoding = face_recognition.face_encodings(florencia)[0]

anderson = face_recognition.load_image_file("anderson.jpeg")
anderson_encoding = face_recognition.face_encodings(anderson)[0]

# Create arrays of known face encodings and their names
known_face_encodings = [
    lucasface_encoding,
    paulo_encoding,
    suzy_encoding,
    barbara_encoding,
    antonio_encoding,
    florencia_encoding,
    anderson_encoding
]
known_face_names = [
    "Lucas",
    "Paulo",
    "Suzy",
    "Barbara",
    "Antonio",
    "Florencia",
    "Anderson"
]
name = "Unknown"
while True:
    # Grab a single frame of video
    ret, frame = video_capture.read()
    
    # Convert the image from BGR color (which OpenCV uses) to RGB color (which face_recognition uses)
    rgb_frame = frame[:, :, ::-1]

    # Find all the faces and face enqcodings in the frame of video
    face_locations = face_recognition.face_locations(rgb_frame)
    face_encodings = face_recognition.face_encodings(rgb_frame, face_locations)

   
    # Loop through each face in this frame of video
    for (top, right, bottom, left), face_encoding in zip(face_locations, face_encodings):
        # See if the face is a match for the known face(s)
        matches = face_recognition.compare_faces(known_face_encodings, face_encoding)
        

        # If a match was found in known_face_encodings, just use the first one.
        if True in matches:
            first_match_index = matches.index(True)   
            name1 = name              
            name = known_face_names[first_match_index]

            if name1 != name:   
                print("Enviando para o knot mesmo que esteja igual hahahahahahahah")
                for thing in myThings:
                    # print(thing)
                    # print(60*'-')
                    # print('DATA')
                    data = conn.setData(thing['uuid'],1,True)
                    print(data)
                    '''print('Set data')
                    if thing.get('schema'):
                        for sensor in thing.get('schema'):
                            if sensor['name'] == 'LED':
                                conn.setData(thing['uuid'], sensor['sensor_id'], True)
                    print(60*'*')'''
        else:
            name = "Unknown"
            print("aqui e o else pro Unknown")
            for thing in myThings:
                print(thing)
                print(60*'-')
                print('DATA')
                data = conn.setData(thing['uuid'],1,False)
                print(data)
                '''print('Set data')
                if thing.get('schema'):
                    for sensor in thing.get('schema'):
                        if sensor['name'] == 'LED':
                            conn.setData(thing['uuid'], sensor['sensor_id'], True)
                print(60*'*')'''

        # Draw a box around the face
        cv2.rectangle(frame, (left, top), (right, bottom), (0, 0, 255), 2)

        # Draw a label with a name below the face
        cv2.rectangle(frame, (left, bottom - 35), (right, bottom), (0, 0, 255), cv2.FILLED)
        font = cv2.FONT_HERSHEY_DUPLEX
        cv2.putText(frame, name, (left + 6, bottom - 6), font, 1.0, (255, 255, 255), 1)

    # Display the resulting image
    cv2.imshow('Video', frame)

    # Hit 'q' on the keyboard to quit!
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release handle to the webcam
video_capture.release()

cv2.destroyAllWindows()