""" Utility to test any command using an UDP socket """
import socket
import sys

_ACK_STRING = b'OK'

UDP_IP = "192.168.188.100"
UDP_PORT = 20201
UDP_SOURCE_PORT = 20200
UDP_BUFFER_SIZE = 1024
UDP_TIMEOUT = 10

def _get_command(arg_list):
    return ' '.join((arg) for arg in arg_list)

def _output(target_ip, target_port, source_port, message):
    print("UDP target IP:", target_ip)
    print("UDP target port:", target_port)
    print("UDP source port:", source_port)
    print("Message:", message)

def _send(target_ip, target_port, source_port, message):
    _output(target_ip, target_port, source_port, message)
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.bind(('', source_port))
    udp_socket.settimeout(UDP_TIMEOUT)
    udp_socket.sendto(bytes(message, "utf-8"), (target_ip, target_port))
    return udp_socket

def _receive(udp_socket, buffer_size):
    data, _ = udp_socket.recvfrom(buffer_size)
    return data

def main():
    """ Parse the command that should be send, send it through a socket and
        receive the response """
    command = _get_command(sys.argv[1:])
    udp = _send(UDP_IP, UDP_PORT, UDP_SOURCE_PORT, command)
    response = _receive(udp, UDP_BUFFER_SIZE)
    if response == _ACK_STRING:
        print("Received Ack!")

if __name__ == '__main__':
    main()
