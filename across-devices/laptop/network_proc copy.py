import multiprocessing as mp
import socket as s
import ctypes
from time import perf_counter_ns

class SendInstructions():
    ARDUINO_SET_VAR = b"\x00"
    ARDUINO_GET_VAR = b"\x01"

class RecvTypes():
    CV = b"\x00"
    CV_SIZE = 4_000 * 3_000 * 3

def __float_to_bytes(value: float) -> bytes:
    c_float = ctypes.c_float(value)
    return bytes(c_float)

__IP = "127.0.0.1" #"192.168.0.1"
__PORT = 9876
#__RECV_FREQ = 50

"""def network_proc(send_q: mp.Queue, recv_q: mp.Queue):
    conn = s.socket(s.AF_INET, s.SOCK_STREAM)
    conn.connect((__IP, __PORT))
    recv_data = b""
    while True:
        # Send
        if not send_q.empty():
            ins = send_q.get()
            conn.settimeout(1)
            conn.send(ins[0])
            if (ins[0] == Instructions.SET_MOTOR_L):
                conn.send(float_to_bytes(ins[1]))
            elif (ins[0] == Instructions.SET_MOTOR_R):
                conn.send(float_to_bytes(ins[1]))
            elif (ins[0] == Instructions.SET_VAR):
                conn.send(ins[1])
                conn.send(float_to_bytes(ins[2]))
            elif (ins[0] == Instructions.GET_VAR):
                conn.send(ins[1])
        # Recv
        conn.settimeout(1 / __RECV_FREQ)
        try:
            recv_data += conn.recv(8192)
            if data[0] == Recvs.CV:
                conn.settimeout(1)
                while len(data) < 1 + Recvs.CV_SIZE:
                    data += conn.recv(8192)
                recv_q.put((data[0], data[1:]))
        except:
            recv_data = b" """

def __network_send_proc(socket: s.socket, send_q: mp.Queue, running: mp.Value) -> None:
    while running.value:
        # No type checking, assuming main process sends correct instructions
        print("IN SEND LOOP")
        try:
            send_data = send_q.get_nowait()
        except: continue
        final_send = send_data[0]
        if send_data[0] == RecvTypes.CV:
            final_send += send_data[1]
        try:
            print("Sending", len(final_send))
            socket.sendall(final_send)
        except:
            pass
    print("SEND DONE 1")
    try: socket.close()
    except: pass
    print("SEND DONE 2")

__LARGE_INS_TIMEOUT_NS = 1_000_000_000
def __network_recv_proc(socket: s.socket, recv_q: mp.Queue, running: mp.Value) -> None:
    recv_buffer = b""
    last_recv_time = 0
    check_for_timeout = False
    while running.value:
        try:
            recv_data = socket.recv(8192)
        except: continue
        # Clears buffer after timeout. Intended for if erroneous byte is that of instruction.
        recv_time = perf_counter_ns()
        if check_for_timeout:
            if (recv_time - last_recv_time) > __LARGE_INS_TIMEOUT_NS: recv_buffer = b""
        last_recv_time = recv_time

        recv_buffer += recv_data

        if recv_buffer[0] == RecvTypes.CV:
            if len(recv_buffer) < (RecvTypes.CV_SIZE + 1):
                check_for_timeout = True
                continue
            check_for_timeout = False
            recv_q.put((RecvTypes.CV, recv_buffer[1 : 1+RecvTypes.CV_SIZE]))
            recv_buffer = recv_buffer[1+RecvTypes.CV_SIZE:]

    try: socket.close()
    except: pass

def run_network_proc(send_q: mp.Queue, recv_q: mp.Queue) -> tuple[mp.Value, mp.Process, mp.Process]:
    socket = s.socket(s.AF_INET, s.SOCK_STREAM)
    socket.bind((__IP, __PORT))
    socket.listen(1)
    conn, addr = socket.accept()
    running = mp.Value(ctypes.c_bool, True)
    conn.settimeout(1)
    send_proc = mp.Process(target=__network_send_proc, args=(conn, send_q, running))
    recv_proc = mp.Process(target=__network_recv_proc, args=(conn, recv_q, running))
    send_proc.start()
    recv_proc.start()
    return (running, send_proc, recv_proc)

def stop_network_proc(arg: tuple) -> None:
    arg[0].value = False
    print("A")
    arg[1].join()
    print("B")
    arg[2].join()
    print("C")


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
    that isn't already being sent)
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
    send_q, recv_q = create_queues()
    r = run_network_proc(send_q, recv_q)
    while True:
        a = input("Send/Recv/Quit\n").upper()
        if a == "S":
            send(send_q, (RecvTypes.CV, bytes(RecvTypes.CV_SIZE)))
        if a == "R":
            print(recv(recv_q))
        if a == "Q":
            stop_network_proc(r)
            quit()