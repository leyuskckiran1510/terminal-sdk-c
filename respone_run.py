import os
from dotenv import load_dotenv
import requests

load_dotenv()
token = os.environ["TOKEN"]

BASE_URL = "https://api.dev.terminal.shop"

api = lambda x: BASE_URL + x
headers = {
    "Authorization": f"Bearer {token}",
}

res = requests.get(api("/profile"), headers=headers)
