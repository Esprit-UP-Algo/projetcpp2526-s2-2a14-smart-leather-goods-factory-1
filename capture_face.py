import cv2
import os
import sys

save_dir = "C:/Users/AZERTY/Desktop/qt/database_faces"

def capture_face(cin_value):
    os.makedirs(save_dir, exist_ok=True)

    video_capture = cv2.VideoCapture(0)

    if not video_capture.isOpened():
        print("CAMERA_ERROR")
        return

    while True:
        ret, frame = video_capture.read()
        if not ret:
            print("CAMERA_ERROR")
            break

        cv2.putText(frame, "Press S to save photo - Press Q to quit",
                    (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 0), 2)

        cv2.imshow("Capture Face ID", frame)

        key = cv2.waitKey(1) & 0xFF

        if key == ord('s'):
            filename = os.path.join(save_dir, f"{cin_value}.jpg")
            cv2.imwrite(filename, frame)
            print(f"PHOTO_SAVED:{filename}")
            break

        elif key == ord('q'):
            print("CAPTURE_CANCELLED")
            break

    video_capture.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("NO_CIN")
    else:
        cin_value = sys.argv[1].strip()
        capture_face(cin_value)