import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.application import MIMEApplication
import requests
import json

def read_json_file(file_path):
    try:
        with open(file_path, 'r') as json_file:
            data = json.load(json_file)
            return data
    except FileNotFoundError:
        print(f"File not found: {file_path}")
    except json.JSONDecodeError as e:
        print(f"Failed to decode JSON: {e}")

def send_email(sender_email, sender_password, recipient_email, subject, message_body, attachment_paths=None):
    try:
        # Create the email message
        message = MIMEMultipart()
        message['From'] = sender_email
        message['To'] = recipient_email
        message['Subject'] = subject

        # Attach the message body
        message.attach(MIMEText(message_body, 'plain'))

        # Attach multiple files (if provided)
        if attachment_paths:
            for attachment_path in attachment_paths:
                with open(attachment_path, "rb") as attachment_file:
                    part = MIMEApplication(attachment_file.read(), Name=attachment_path.split("/")[-1])
                    part['Content-Disposition'] = f'attachment; filename="{attachment_path.split("/")[-1]}"'
                    message.attach(part)

        # Connect to Gmail's SMTP server
        smtp_server = smtplib.SMTP("smtp.gmail.com", 587)
        smtp_server.starttls()
        smtp_server.login(sender_email, sender_password)

        # Send the email
        smtp_server.sendmail(sender_email, recipient_email, message.as_string())

        # Quit the SMTP server
        smtp_server.quit()
        print("Email sent successfully!")

    except Exception as e:
        print(f"An error occurred: {str(e)}")

def send_sms(sid, token, to, from_, body):
    try:
        account_sid = sid
        auth_token = token

        message = body
        from_number = from_
        to_number = to

        url = f'https://api.twilio.com/2010-04-01/Accounts/{account_sid}/Messages.json'

        payload = {
            'Body': message,
            'From': from_number,
            'To': to_number
        }

        response = requests.post(url, auth=(account_sid, auth_token), data=payload)

        if response.status_code == 201:
            print("SMS sent successfully!")
        else:
            print("SMS sending failed.")

        

    except Exception as e:
        print(f"An error occurred: {str(e)}")


# Replace 'your_json_file.json' with the actual path to your JSON file
#file_path = 'D:/esprit study/2eme/QT/projects/try2/'+'jsonfile.json'
file_path = './'+'jsonfile.json'

# Call the function to read the JSON file and get the result as a dictionary
json_data = read_json_file(file_path)
#print(json_data)

json_type = json_data["type"][0]
if (json_type == "mail"):

    # Email configuration
    sender_email = json_data["user"][0]
    sender_password = json_data["pass"][0]
    recipient_email = json_data["receiver"][0]
    subject = json_data["subject"][0]
    message_body = json_data["message_body"][0]

    # Attach multiple files (e.g., PDFs) to the email (optional)
    attachment_paths = json_data["files"]

    # Call the function to send the email with multiple attachments
    send_email(sender_email, sender_password, recipient_email, subject, message_body, attachment_paths)

elif (json_type == "sms"):
    # sms configuration
    sms_sid = json_data["sid"][0]
    sms_token = json_data["token"][0]
    sms_to = json_data["to"][0]
    sms_from = json_data["from"][0]
    sms_body = json_data["body"][0]

    # Call the function to send the email with multiple attachments
    send_sms(sms_sid, sms_token, sms_to, sms_from, sms_body)
