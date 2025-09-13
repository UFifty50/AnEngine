import sys
import time

import enum
from enum import IntFlag

import requests
from fake_useragent import UserAgent


def DownloadFile(url, filepath):
    with open(filepath, 'wb') as f:
        userAgent = UserAgent()
        headers = {'User-Agent': userAgent.chrome}
        resp = requests.get(url, headers=headers, stream=True)
        total = resp.headers.get('content-length')

        if total is None:
            f.write(resp.content)

        else:
            downloaded = 0
            total = int(total)
            startTime = time.time()

            for data in resp.iter_content(chunk_size = max(int(total / 1000), 1024 * 1024)):
                downloaded += len(data)
                f.write(data)

                done = int(50 * downloaded / total)
                percentage = (downloaded / total) * 100
                elapsedTime = time.time() - startTime
                avgKBps = (downloaded / 1024) / elapsedTime
                avgSpeed = f'{avgKBps:.2f} KB/s'

                if avgKBps > 1024:
                    avgMBps = avgKBps / 1024
                    avgSpeed = f'{avgMBps:.2f} MB/s'
                
                print(f'\r[{"█" * done}{"." * (50-done)}] {percentage:.2f}% ({avgSpeed})     ', flush=True)
    
    print('\n')

class Platform(IntFlag):
    WINDOWS = 1
    LINUX = 2

class UnsupportedPlatformError(Exception):
    def __init__(self, message, platform: Platform):
        super().__init__(message)
        self.message = message
        self.platform = platform

    def __str__(self):
        platformSupport = "Currently supported platforms: ("
        for p in Platform:
             if self.platform & p:
                platformSupport += f"{p.name} "
        platformSupport = platformSupport.strip() + ")"

        return f"{self.message}\n" + platformSupport
