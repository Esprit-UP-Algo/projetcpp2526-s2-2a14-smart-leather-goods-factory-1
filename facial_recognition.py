import face_recognition
import cv2
import os
import time
import numpy as np
from PIL import Image

database_folder = "C:/Users/AZERTY/Desktop/qt/database_faces"
clean_folder = "C:/Users/AZERTY/Desktop/qt/database_faces_clean"


def prepare_images():
    if not os.path.exists(database_folder):
        print("DB_FOLDER_NOT_FOUND")
        return False

    os.makedirs(clean_folder, exist_ok=True)

    for filename in os.listdir(database_folder):
        if not filename.lower().endswith((".png", ".jpg", ".jpeg")):
            continue

        src = os.path.join(database_folder, filename)
        dst = os.path.join(clean_folder, filename)

        try:
            img = Image.open(src).convert("RGB")
            img.save(dst, "JPEG")
        except Exception as e:
            print(f"PREP_ERROR:{filename} -> {e}")

    return True


def load_known_faces():
    known_faces = []

    if not os.path.exists(clean_folder):
        print("CLEAN_FOLDER_NOT_FOUND")
        return known_faces

    for filename in os.listdir(clean_folder):
        filepath = os.path.join(clean_folder, filename)

        if not filename.lower().endswith((".png", ".jpg", ".jpeg")):
            continue

        try:
            image = face_recognition.load_image_file(filepath)
            encodings = face_recognition.face_encodings(image)

            if len(encodings) > 0:
                cin_value = os.path.splitext(filename)[0]  # ex: 12345678
                known_faces.append((cin_value, encodings[0]))
            else:
                print(f"NO_FACE_IN_IMAGE:{filename}")

        except Exception as e:
            print(f"IMAGE_ERROR:{filename} -> {e}")

    return known_faces


def recognize_face():
    if not prepare_images():
        return None

    known_faces = load_known_faces()

    if len(known_faces) == 0:
        print("NO_DATABASE_FACES")
        return None

    video_capture = cv2.VideoCapture(0)

    if not video_capture.isOpened():
        print("CAMERA_ERROR")
        return None

    start_time = time.time()
    recognized_face = None

    while True:
        ret, frame = video_capture.read()
        if not ret:
            print("FRAME_ERROR")
            break

        small_frame = cv2.resize(frame, (0, 0), fx=0.25, fy=0.25)
        rgb_small_frame = cv2.cvtColor(small_frame, cv2.COLOR_BGR2RGB)
        rgb_small_frame = np.ascontiguousarray(rgb_small_frame, dtype=np.uint8)

        try:
            face_locations = face_recognition.face_locations(rgb_small_frame, model="hog")
            face_encodings = face_recognition.face_encodings(rgb_small_frame, face_locations)
        except Exception as e:
            print(f"FACE_RUNTIME_ERROR:{e}")
            break

        for (top, right, bottom, left) in face_locations:
            top *= 4
            right *= 4
            bottom *= 4
            left *= 4
            cv2.rectangle(frame, (left, top), (right, bottom), (0, 255, 0), 2)

        for face_encoding in face_encodings:
            best_match_cin = None
            best_match_score = float("inf")

            for cin_value, known_face_encoding in known_faces:
                face_distances = face_recognition.face_distance([known_face_encoding], face_encoding)

                if len(face_distances) > 0 and face_distances[0] < best_match_score:
                    best_match_score = face_distances[0]
                    best_match_cin = cin_value

            if best_match_cin is not None and best_match_score < 0.55:
                recognized_face = best_match_cin
                break

        cv2.imshow("Face Recognition - press q to quit", frame)

        if recognized_face:
            break

        if time.time() - start_time > 10:
            print("UNKNOWN")
            break

        if cv2.waitKey(1) & 0xFF == ord('q'):
            print("STOPPED")
            break

    video_capture.release()
    cv2.destroyAllWindows()

    return recognized_face


recognized_name = recognize_face()

if recognized_name:
    print(f"USER:{recognized_name}")