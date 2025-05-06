import serial
import json
import time
import pyttsx3
from openai import OpenAI

# Initialize OpenAI client
# ❗️IMPORTANT: For security, DO NOT hardcode your actual API key in code.
# Replace "" with your own key when running locally.
# For example:
# client = OpenAI(api_key="sk-xx...")

client = OpenAI(
    api_key=""  # ← Insert your own API key here if not using environment variables
)
# Initialize text-to-speech engine
engine = pyttsx3.init()

# Serial port configuration
SERIAL_PORT = "/dev/tty.usbmodem14101"
BAUD_RATE = 115200
GAP_SECONDS = 15  # Minimum time between queries

# Query GPT with waste label and confidence
def ask_chatgpt(label, confidence):
    prompt = (
    f"The item is '{label}' with {confidence*100:.1f}% confidence. "
    "In one concise sentence (no more than 20 words), say whether it's recyclable, hazardous, or other, "
    "and briefly describe how to dispose of it."
    )
    response = client.chat.completions.create(
        model="gpt-3.5-turbo",
        messages=[{"role": "user", "content": prompt}]
    )
    gpt_reply = response.choices[0].message.content.strip()
    return f"This item is most likely {label}. {gpt_reply}"

# Main loop
def main():
    print("📡 Listening to serial...")
    last_gpt_time = 0
    previous_label = None
    previous_response = None

    with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=2) as ser:
        while True:
            try:
                line = ser.readline().decode("utf-8").strip()
                if not line.startswith("{"):
                    continue

                try:
                    data = json.loads(line)
                except json.JSONDecodeError:
                    continue

                label = data.get("label", "unknown")
                confidence = float(data.get("confidence", 0))
                current_time = time.time()

                if current_time - last_gpt_time >= GAP_SECONDS:
                    print(f"📥 Received: {data}")

                    # Reuse last response if label hasn't changed
                    if label == previous_label:
                        response = previous_response
                        print("🔁 Reusing previous GPT response.")
                    else:
                        response = ask_chatgpt(label, confidence)
                        previous_label = label
                        previous_response = response

                    print("🤖 GPT says:", response)
                    ser.write((response + "\n").encode("utf-8"))
                    engine.say(f"{label}, {int(confidence * 100)} percent. {response}")
                    engine.runAndWait()

                    last_gpt_time = current_time

            except UnicodeDecodeError:
                continue
            except KeyboardInterrupt:
                print("\n⛔ Interrupted by user.")
                break

if __name__ == "__main__":
    main()
