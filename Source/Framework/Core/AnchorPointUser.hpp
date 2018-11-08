//
// AnchorPointUser.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Types.hpp"

namespace Framework
{
    namespace Core
    {
        class AnchorPointUser
        {
        public:
            using Point2D = Types::Point2D;

            AnchorPointUser() = default;
            virtual ~AnchorPointUser() = default;

            virtual void setAnchorPoint(const Point2D& anchorPoint);
            virtual const Point2D& getAnchorPoint() const;

        private:
            Point2D _anchorPoint = Point2D(0.0f, 0.0f);
        };
    }
}
