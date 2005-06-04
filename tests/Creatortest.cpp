// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).
// Copyright (C) 2003 Alistair Riddoch

#include "IGEntityExerciser.h"
#include "allOperations.h"

#include "rulesets/Creator.h"
#include "rulesets/Python_API.h"

#include <cassert>

using Atlas::Message::MapType;
using Atlas::Message::ListType;

class TestWorld : public BaseWorld {
  public:
    explicit TestWorld(Entity & gw) : BaseWorld(gw) { }

    virtual bool idle(int, int) { return false; }
    virtual Entity * addEntity(Entity * obj, bool setup = true) { return 0; }
    virtual Entity * addNewEntity(const std::string &, const MapType &) {
        return 0;
    }
    virtual void message(Operation & op, Entity & ent) { }
    virtual Entity * findByName(const std::string & name) { return 0; }
    virtual Entity * findByType(const std::string & type) { return 0; }
    virtual float constrainHeight(Entity*, const Point3D&,
                                  const std::string&) { return 0.f; }
    virtual void addPerceptive(const std::string &) { }
};

int main(int argc, char ** argv)
{
    loadConfig(argc, argv);

    init_python_api();

    Creator e("testId");
    Entity world("testWorldId");
    TestWorld tw(world);

    e.m_world = &tw;

    IGEntityExerciser<Creator> ee(e);

    // Throw an op of every type at the entity
    ee.runOperations();

    // Subscribe the entity to every class of op
    std::set<std::string> opNames;
    ee.addAllOperations(opNames);
    ee.subscribeOperations(opNames);

    // Throw an op of every type at the entity again now it is subscribed
    ee.runOperations();

    std::set<std::string> attrNames;
    attrNames.insert("status");
    attrNames.insert("id");
    attrNames.insert("name");
    attrNames.insert("mass");
    attrNames.insert("bbox");
    attrNames.insert("contains");
    attrNames.insert("drunkness");
    attrNames.insert("sex");

    // Make sure we have all the default attributes
    assert(ee.checkAttributes(attrNames));

    attrNames.insert("test_int");
    attrNames.insert("test_float");
    attrNames.insert("test_list_string");
    attrNames.insert("test_list_int");
    attrNames.insert("test_list_float");
    attrNames.insert("test_map_string");
    attrNames.insert("test_map_int");
    attrNames.insert("test_map_float");

    // Make sure we don't have the test attributes yet
    assert(!ee.checkAttributes(attrNames));

    // Add the test attributes
    e.set("test_int", 1);
    e.set("test_float", 1.f);
    e.set("test_list_string", "test_value");
    e.set("test_list_int", ListType(1, 1));
    e.set("test_list_float", ListType(1, 1.f));
    e.set("test_map_string", ListType(1, "test_value"));
    MapType test_map;
    test_map["test_key"] = 1;
    e.set("test_map_int", test_map);
    test_map["test_key"] = 1.f;
    e.set("test_map_float", test_map);
    test_map["test_key"] = "test_value";
    e.set("test_map_string", test_map);
    
    // Make sure we have the test attributes now
    assert(ee.checkAttributes(attrNames));

    MapType entityAsAtlas;

    // Dump a representation of the entity into an Atlas Message
    e.addToMessage(entityAsAtlas);

    // Make sure we got at least some of it
    assert(entityAsAtlas.size() > 14);

    // Read the contents of the Atlas Message back in
    e.merge(entityAsAtlas);

    return 0;
}
