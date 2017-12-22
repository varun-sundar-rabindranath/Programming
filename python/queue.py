from Queue import PriorityQueue
from Queue import Queue

class A:
    priority = None 
    name     = "varun"

    def __init__(self, p_, name_):
        self.priority = p_
        self.name     = name_

q_queue = PriorityQueue()

q_queue._init([(0, "a"), (1, "b")])

q_queue.put((0, "a"))

q_queue.put((1, "c"))
q_queue.put((1, "b"))

q_queue.put((2, "g"))
q_queue.put((2, "d"))
q_queue.put((2, "e"))
q_queue.put((2, "f"))

q_queue.put((3, "h"))
q_queue.put((3, "i"))
q_queue.put((3, "j"))
q_queue.put((3, "k"))
q_queue.put((3, "l"))
q_queue.put((3, "m"))
q_queue.put((3, "n"))
q_queue.put((3, "o"))

print q_queue.queue
