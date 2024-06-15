import tkinter as tk
import network_proc as net
from time import sleep
from PIL import Image, ImageTk
import multiprocessing as mp # Only used for function type declarations

class UIWindow(tk.Tk):
    __FONT = ("Helvetica", 24)
    __pressed = [False for i in range(4)]
    __target_keys = { "Up" : 0,
                      "Down" : 1,
                      "Left" : 2,
                      "Right" : 3
                     }
    __DIRS_TO_PI_DIRS = { "" : net.SendInstructions.PI_DIR_N,
                          "^" : net.SendInstructions.PI_DIR_F,
                          "^>" : net.SendInstructions.PI_DIR_FR,
                          ">" : net.SendInstructions.PI_DIR_R,
                          "v>" : net.SendInstructions.PI_DIR_BR,
                          "v" : net.SendInstructions.PI_DIR_B,
                          "<v" : net.SendInstructions.PI_DIR_BL,
                          "<" : net.SendInstructions.PI_DIR_L,
                          "<^" : net.SendInstructions.PI_DIR_FL
                         }
    __MODES = ("CAMERA", "REMOTE")
    __MODE_TO_PI_MODE = { __MODES[0] : net.SendInstructions.PI_DRIVE_MODE_CAMERA,
                          __MODES[1] : net.SendInstructions.PI_DRIVE_MODE_REMOTE,
                        }
    __cur_mode = __MODES[0]
    __RECV_FREQ = 50
    __BATTERY_FREQ = 1
    
    __BATTERY_VAR = b"\x00" # Unknown yet, set when known
    def __init__(self, send_q: mp.Queue, recv_q: mp.Queue, net_args: tuple):
        super().__init__()
        self.title("Backup UI (RAYMOND WHERE IS YOURS?!?!?!)")
        self.geometry("650x545")
        self.resizable(False, False)
        # self.configure(bg="#f589c1") Thank you Sophie, but we can't make everything pink -_-

        self.__send_q = send_q
        self.__recv_q = recv_q
        self.__net_args = net_args

        self.__video_feed_canvas = tk.Canvas(self, width=640, height=480, bg="#0000aa")
        self.__video_feed_canvas.place(anchor="nw", x=5, y=5, w=640, h=480)
        self.__video_feed_canvas_imageid = None

        self.__direction_label = tk.Label(self, text="", font=self.__FONT, justify="center")
        self.__direction_label.place(anchor="nw", x=5, y=490, w=50, h=50)

        self.__battery_label = tk.Label(self, text="Battery: -.--", font=self.__FONT, justify="left")
        self.__battery_label.place(anchor="nw", x=55, y=490, w=200, h=50)

        self.__mode_label = tk.Label(self, text="Mode: CAMERA", font=self.__FONT, justify="left")
        self.__mode_label.place(anchor="nw", x=260, y=490, w=250, h=50)

        self.__bind_keys()
        self.__run_recv_proc()
        self.__run_battery_proc()

        self.protocol("WM_DELETE_WINDOW", self.__on_quit)




    def __update_direction(self):
        targ_dir = ""
        # FORWARD
        if self.__pressed[0] and not self.__pressed[1]:
            targ_dir = "^"
        # BACKWARDS
        elif self.__pressed[1] and not self.__pressed[0]:
            targ_dir = "v"

        # LEFT
        if self.__pressed[2] and not self.__pressed[3]:
            targ_dir = "<" + targ_dir
        # RIGHT
        elif self.__pressed[3] and not self.__pressed[2]:
            targ_dir += ">"
        
        self.__direction_label.config(text=targ_dir)
        net.send(self.__send_q, (net.SendInstructions.PI_SET_DIR, self.__DIRS_TO_PI_DIRS[targ_dir]))

    def __change_mode(self, *args):
        cur_ind = self.__MODES.index(self.__cur_mode)
        if cur_ind < len(self.__MODES) - 1: cur_ind += 1
        else: cur_ind = 0
        self.__cur_mode = self.__MODES[cur_ind]

        self.__mode_label.config(text="Mode: " + self.__cur_mode)
        net.send(self.__send_q, (net.SendInstructions.PI_SET_DRIVE_MODE, self.__MODE_TO_PI_MODE[self.__cur_mode]))

    def __update_image(self):
        self.__video_feed_image = ImageTk.PhotoImage(Image.frombytes("RGB", (640, 480), self.__video_feed_image_bytes, "raw"))
        if self.__video_feed_canvas_imageid != None:
            self.__video_feed_canvas.delete(self.__video_feed_canvas_imageid)
        self.__video_feed_canvas_imageid = self.__video_feed_canvas.create_image(0, 0, anchor="nw", image=self.__video_feed_image)



    def __bind_keys(self):
        for cur_key in ("Up", "Down", "Left", "Right"):
            self.bind(f"<KeyPress-{cur_key}>", self.__on_dir_key_press)
            self.bind(f"<KeyRelease-{cur_key}>", self.__on_dir_key_release)
        self.bind("<KeyPress-Delete>", self.__change_mode)

    def __on_dir_key_press(self, event):
        try: self.__pressed[self.__target_keys[event.keysym]] = True
        except: return
        self.__update_direction()
    
    def __on_dir_key_release(self, event):
        try: self.__pressed[self.__target_keys[event.keysym]] = False
        except: return
        self.__update_direction()
    


    def __run_recv_proc(self):
        recv = net.recv(self.__recv_q)
        if recv != None:
            if recv[0] == net.RecvTypes.CV:
                self.__video_feed_image_bytes = recv[1]
                self.__update_image()
            elif recv[0] == net.RecvTypes.VAR:
                if recv[1] == self.__BATTERY_VAR[0]:
                    self.__battery_label.config(text=f"Battery: {recv[2]:3.2f}")
        self.after(1_000 // self.__RECV_FREQ, self.__run_recv_proc)
    
    def __run_battery_proc(self):
        net.send(self.__send_q, (net.SendInstructions.ARDUINO_GET_VAR, self.__BATTERY_VAR))
        self.after(1_000 // self.__BATTERY_FREQ, self.__run_battery_proc)
    
    def __on_quit(self):
        net.stop_network_proc(self.__net_args)
        self.destroy()

import ctypes

if __name__ == "__main__":
    send_q, recv_q = net.create_queues()
    net_args = net.run_network_proc(send_q, recv_q)

    window = UIWindow(send_q, recv_q, net_args)
    window.mainloop()