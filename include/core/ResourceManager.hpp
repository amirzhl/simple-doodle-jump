// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

// ---------------------------------------------------------------------------
//  ResourceManager<T>  (fulfils the "meaningful template" requirement)
//
//  A generic, reusable cache for any SFML resource type that exposes a
//  `bool loadFromFile(const std::string&)` method -- e.g. sf::Texture,
//  sf::Font and (in the next phase) sf::SoundBuffer.
//
//  Each unique file path is loaded from disk exactly once; subsequent
//  requests return a reference to the already-loaded resource. This avoids
//  redundant, expensive disk I/O and keeps resource ownership in one place.
// ---------------------------------------------------------------------------
template <typename Resource>
class ResourceManager {
public:
	ResourceManager() = default;

	// Non-copyable: the cache owns unique resources.
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	// Return a reference to the resource loaded from `path`, loading it on the
	// first request and caching it for all later requests.
	Resource& acquire(const std::string& path) {
		auto found = resources_.find(path);
		if (found != resources_.end()) {
			return *found->second;
		}

		auto resource = std::make_unique<Resource>();
		if (!resource->loadFromFile(path)) {
			throw std::runtime_error("ResourceManager: failed to load '" + path + "'");
		}

		Resource& ref = *resource;
		resources_.emplace(path, std::move(resource));
		return ref;
	}

	// True if the given path has already been loaded into the cache.
	bool isLoaded(const std::string& path) const {
		return resources_.find(path) != resources_.end();
	}

private:
	std::unordered_map<std::string, std::unique_ptr<Resource>> resources_;
};
