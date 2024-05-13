import openai

openai.api_base = "https://openrouter.ai/api/v1"
openai.api_key = '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'

user_msg_file_path = r'C:\Users\everp\Documents\GitHub\2a23-smart_delivery\chatbot\messages\merged.txt'
with open(user_msg_file_path, 'r') as user_msg_file:
    user_msg = user_msg_file.read()

response = openai.ChatCompletion.create(
    model="openai/gpt-3.5-turbo",
    messages=[
        {"role": "system", "content": ""},
        {"role": "user", "content": user_msg},
    ],
    headers={
        "HTTP-Referer": 'http://localhost:3000',
        "X-Title": 'Your App Name',
    },
)

reply_content = response.choices[0].message['content']

response_file_path = r'C:\Users\everp\Documents\GitHub\2a23-smart_delivery\chatbot\messages\response.txt'
with open(response_file_path, 'w') as response_file:
    response_file.write(reply_content)

print(reply_content)
