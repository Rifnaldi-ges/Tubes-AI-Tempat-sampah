import cv2
import numpy as np
import serial
import time
from tensorflow.keras.models import load_model
from tensorflow.keras.preprocessing.image import img_to_array

# Load model
model = load_model('d:/vs/AI/model.h5')
class_labels = ['metal', 'paper', 'plastic']

# Ambil ukuran input dari model
input_height, input_width = model.input_shape[1:3]

# Buka kamera
cap = cv2.VideoCapture(0)

# Hubungkan ke ESP32 (ubah COM sesuai komputer kamu)
esp = serial.Serial('COM6', 115200, timeout=1)
time.sleep(2)

print("Sistem siap mendeteksi...")

last_label = None
cooldown = 5  # waktu jeda (detik) setelah deteksi
last_time = time.time()

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Preprocess
    img = cv2.resize(frame, (input_width, input_height))
    img_array = img_to_array(img)
    img_array = np.expand_dims(img_array, axis=0) / 255.0

    # Prediksi
    prediction = model.predict(img_array, verbose=0)
    class_index = np.argmax(prediction)
    label = class_labels[class_index]
    confidence = np.max(prediction)

    # Tampilkan deteksi
    cv2.putText(frame, f'{label} ({confidence*100:.2f}%)', (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    cv2.imshow('Deteksi Sampah', frame)

    # Cek apakah prediksi di atas 70%, berbeda, dan sudah lewat cooldown
    current_time = time.time()
    if confidence > 0.70 and label != last_label and current_time - last_time > cooldown:
        if label == 'paper':
            esp.write(b'L\n')
        elif label == 'metal':
            esp.write(b'R\n')
        elif label == 'plastic':
            esp.write(b'P\n')

        print(f'Kirim perintah: {label} (Confidence: {confidence*100:.2f}%)')
        last_label = label
        last_time = current_time

    # Tekan q untuk keluar
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Bersihkan
cap.release()
cv2.destroyAllWindows()
esp.close()