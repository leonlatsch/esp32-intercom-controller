import os
import requests
import sys

if len(sys.argv) < 3:
    print("[!] Provide ip and secret as parameters")
    exit()

ip = sys.argv[1]
secret = sys.argv[2]
url = f"http://{ip}/update"

# Compilation

filename = "firmware.bin"
filepath = ".pio/build/denky32/firmware.bin"

os.system("pio run")

if not os.path.isfile(filepath):
    print(f"[!] Error: Could not find {filename}")
    exit(1)

# Upload
    
print(f"[*] Uploading {filename} to {ip}")

payload={}
files=[(filename, (filename, open(filepath,'rb'),'application/octet-stream'))]
headers = {
  'secret': secret
}

response = requests.request("POST", url, headers=headers, data=payload, files=files)
print(f"[*] {response.text}")
