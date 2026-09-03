// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#ifdef GEODESK_WITH_GEOS

#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <string_view>
#include <catch2/catch_test_macros.hpp>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geodesk/geodesk.h>

using namespace geodesk;

TEST_CASE("Intersects with GEOS geometry (#26)")
{
	geos::geom::GeometryFactory::Ptr factory = geos::geom::GeometryFactory::create();

	Features france("d:\\geodesk\\tests\\france.gol");
	Feature paris = france("a[boundary=administrative][admin_level=8][name=Paris]").one();
	std::unique_ptr<geos::geom::Geometry> geomParis = paris.toGeometry(factory.get());
	uint64_t countByFeature = france.intersecting(paris).count();
	uint64_t countByGeom = france.intersecting(factory.get(), geomParis).count();
	REQUIRE(countByFeature > 1000);
	REQUIRE(countByFeature == countByGeom);
}

#endif

