import network_proc as net
from time import sleep


if __name__ == "__main__":

    print("START")

    send_q, recv_q = net.create_queues()
    net_args = net.run_network_proc(send_q, recv_q)

    print("CONNECTED")

    ind = int(input("Input target index: ")).to_bytes(1, "little")

    net.send(send_q, (net.SendInstructions.ARDUINO_GET_VAR, ind))
    while True:
        recv = net.recv(recv_q)
        if recv == None:
            sleep(1/10_000)
            continue

        print(f"Cur val of index {recv[1]} is {recv[2]}")
        break

    new_val = float(input("Enter new value: "))

    net.send(send_q, (net.SendInstructions.ARDUINO_SET_VAR, ind, new_val))
    sleep(1/1_00)

    net.stop_network_proc(net_args)
