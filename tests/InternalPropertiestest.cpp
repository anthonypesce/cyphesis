// Cyphesis Online RPG Server and AI Engine
// Copyright (C) 2009 Alistair Riddoch
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

// $Id$

#ifdef NDEBUG
#undef NDEBUG
#endif
#ifndef DEBUG
#define DEBUG
#endif

#include "PropertyCoverage.h"

#include "rulesets/InternalProperties.h"

void test_SetupProperty()
{
    SetupProperty * ap = new SetupProperty;

    PropertyCoverage pc(ap);

    pc.testDataAppend(0);

    pc.basicCoverage();
}

void test_TickProperty()
{
    TickProperty * ap = new TickProperty;

    PropertyCoverage pc(ap);

    pc.testDataAppend(0);

    pc.basicCoverage();
}

void test_SimpleProperty()
{
    SimpleProperty * ap = new SimpleProperty;

    PropertyCoverage pc(ap);

    pc.testDataAppend(0);

    pc.basicCoverage();
}

int main()
{
    test_SetupProperty();
    test_TickProperty();
    test_SimpleProperty();
    return 0;
}

// stubs

#include "TestWorld.h"

void TestWorld::message(const Operation & op, Entity & ent)
{
}

Entity * TestWorld::addNewEntity(const std::string &,
                                 const Atlas::Objects::Entity::RootEntity &)
{
    return 0;
}

namespace Atlas { namespace Objects { namespace Operation {
int TICK_NO = -1;
int SETUP_NO = -1;
} } }
