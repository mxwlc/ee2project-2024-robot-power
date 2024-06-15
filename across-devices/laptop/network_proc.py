import multiprocessing as mp
import socket as s
import ctypes
from time import perf_counter_ns

class SendInstructions():
    ARDUINO_SET_VAR = b"\x00"

    ARDUINO_GET_VAR = b"\x01"
    
    PI_SET_DIR = b"\x02"
    PI_DIR_N = b"\x00"
    PI_DIR_F = b"\x01"
    PI_DIR_FR = b"\x02"
    PI_DIR_R = b"\x03"
    PI_DIR_BR = b"\x04"
    PI_DIR_B = b"\x05"
    PI_DIR_BL = b"\x06"
    PI_DIR_L = b"\x07"
    PI_DIR_FL = b"\x08"
    
    PI_SET_DRIVE_MODE = b"\x03"
    PI_DRIVE_MODE_CAMERA = b"\x00"
    PI_DRIVE_MODE_REMOTE = b"\x01"

class RecvTypes():
    CV = b"\x00"
    CV_SIZE = 640 * 480 * 3

    VAR = b"\x01"

def __float_to_bytes(value: float) -> bytes:
    c_float = ctypes.c_float(value)
    return bytes(c_float)

__IP = "192.168.0.1"
__PORT = 9876

def __network_send_proc(socket: s.socket, send_q: mp.Queue, running: mp.Value) -> None:
    while running.value:
        # Don't want an indefinitely stalling send_q.get(), makes join() impossible
        try:
            send_data = send_q.get_nowait()
        except: continue
        # No type checking, assuming main process sends correct instructions
        final_send = send_data[0]
        if send_data[0] == SendInstructions.ARDUINO_SET_VAR:
            final_send += send_data[1]
            final_send += __float_to_bytes(send_data[2])
        elif send_data[0] == SendInstructions.ARDUINO_GET_VAR:
            final_send += send_data[1]
        elif send_data[0] == SendInstructions.PI_SET_DIR:
            final_send += send_data[1]
        elif send_data[0] == SendInstructions.PI_SET_DRIVE_MODE:
            final_send += send_data[1]

        try:
            socket.sendall(final_send)
        except:
            pass
    try: socket.close()
    except: pass

__LARGE_INS_TIMEOUT_NS = 1_000_000_000
def __network_recv_proc(socket: s.socket, recv_q: mp.Queue, running: mp.Value) -> None:
    recv_buffer = b""
    last_recv_time = 0
    check_for_timeout = False

    while running.value:
        try: recv_data = socket.recv(8192)
        except: continue
        # Clears buffer after timeout. Intended for if erroneous first byte is that of instruction.
        recv_time = perf_counter_ns()
        if check_for_timeout:
            if (recv_time - last_recv_time) > __LARGE_INS_TIMEOUT_NS:
                recv_buffer = b""
        last_recv_time = recv_time

        recv_buffer += recv_data

        if recv_buffer[0] == RecvTypes.CV[0]:
            if len(recv_buffer) < (RecvTypes.CV_SIZE + 1):
                check_for_timeout = True
                continue
            check_for_timeout = False
            recv_q.put((RecvTypes.CV, recv_buffer[1 : 1+RecvTypes.CV_SIZE]))
            recv_buffer = recv_buffer[1+RecvTypes.CV_SIZE:]
        elif recv_buffer[0] == RecvTypes.VAR[0]:
            if len(recv_buffer) < 6:
                check_for_timeout = True
                continue
            recv_q.put((RecvTypes.VAR, recv_buffer[1], float(ctypes.c_float.from_buffer_copy(recv_buffer[2:6]).value)))
            recv_buffer = recv_buffer[6:]
        else:
            recv_buffer = b""

    try: socket.close()
    except: pass

def run_network_proc(send_q: mp.Queue, recv_q: mp.Queue) -> tuple[mp.Value, mp.Process, mp.Process]:
    socket = s.socket(s.AF_INET, s.SOCK_STREAM)
    socket.connect((__IP, __PORT))
    socket.settimeout(1)
    running = mp.Value(ctypes.c_bool, True)
    send_proc = mp.Process(target=__network_send_proc, args=(socket, send_q, running))
    recv_proc = mp.Process(target=__network_recv_proc, args=(socket, recv_q, running))
    send_proc.start()
    recv_proc.start()
    return (running, send_proc, recv_proc)

def stop_network_proc(arg: tuple[mp.Value, mp.Process, mp.Process]) -> None:
    arg[0].value = False
    arg[1].terminate()
    arg[2].terminate()


def create_queues() -> tuple[mp.Queue, mp.Queue]:
    return (mp.Queue(), mp.Queue())


def send(send_q: mp.Queue, instruction: tuple) -> None:
    """
    Sends data to the RPi. Pass the instruction as any of the following tuples:\n
    [1] `(SendInstructions.ARDUINO_SET_VAR, index: bytes, value: float)`,\n
    e.g. `(SendInstructions.ARDUINO_SET_VAR, b"\\x02", 0.3)` sets variable 2 to 0.3;\n
    [2] `(SendInstructions.ARDUINO_GET_VAR, index: bytes)`,\n
    e.g. `(SendInstructions.ARDUINO_GET_VAR, b"\\x03")` gets variable 3 \
    (Note: this isn't returned, it acts as a request to get an Arduino variable \
    that isn't already being sent) \n
    [3] `(SendInstructions.PI_SET_DIR, dir: bytes)`,\n
    e.g. `(SendInstructions.PI_SET_DIR, SendInstructions.PI_DIR_FR)` sets the desired direction to forwards and right. \n
    [4] `(SendInstructions.PI_SET_DRIVE_MODE, mode: bytes)`,\n
    e.g. `(SendInstructions.PI_SET_DRIVE_MODE, SendInstructions.PI_DRIVE_MODE_CAMERA)` sets the desired drive mode to camera-controlled.
    """
    send_q.put(instruction)

def recv(recv_q: mp.Queue) -> tuple:
    """
    Receives data from RPi. Returns an instruction if received as one of the following tuples, otherwise `None`:\n
    [1] `(RecvTypes.CV, data: bytes)`
    """
    try: recv_data = recv_q.get_nowait()
    except: return

    return recv_data


if __name__ == "__main__":
    # USAGE INSTRUCTIONS:

    # CALL THIS AT THE START OF YOUR CODE.
    send_q, recv_q = create_queues()
    network_proc = run_network_proc(send_q, recv_q)

    # CAN CALL SEND AND RECV HERE
    # e.g. send(send_q, SendInstructions.ARDUINO_SET_VAR, b"\x00", 0.2)
    # e.g. if recv(recv_q) != None: print("Received data!")

    # CALL THIS WHEN QUITTING. 
    stop_network_proc(network_proc)