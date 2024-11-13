from gtts import gTTS

import os
import openai
import playsound
import speech_recognition as sr

r = sr.Recognizer()
client = openai.Client(api_key = "")
assistant = client.beta.assistants.create(instructions = "너는 친절한 어시스턴트야.", model = "gpt-4o-mini")
thread = client.beta.threads.create()

def speak(txt):
    tts = gTTS(text = txt, lang = 'ko')
    filename = 'v.mp3'
    tts.save(filename)
    playsound.playsound(filename)
    os.remove(filename)

speak("준비가 완료되었습니다.")

while True:

    with sr.Microphone() as s:
        audio = r.listen(s)

    try:
        text = r.recognize_google(audio, language='ko-KR')

        if text == "종료":
            speak("시스템을 종료합니다.")
            client.beta.assistants.delete(assistant.id)
            break

        message = client.beta.threads.messages.create(
            thread_id = thread.id, 
            role = "user", 
            content = text
            )
        
        run = client.beta.threads.runs.create(
            thread_id = thread.id, 
            assistant_id = assistant.id, 
            instructions = "사용자를 사용자라고 불러. 사용자는 프리미엄 계정을 가지고 있다."
            )
        
        print("Run completed with status: " + run.status)

        if run.status == "completed":
            messages = client.beta.threads.messages.list(thread_id=thread.id)

            print("messages: ")
            for message in messages:
                assert message.content[0].type == "text"
                print({"role": message.role, "message": message.content[0].text.value})

    except sr.UnknownValueError:
        speak("죄송합니다. 다시 말씀해주실 수 있나요?")

    except sr.RequestError:
        speak("문제가 발생하였습니다. 나중에 다시 시도해주십시오.")
