import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from jinja2 import Environment, FileSystemLoader
import os
from config import settings

class EmailService:
    def __init__(self):
        template_dir = os.path.join(os.path.dirname(__file__), '..', 'templates')
        self.jinja_env = Environment(loader=FileSystemLoader(template_dir))

    def render_html(self, context: dict) -> str:
        """
        Renders the HTML email template with the provided context.
        """
        template = self.jinja_env.get_template('email_template.html')
        return template.render(**context)

    def send_email(self, subject: str, html_content: str):
        """
        Sends an HTML email via SMTP.
        """
        if not settings.SMTP_PASSWORD:
            print("WARNING: SMTP_PASSWORD not set. Skipping email send.")
            print(f"Subject: {subject}")
            return

        msg = MIMEMultipart('alternative')
        msg['Subject'] = subject
        msg['From'] = settings.FROM_EMAIL
        msg['To'] = settings.TO_EMAIL

        part2 = MIMEText(html_content, 'html')
        msg.attach(part2)

        with smtplib.SMTP(settings.SMTP_SERVER, settings.SMTP_PORT) as server:
            server.starttls()
            server.login(settings.SMTP_USER, settings.SMTP_PASSWORD)
            server.sendmail(settings.FROM_EMAIL, settings.TO_EMAIL, msg.as_string())
