# -*- coding: utf-8 -*-

# we feed mouse events here from OF

from GestureAgents.Events import Event
from GestureAgents.Agent import Agent

class TuioCursorEvents:
    newAgent = Event()


class CursorAgent(Agent):
    eventnames = ('newCursor', 'updateCursor', 'removeCursor')

