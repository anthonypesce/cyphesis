#This file is distributed under the terms of the GNU General Public license.
#Copyright (C) 2011 Jekin Trivedi <jekintrivedi@gmail.com> (See the file COPYING for details).

from atlas import *
from physics import *
from physics import Quaternion
from physics import Point3D
from physics import Vector3D

import server

class Earthwall(server.Task):
    """A task for creating a Earthwall by consuming one part boulder , 2 part earth and 3 part sand """
    
    materials = ["pile"]
    def walls_operation(self, op):
        """ Op handler for walls op which activates this task """

        if len(op) < 1:
            sys.stderr.write("Earthwall task has no target in walls op")

        # FIXME Use weak references, once we have them
        self.target = op[0].id
        self.tool = op.to

        self.pos = Point3D(op[0].pos)
        
    def tick_operation(self, op):

        """ Op handler for regular tick op """
        target=server.world.get_object(self.target)
        if not target:
            # print "Target is no more"
            self.irrelevant()
            return

        self.rate = 0.5 / 0.75
        self.progress += 1

        if not target:
            print "Target is no more"
            self.irrelevant()
            return

        res=Oplist()

        if self.progress < 1:
            # print "Not done yet"
            return self.next_tick(0.75)

        self.progress = 0


        raw_materials = []

        for item in self.character.contains:
            if item.type[0] in self.materials:
                raw_materials.append(item.id)
                break
        else:
            print "No materials in inventory"
            self.irrelevant()
            return

        chunk_loc = target.location.copy()
        chunk_loc.coordinates = target.location.coordinates
        chunk_loc.orientation = target.location.orientation
        set = Operation("set", Entity(raw_materials.pop(), status = -1), to = target)
        res.append(set)

        set = Operation("set", Entity(target.id, status = -1), to = target)
        res.append(set)

        create=Operation("create", Entity(name = "wall", type = "wall", location = chunk_loc), to = target)
        res.append(create)
        self.progress = 1
        self.irrelevant()

        return res