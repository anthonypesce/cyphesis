// Cyphesis Online RPG Server and AI Engine
// Copyright (C) 2012 Alistair Riddoch
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

#include "server/ExternalProperty.h"

#include "common/Router.h"

#include <Atlas/Objects/RootEntity.h>

#include <cassert>

using Atlas::Message::Element;
using Atlas::Message::MapType;
using Atlas::Objects::Entity::RootEntity;

class TestMind : public Router
{
  public:
    TestMind() : Router("", -1) { }

    virtual void externalOperation(const Operation &) { }
    virtual void operation(const Operation &, OpVector &) { }
};

int main()
{
    {
        Router * e = 0;
        ExternalProperty * ep = new ExternalProperty(e);
        delete ep;
    }
    {
        Router * e = 0;
        ExternalProperty * ep = new ExternalProperty(e);

        Element elem;
        assert(elem.isNone());
        int res = ep->get(elem);
        assert(res == 0);
        assert(elem.isInt());
        assert(elem.Int() == 0);

        delete ep;
    }
    {
        Router * e = new TestMind;
        ExternalProperty * ep = new ExternalProperty(e);

        Element elem;
        assert(elem.isNone());
        int res = ep->get(elem);
        assert(res == 0);
        assert(elem.isInt());
        assert(elem.Int() == 1);

        delete ep;
        delete e;
    }
    {
        Router * e = 0;
        ExternalProperty * ep = new ExternalProperty(e);

        MapType map;
        assert(map.empty());
        ep->add("external", map);
        MapType::const_iterator I = map.find("external");
        assert(I != map.end());
        assert(I->second.isInt());
        assert(I->second.Int() == 0);

        delete ep;
    }
    {
        Router * e = new TestMind;
        ExternalProperty * ep = new ExternalProperty(e);

        MapType map;
        assert(map.empty());
        ep->add("external", map);
        MapType::const_iterator I = map.find("external");
        assert(I != map.end());
        assert(I->second.isInt());
        assert(I->second.Int() == 1);

        delete ep;
        delete e;
    }
    {
        Router * e = 0;
        ExternalProperty * ep = new ExternalProperty(e);

        RootEntity ent;
        ep->add("external", ent);
        Element elem = ent->getAttr("external");
        assert(elem.isInt());
        assert(elem.Int() == 0);

        delete ep;
    }
    {
        Router * e = new TestMind;
        ExternalProperty * ep = new ExternalProperty(e);

        RootEntity ent;
        ep->add("external", ent);
        Element elem = ent->getAttr("external");
        assert(elem.isInt());
        assert(elem.Int() == 1);

        delete ep;
    }
    {
        Router * e = 0;
        ExternalProperty * ep = new ExternalProperty(e);

        const Element elem;
        assert(elem.isNone());
        ep->set(elem);

        delete ep;
    }
    return 0;
}

// stubs

Router::Router(const std::string & id, long intId) : m_id(id),
                                                             m_intId(intId)
{
}

Router::~Router()
{
}

void Router::addToMessage(MapType & omap) const
{
}

void Router::addToEntity(const Atlas::Objects::Entity::RootEntity & ent) const
{
}
