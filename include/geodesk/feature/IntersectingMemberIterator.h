// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <geodesk/feature/MemberIterator.h>
#include <geodesk/feature/FeaturePtr.h>
#include <geodesk/feature/FeatureStore.h>

namespace geodesk {

/// \cond lowlevel
///

// TODO: create a common MemberIteratorBase
// TODO: restrict to inner/outer
class IntersectingMemberIterator : public RelatedIteratorBase<IntersectingMemberIterator,FeaturePtr,1,2>
{
public:
	IntersectingMemberIterator(FeatureStore* store, RelationPtr relation, const Box& bounds) :
		RelatedIteratorBase(store, relation.bodyptr(), Tex::MEMBERS_START_TEX),
		bounds_(bounds)
	{
	}

	bool readAndAcceptRole()    // CRTP override
	{
		if (member_ & MemberFlags::DIFFERENT_ROLE)
		{
			int rawRole = p_.getUnsignedShort();
			p_ += (rawRole & 1) ? 2 : 4;
		}
		return true;
	}

	bool acceptTile(Tip tip) const    // CRTP override
	{
		// Since a tile can contain a twin-tile member
		// (whose bounds could cover the entire extent
		// of the other tile), we check if the query
		// bbox intersects the entire 3x3 grid

		Tile tile = store_->reverseTileIndex().lookupFast(tip);
		return tile.intersectsNeighborsSimple(bounds_);
	}

private:
	Box bounds_;
};

/// \endcond
} // namespace geodesk
