// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <clarisma/util/BufferWriter.h>
#ifdef GEODESK_WITH_GEOS
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#endif
#include <geodesk/feature/WayPtr.h>
#include <geodesk/feature/RelationPtr.h>
#include <geodesk/geom/Coordinate.h>
#include <functional>

namespace geodesk {

class Polygonizer;

///
/// \cond lowlevel
///
class GeometryWriter : public clarisma::BufferWriter
{
public:
	explicit GeometryWriter(clarisma::Buffer* buf) : BufferWriter(buf) {}
	
	void precision(int precision) 
	{ 
		assert(precision >= 0);
		assert(precision <= 15);
		precision_ = precision;
	}

protected:
	void writeCoordinate(Coordinate c);
	
	template<typename Iter>
	void writeCoordinates(Iter& iter)
	{
		bool isFirst = true;
		writeByte(coordGroupStartChar_);
		for (int count = iter.coordinatesRemaining(); count > 0; count--)
		{
			Coordinate c = iter.next();
			if (!isFirst) writeByte(',');  // TODO: always comma for all formats?
			isFirst = false;
			writeCoordinate(c);
		}
		writeByte(coordGroupEndChar_);
	}

	// ==== GEOS Geometries ====

	void writeCoordinateSegment(bool isFirst, const Coordinate* coords, size_t count);
	#ifdef GEODESK_WITH_GEOS
	void writeCoordSequence(geos::geom::GeometryFactory* context, const GEOSCoordSequence* coords);
	void writePointCoordinates(geos::geom::GeometryFactory* context, const geos::geom::Geometry::Ptr point);
	void writeLineStringCoordinates(geos::geom::GeometryFactory* context, const geos::geom::Geometry::Ptr line);
	void writePolygonCoordinates(geos::geom::GeometryFactory* context, const geos::geom::Geometry::Ptr polygon);
	void writeMultiPolygonCoordinates(geos::geom::GeometryFactory* context, const geos::geom::Geometry::Ptr multiPolygon);
	void writeGeometryCoordinates(geos::geom::GeometryFactory* context, int type, const geos::geom::Geometry::Ptr geom);

	void writeMultiGeometryCoordinates(
		geos::geom::GeometryFactory* context, const geos::geom::Geometry::Ptr multi, 
		std::function<void(geos::geom::GeometryFactory*, const geos::geom::Geometry::Ptr)> writeFunc)
	{
		writeByte(coordGroupStartChar_);
		int count = GEOSGetNumGeometries_r(context, multi);
		for (int i = 0; i < count; i++) 
		{
			if(i > 0) writeByte(',');
			const geos::geom::Geometry::Ptr geom = GEOSGetGeometryN_r(context, multi, i);
			writeFunc(context, geom);
		}
		writeByte(coordGroupEndChar_);
	}
	#endif

	// ==== Feature Geometries ====

	void writeWayCoordinates(WayPtr way, bool group);
	void writePolygonizedCoordinates(const Polygonizer& polygonizer);

	int precision_ = 7;
	bool latitudeFirst_ = false;
	char coordValueSeparatorChar_ = ',';
	char coordStartChar_ = '[';
	char coordEndChar_ = ']';
	char coordGroupStartChar_ = '[';
	char coordGroupEndChar_ = ']';
};

// \endcond

} // namespace geodesk
