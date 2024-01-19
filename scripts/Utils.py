import sys
import time
import requests
from fake_useragent import UserAgent


def DownloadFile(url, filepath):
    with open(filepath, 'wb') as f:
        userAgent = UserAgent()
        headers = {'User-Agent': userAgent.chrome}
        resp = requests.get(url, headers=headers, stream=True)
        total = resp.headers.get('content-length')

        if total is None:
            f.write(response.content)

        else:
            downloaded = 0
            total = int(total)
            startTime = time.time()

            for data in response.iter_content(chunk_size = max(int(total / 1000), 1024 * 1024)):
                downloaded += len(data)
                f.write(data)

                done = int(50 * downloaded / total)
                percentage = (downloaded / total) * 100
                elapsedTime = time.time() - startTime
                avgKBps = (downloaded / 1024) / elapsedTime
                avgSpeed = f'{avgKBps:.2f} KB/s'

                if (avgKBps > 1024):
                    avgMBps = avgKBps / 1024
                    avgSpeed = f'{avgMBps:.2f} MB/s'
                
                print(f'\r[{"█" * done}{"." * (50-done)}] {percentage:.2f}% ({avgSpeed})     ', flush=True)
    
    print('\n')
