//
// AnchorPointUser.cpp
//
// @author Roberto Cano
//

#include "AnchorPointUser.hpp"

using namespace Framework::Core;

void AnchorPointUser::setAnchorPoint(const Point2D& anchorPoint)
{
    _anchorPoint = anchorPoint;
}

const AnchorPointUser::Point2D& AnchorPointUser::getAnchorPoint() const
{
    return _anchorPoint;
}