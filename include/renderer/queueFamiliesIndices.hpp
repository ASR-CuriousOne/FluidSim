#ifndef QUEUE_FAMILIES_INDICIES_HPP
#define QUEUE_FAMILIES_INDICIES_HPP

#include <cstdint>
#include <optional>


namespace Renderer {
	struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete(){
            return graphicsFamily.has_value() && presentFamily.has_value(); 
        }
    };
}

#endif
