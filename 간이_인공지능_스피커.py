from gtts import gTTS
import os
import playsound
import speech_recognition as sr
import openai
import time

# OpenAI API 키 설정
openai.api_key = ''  # 환경 변수로 설정하는 것을 권장합니다

# 전역 변수
is_speaking = False
activation_word = "테슬라"  # 호출 단어
conversation_active = False  # 대화 모드 상태

def speak(txt):
    global is_speaking
    is_speaking = True  # 답변 중 플래그 설정
    tts = gTTS(text=txt, lang='ko')
    filename = "voice.mp3"
    tts.save(filename)
    playsound.playsound(filename)
    os.remove(filename)
    time.sleep(0.5)  # 답변 후 간격
    is_speaking = False  # 답변이 끝났음을 표시

def chat_with_gpt(messages):
    try:
        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=messages
        )
        return response['choices'][0]['message']['content'].strip()
    except Exception as e:
        print("ChatGPT 응답 중 오류 발생:", e)
        return "오류가 발생했습니다."

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
    messages = [{"role": "system", "content": "You are Tesla."}]
    for line in history:
        if line.startswith("나:"):
            messages.append({"role": "user", "content": line[3:]})
        elif line.startswith("ChatGPT:"):
            messages.append({"role": "assistant", "content": line[8:]})
    return messages

# 대화 기록 불러오기 및 초기화
conversation_history = load_conversation_history("conversation_history.txt")
messages = format_messages(conversation_history)

# 음성 인식 및 마이크 설정
Recognizer = sr.Recognizer()
mic = sr.Microphone()

try:
    while True:
        # 대화 모드가 꺼져 있으면 호출 대기
        if not conversation_active:
            print("듣는 중... 호출 대기 중입니다.")
        
        with mic as source:
            Recognizer.adjust_for_ambient_noise(source)
            audio = Recognizer.listen(source)

        try:
            # 사용자 음성을 텍스트로 변환
            data = Recognizer.recognize_google(audio, language="ko")
            print("나:", data)

            # 대화 모드 활성화 단어 확인
            if activation_word in data and not conversation_active:
                conversation_active = True
                speak("네, 무슨 도움이 필요한가요?")
                continue

            # 대화 모드가 활성화된 경우 처리
            if conversation_active:
                # "그만"을 통해 대화 모드 종료
                if "그만" in data:
                    conversation_active = False
                    speak("네, 알겠습니다.")
                    break
                elif "지워" in data:
                    # 대화 기록 초기화
                    conversation_history = []
                    messages = format_messages(conversation_history)
                    os.remove("conversation_history.txt")
                    speak("대화 내용을 지웠습니다.")
                    continue

                # 대화 기록에 추가
                conversation_history.append(f"나: {data}")
                messages = format_messages(conversation_history)
                response = chat_with_gpt(messages)
                print("ChatGPT:", response)
                
                # 빈 줄 추가하여 대화 기록에 저장
                conversation_history.append(f"ChatGPT: {response}\n")
                speak(response)

        except sr.UnknownValueError:
            print("이해하지 못했습니다.")
        except sr.RequestError as e:
            print("에러:", e)

finally:
    # 대화 기록 파일에 저장
    save_conversation_history("conversation_history.txt", conversation_history)
    print("대화 내용이 'conversation_history.txt' 파일에 저장되었습니다.")
