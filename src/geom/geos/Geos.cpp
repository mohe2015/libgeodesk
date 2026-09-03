// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#ifdef GEODESK_WITH_GEOS
#include <geodesk/geom/Distance.h>
#include <geodesk/geom/geos/Geos.h>
#include <geos/operation/distance/DistanceOp.h>

namespace geodesk {

bool Geos::centroid(geos::geom::GeometryFactory* context,
    const geos::geom::Geometry::Ptr geom, Coordinate* centroid)
{
    std::unique_ptr<geos::geom::Point> c = geom->getCentroid();
    if (!c) return false;
    *centroid = Coordinate(c->getX(), c->getY());
    return true;
}

double Geos::distanceMeters(geos::geom::GeometryFactory* context,
    const geos::geom::Geometry::Ptr geom1, const geos::geom::Geometry::Ptr geom2)
{
    return geos::operation::distance::DistanceOp::distance(*geom1, *geom2);
}



} // namespace geodesk

#endif // GEODESK_WITH_GEOS

