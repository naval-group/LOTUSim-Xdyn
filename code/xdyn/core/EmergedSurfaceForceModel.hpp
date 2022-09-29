/*
 * EmergedSurfaceForceModel.hpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#ifndef EMERGEDSURFACEFORCEMODEL_HPP_
#define EMERGEDSURFACEFORCEMODEL_HPP_

#include "SurfaceForceModel.hpp"

/** \brief This class implemented SurfaceForceModel's begin & end methods
 */
class EmergedSurfaceForceModel : public SurfaceForceModel
{
    public:
        EmergedSurfaceForceModel(const std::string& name, const std::string& body_name, const EnvironmentAndFrames& env);
        virtual ~EmergedSurfaceForceModel();

    private:
        EmergedSurfaceForceModel();
        FacetIterator begin(const MeshIntersectorPtr& intersector) const;
        FacetIterator end(const MeshIntersectorPtr& intersector) const;
};

#endif /* EMERGEDSURFACEFORCEMODEL_HPP_ */
