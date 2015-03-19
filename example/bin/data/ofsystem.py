from GestureAgents.Events import Event
import GestureAgents.Reactor as Reactor

class ofSystem(object):
    def __init__(self, sources=None):
        self.new_agents = {}
        self.recognizers = []
        self.sources = sources or []

    def newAgent(self, recognizer):
        if recognizer not in self.new_agents:
            if recognizer in self.sources:
                self.new_agents[recognizer] = recognizer.newAgent
            else:
                self.new_agents[recognizer] = Event()
                self.recognizers.append(recognizer(self))
        return self.new_agents[recognizer]

    def update(self):
        Reactor.run_all_now()

def registerNewAgent(system,recognizer,callback):
    from GestureAgents.AppRecognizer import AppRecognizer
    AppRecognizer(system, recognizer).newAgent.register(
        lambda s,x: callback.call(x), callback)

def registerEvent(event,callback):
    event.register(lambda s,x: callback.call(x), callback)
