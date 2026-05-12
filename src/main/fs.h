#pragma once

// Compatibility shim: exposes a single `fs` namespace that aliases either
// boost::filesystem or std::filesystem depending on what was found at
// configure time. All other sources should include this header and use
// `fs::path`, `fs::absolute`, etc. — no #if needed at call sites.

#if EXAMPLE_USE_BOOST_FILESYSTEM
  #include <boost/filesystem.hpp>
  namespace fs = boost::filesystem;
#else
  #include <filesystem>
  namespace fs = std::filesystem;
#endif
