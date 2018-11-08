//
// GameObjectComponent.hpp
//
// @author Roberto Cano
//

#pragma once

#include <string>

#include "Types.hpp"

namespace Framework
{
	using namespace Types;
	namespace Core
	{
		class Instanceable
		{
		public:
			Instanceable() = default;

			const InstanceId& getInstanceId() const;

			void setName(const std::string& name);
			const std::string& getName() const;

		protected:
			static int gInstanceNumber;
			void generateInstanceId(const std::string& classId);

		private:
			InstanceId _instanceId;
			std::string _name;
		};
	}
}
