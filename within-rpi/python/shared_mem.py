from multiprocessing import shared_memory

SHARED_MEM_NAME = "/shared_mem"

class SharedMemory():
    def __init__(self, name, size, host = True):
        if name == None: name = SHARED_MEM_NAME
        self.host = host
        self.shm = shared_memory.SharedMemory(name=name, size=size, create=host)
    
    def __del__(self):
        self.shm.close()
        if self.host:
            self.shm.unlink()

    def getBuffer(self):
        return self.shm.buf