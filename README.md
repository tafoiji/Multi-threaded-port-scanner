# Multi-threaded-port-scanner

This is the application of multi-threaded port scanner. It provides ability to choose count of checking threads (up to 1000), and check all ports you need (or just all ports). Using 1000 threads you can check all 65536 ports just in less than a minute. You can check also TCP or/and UDP ports, using IPv4 or IPv6 address.

## Installation
You can download and execute application by this links in releases:

![image](https://github.com/user-attachments/assets/e16c1be4-e52f-4706-bba8-0750d498b612)


*************

Or use git clone 

```bash
git clone git@github.com:tafoiji/Multi-threaded-port-scanner.git
```

Open Scanner.pro in qt or use shell commands in Windows:
```bash
qmake
mingw32-make
windeployqt release/Scanner.exe
"release/Scanner.exe"
```




## Usage

#### Important

After execution you can see first window. You can choose count of threads(**1-10000**) to scan ports and enter a valid **IPv4** or **IPv6** address. Press Submit button. After that application will create this count of threads and will be waiting for you filling out port information. Application will tell you if address is not valid. Don't set big amount of threads on weak devices. 

![image](https://github.com/user-attachments/assets/52ffe0fb-68bc-43b2-88c3-91fe3624fa8f)

If all information is valid application will request information about ports: 
* Type of ports you want to scan (**TCP, UDP, TCP/UDP**),
* A message for your UDP endpoint (you need to understand, by which protocol, endpoint you want to scan, is working, and send message on which you will get response), if UDP selected,
* Choosing scanning options:
  * List of popular ports,
  * Your list of ports (you will need to enter them separating by space),
  * All ports (**0-65535**).
After that push **START SCANNING** button.

![image](https://github.com/user-attachments/assets/68ac4415-8a27-4301-a571-4580ef50dd4c)

Application start to scan ports.
You can stop scanning at any moment by pushing on **Stop** button and change port information. You can safely exit application at any moment too.

#### Results

![image](https://github.com/user-attachments/assets/c4788f1c-91a7-4b0d-9b39-bb0f183b6afc)



