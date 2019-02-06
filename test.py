import face_recognition
import cv2
import socket

# This is a super simple (but slow) example of running face recognition on live video from your webcam.
# There's a second example that's a little more complicated but runs faster.

# PLEASE NOTE: This example requires OpenCV (the `cv2` library) to be installed only to read from your webcam.
# OpenCV is *not* required to use the face_recognition library. It's only required if you want to run this
# specific demo. If you have trouble installing it, try any of the other demos that don't require it instead.

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

HOST = '127.0.0.1'     # Endereco IP do Servidor
PORT = 8884            # Porta que o Servidor esta
tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
dest = (HOST, PORT)
tcp.connect(dest)
print ('Para sair use CTRL+X\n')


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

         
        name = "Unknown"

        # If a match was found in known_face_encodings, just use the first one.
        if True in matches:
            first_match_index = matches.index(True)
            name = known_face_names[first_match_index]
        else:
            

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