import socket

DESTINATION = "192.168.0.3"  # Replace with your destination hostname/IP
UDP_PORT = 5000
MESSAGE = "Test UDP packet from Raspberry Pi"

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(MESSAGE.encode('utf-8'), (DESTINATION, UDP_PORT))
print("Test message sent.")
