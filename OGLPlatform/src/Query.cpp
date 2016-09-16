//
//  Query.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 13/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "Query.hpp"

namespace gs
{
    Query::Query(GLenum target) : target(target)
    {
        glGenQueries(1, &queryID);
    }
    
    Query::Query(const Query& q) : target(q.target), queryID(q.queryID)
    {}
    
    Query::~Query()
    {
        glDeleteQueries(1, &queryID);
        queryID = 0;
    }
    
    void Query::BeginQuery() const
    {
        glBeginQuery(target, queryID);
    }
    
    void Query::EndQuery() const
    {
        glEndQuery(target);
    }
}