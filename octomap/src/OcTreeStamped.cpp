// $Id$

/**
* OctoMap:
* A probabilistic, flexible, and compact 3D mapping library for robotic systems.
* @author K. M. Wurm, A. Hornung, University of Freiburg, Copyright (C) 2009-2011
* @see http://octomap.sourceforge.net/
* License: New BSD License
*/

/*
 * Copyright (c) 2009-2011, K. M. Wurm, A. Hornung, University of Freiburg
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of Freiburg nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "octomap/OcTreeStamped.h"

namespace octomap {

  OcTreeStamped::OcTreeStamped(double _resolution)
    : OccupancyOcTreeBase<OcTreeNodeStamped> (_resolution)  {
    itsRoot = new OcTreeNodeStamped();
    tree_size++;
  }

  unsigned int OcTreeStamped::getLastUpdateTime() {
    // this value is updated whenever inner nodes are 
    // updated using updateOccupancyChildren()
    return itsRoot->getTimestamp();
  }

  void OcTreeStamped::degradeOutdatedNodes(unsigned int time_thres) {
    unsigned int query_time = (unsigned int) time(NULL); 

    for(leaf_iterator it = this->begin_leafs(), end=this->end_leafs(); 
        it!= end; ++it) {
      if ( this->isNodeOccupied(*it) 
           && ((query_time - it->getTimestamp()) > time_thres) ) {
        integrateMissNoTime(&*it);
      }
    }
  }  

  void OcTreeStamped::updateNodeLogOdds(OcTreeNodeStamped* node, const float& update) const {
    OccupancyOcTreeBase<OcTreeNodeStamped>::updateNodeLogOdds(node, update);
    node->updateTimestamp();
  }

  void OcTreeStamped::integrateMissNoTime(OcTreeNodeStamped* node) const{
    OccupancyOcTreeBase<OcTreeNodeStamped>::updateNodeLogOdds(node, probMissLog);
  }

} // end namespace

