from gtts import gTTS
import os
import playsound
import speech_recognition as sr
import openai
import time

# OpenAI API 키 설정
openai.api_key = ''

def speak(txt):
    tts = gTTS(text=txt, lang='ko')  # 한국어
    filename = 'voice.mp3'  # 파일 이름
    tts.save(filename)
    playsound.playsound(filename)
    os.remove(filename)  # 파일 유지하고 싶으면 생략

def chat_with_gpt(messages):
    response = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",  # 또는 사용할 모델 이름
        messages=messages
    )
    return response.choices[0].message['content'].strip()

def load_conversation_history(filename):
    if os.path.exists(filename):
        with open(filename, "r", encoding="utf-8") as f:
            return [line.strip() for line in f.readlines()]
    return []

def save_conversation_history(filename, history):
    with open(filename, "w", encoding="utf-8") as f:
        for line in history:
            f.write(line + "\n")

def format_messages(history):
    messages = [{"role": "system", "content": "You are a helpful assistant."}]
    for line in history:
        if line.startswith("나:"):
            messages.append({"role": "user", "content": line[3:]})
        elif line.startswith("ChatGPT:"):
            messages.append({"role": "assistant", "content": line[8:]})
    return messages

conversation_history = load_conversation_history("conversation_history.txt")
messages = format_messages(conversation_history)

Recognizer = sr.Recognizer()
mic = sr.Microphone()

try:
    while True:
        with mic as source:
            print("듣는중....")
            audio = Recognizer.listen(source)

        try:
            data = Recognizer.recognize_google(audio, language="ko")
            print("나:", data)

            if "그만" in data:
                speak("프로그램을 종료합니다.")
                break
            elif "지워" in data:
                conversation_history = []
                messages = format_messages(conversation_history)
                speak("대화 내용을 지웠습니다.")
                continue
            elif "자기소개" in data or "자기 소개" in data:
                time.sleep(2)
                playsound.playsound("heytesla.mp3")
                continue

            conversation_history.append(f"나: {data}")
            messages = format_messages(conversation_history)
            response = chat_with_gpt(messages)
            print("ChatGPT:", response)
            conversation_history.append(f"ChatGPT: {response}")
            speak(response)

        except sr.UnknownValueError:
            speak("이해하지 못하는 말이에요")
        except sr.RequestError as e:
            speak("에러")
finally:
    save_conversation_history("conversation_history.txt", conversation_history)
    print("대화 내용이 'conversation_history.txt' 파일에 저장되었습니다.")
