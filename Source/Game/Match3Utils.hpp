//
// Match3Utils.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Framework/Core/Types.hpp"

namespace Match3
{
	namespace Utils
	{
		template<typename T>
		class Matrix2x2 final
		{
		public:
			using Size = Framework::Types::Size;
			using Position = glm::ivec2;

			explicit Matrix2x2() = default;
			explicit Matrix2x2(const Size& size)
				: _size(size)
			{
				_data.resize(size.width * size.height);
			}

			T& at(const Position& position)
			{
				assert(position.x < _size.width);
				assert(position.y < _size.height);
				return _data.at(position.x * _size.width + position.y);
			}

			const T& at(const Position& position) const
			{
				assert(position.x < _size.width);
				assert(position.y < _size.height);
				return _data.at(position.x * _size.width + position.y);
			}

			const Size& getSize() const
			{
				return _size;
			}

		private:
			Size _size;
			std::vector<T> _data;
		};
	}
}
